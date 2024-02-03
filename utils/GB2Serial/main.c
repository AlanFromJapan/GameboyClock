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
#include <avr/interrupt.h>

#include "serialComm.h"

//Soft SPI using default settings (override defines HERE before include command)
#include "SoftSPI.h"

/************************************************************************/
/* Setup                                                                */
/************************************************************************/
void mainSetup() {
	//Start Serial
	serialHardwareInit();

	//start SPI (using interrupts)
	softspi_setup();

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

	cli();
	USART_SendString("\n\n------------------------");
	USART_SendString("\nLet's start.\n");
	sei();

    while(1) {

    	uint8_t val = 0;

    	while (softspi_hasData()){
    		val = softspi_getByte();
			char vBuff[15];
			itoa(val, vBuff, 16);
			USART_SendString("0x");
			USART_SendString(vBuff);
			USART_SendString("(");
			itoa(val, vBuff, 10);
			USART_SendString(vBuff);
			USART_SendString(")\n");
    	}

		_delay_ms(100);


    }
}
