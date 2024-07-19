/*
 * main.c
 *
 *  Created on: Jul 10, 2024
 *      Author: alan
 */

#include "avr/io.h"
#include "util/delay.h"

#include "AVR156/TWI_master.h"
#include "RTC/DS1307.h"

#include "SoftSPI-config.h"
#include "SoftSPI.h"


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

//
//	struct Date t ; //assign me!
//
//	//road is clear : send the time component requested
//	switch (req){
//		case 'H':
//			softspi_sendByte(t.h);
//			break;
//		case 'M':
//			softspi_sendByte(t.m);
//			break;
//		case 'S':
//			softspi_sendByte(t.s);
//			break;
//	}



	//have a break so your eyes can see the LED flash
	_delay_ms(10);


}


int main (){
	//leds out
	DDRB = 0x03;

	softspi_setup();

	while(1) {

		PORTB |= 0x02;
		PORTB &= ~(0x01);

		_delay_ms(10);

		uint8_t req = waitForNextRequest();
		sendTime(req);

		PORTB |= 0x01;
		PORTB &= ~(0x02);
		_delay_ms(10);

	}

	return 0;
}
