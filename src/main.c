/*
 * GameboyClock
 * main.c
 * 		The MCU part of the project. Supposed to run on the GBLinkClock PCB (https://github.com/AlanFromJapan/pcb-design/tree/master/GameboyLink)
 * 		but you should make it run anywhere you want with minor changes.
 * 		Purpose: get the time one the board's DS1307 or DS3231, and replies time to the GB when asked. That's all.
 *
 *  Created on: Mar 25, 2024
 *      Author: alan
 */


//for delay.h >> defined in the compilation symbols
//#define F_CPU 16000000L

#include <stdlib.h>
#include <string.h>

//System includes
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//for the GB communication
#include "SoftSPI.h"

//other config
#include "config.h"

#include <avr/eeprom.h>

volatile uint16_t latestTime = 0x0000;

/************************************************************************/
/* Setup                                                                */
/************************************************************************/
void mainSetup() {
	//start SPI (using interrupts)
	softspi_setup();

	//Using the LED on D3 for blinking
	//LED is output
	LED_DDR |= LED_MASK;
	//LED off
	LED_PORT &= ~LED_MASK;

	//RTC init
	rtcSetup();
}

/************************************************************************/
/* At startup, checks if time was set at least once. If not does it.    */
/* ==> SEE THE README.MD ON HOW PRESERVE EEPROM BETWEEN PROG CYCLES <== */
/************************************************************************/
void setTimeOnce(uint8_t forceUpdate) {
	uint8_t flag;

	flag = eeprom_read_byte(EEPROM_FLAG_ADDR);

	if (flag != EEPROM_MAGIC_VALUE || forceUpdate != 0) {
		Date d;
		d.second = 05;
		d.minute= 37;
		d.hour = 21 ;
		//d.dayOfWeek = 2;
		d.dayOfMonth = 2;
		d.month = 4;
		d.year = 24;
		rtcWrite(&d);

		eeprom_update_byte(EEPROM_FLAG_ADDR, EEPROM_MAGIC_VALUE);
	}
}

/************************************************************************/
/* GetTime from the onboard RTC                                         */
/*    Returns:0xhhmm                                                    */
/************************************************************************/
uint16_t getTime(){
	Date d;
	rtcRead(&d);

	return ((uint16_t)(d.hour << 8)) | (uint16_t)(d.minute);
}


/************************************************************************/
/* [BLOCKING] Wait for serial link request from the GB to send H or M              */
/* 		Returns what was asked 'H' or 'M'. Others are ignored and stays waiting. */
/************************************************************************/
uint8_t waitForNextRequest(){
	uint8_t val = 0;

	while (1==1) {
		while (!softspi_hasData()) {/* [BLOCKING] WAIT */}

		//something to read, ignore all but latest
		while (softspi_hasData()) {
			val = softspi_getByte();
		}

		if (val == 'H' || val == 'M'){
			return val;
		}
		//else : resume waiting
	}
}


/************************************************************************/
/* SendTime (THE payload)                                               */
/************************************************************************/
void sendTime(const uint8_t req){

	//wait for any previous data to be sent
	while (!softspi_bytesent()){
		;
	}

	//blink on
	LED_PORT |= LED_MASK;

	//road is clear
	if (req == 'H'){
		//send hours
		softspi_sendByte((uint8_t)(latestTime >> 8));
	}
	else {
		//send minutes
		softspi_sendByte((uint8_t)(latestTime));
	}

	//if we sent minutes, ask for and update on the time
	if (req == 'M'){
		latestTime = getTime();
	}

	//have a break so your eyes can see the LED flash
	_delay_ms(20);

	//blink off
	LED_PORT &= ~LED_MASK;

}

/************************************************************************/
/* Main                                                                 */
/************************************************************************/
int main(void) {
	//Setup!
	mainSetup();

	//set time in case
	setTimeOnce(0);

	//needed for SoftSPI: enable the interrupts
	sei();

	while(1) {
		uint8_t req = waitForNextRequest();
		sendTime(req);
	}

	return 0;
}
