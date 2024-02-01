/*
 * GB2Serial
 *	Reads on Gameboy Link port and pass it to the serial interface of the ATmega.
 *	This is a intermediate step of the bigger project.
 *
 *  Created on: Feb 1, 2024
 *      Author: alan
 */


//for delay.h >> defined in the compilation symbols
//#define F_CPU 16000000L

#include <stdlib.h>

//System includes
#include <avr/io.h>
#include <util/delay.h>

#include "serialComm.h"

/************************************************************************/
/* Setup                                                                */
/************************************************************************/
void mainSetup() {
	//Start Serial
	serialHardwareInit();

	//blinking led, just to be sure things work
	//C0 as out
	DDRC |= 0x01;
	//C0 off
	PORTC &= ~0x01;

}

/************************************************************************/
/* Main                                                                 */
/************************************************************************/
int main(void) {
	//Setup!
	mainSetup();

	USART_SendString("\n\n------------------------");
	USART_SendString("\nLet's start.\n");

    while(1) {

    	uint8_t val = 0;

		char vBuff[15];
		itoa(val, vBuff, 5);
		USART_SendString(vBuff);

		USART_SendString(". ");


		//Blink! ~~~~~~~~
    	//C0 off
    	PORTC &= ~0x01;
	    //small delay
		_delay_ms(250);
		//C0 on
		PORTC |= 0x01;
	    //small delay
		_delay_ms(250);


    }
}
