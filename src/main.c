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

//HMS
volatile uint8_t latestTime[3];

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
/* GetTime from the onboard RTC and store in global var                 */
/************************************************************************/
inline void getTime(){
	Date d;
	rtcRead(&d);

	latestTime[0]=d.hour;
	latestTime[1]=d.minute;
	latestTime[2]=d.second;
}


/************************************************************************/
/* [BLOCKING] Wait for serial link request from the GB to send H or M or S  */
/* 		Returns what was asked 'H' or 'M' or 'S'. Others are ignored and stays waiting. */
/************************************************************************/
uint8_t waitForNextRequest(){
	uint8_t val = 0;

	while (1==1) {
		while (!softspi_hasData()) {/* [BLOCKING] WAIT */}

		//something to read, ignore all but latest
		while (softspi_hasData()) {
			val = softspi_getByte();
		}

		if (val == 'H' || val == 'M' || val == 'S'){
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
	LED_PORT |= LED_2;

	//road is clear : send the time component requested
	switch (req){
		case 'H':
			softspi_sendByte(latestTime[0]);
			break;
		case 'M':
			softspi_sendByte(latestTime[1]);
			break;
		case 'S':
			softspi_sendByte(latestTime[2]);
			break;
	}

	//if we sent seconds, ask for and update on the time
	//if (req == 'S'){
		getTime();
	//}

	//have a break so your eyes can see the LED flash
	_delay_ms(10);

	//blink off
	LED_PORT &= ~LED_2;

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
