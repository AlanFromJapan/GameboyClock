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
}

/************************************************************************/
/* GetTime from the onboard RTC                                         */
/*    Returns:0xhhmm                                                    */
/************************************************************************/
uint16_t getTime(){
	//returns h=12 (0x0C) and m=34 (0x22)
	return 0x0C22;
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
	uint16_t tim = getTime();

	//wait for any previous data to be sent
	while (!softspi_bytesent()){
		;
	}

	//blink on
	LED_PORT |= LED_MASK;

	//road is clear
	if (req == 'H'){
		//send hours
		softspi_sendByte((uint8_t)(tim >> 8));
	}
	else {
		//send minutes
		softspi_sendByte((uint8_t)(tim));
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

	//needed for SoftSPI: enable the interrupts
	sei();

	while(1) {
		uint8_t req = waitForNextRequest();
		sendTime(req);
	}

	return 0;
}
