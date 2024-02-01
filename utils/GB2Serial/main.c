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


#define SOFTSPI_PORT 	PORTD
#define SOFTSPI_DIR		DDRD
#define SOFTSPI_CLK		7
#define SOFTSPI_MOSI	6
#define SOFTSPI_MISO	5
//Used PCIEx interrupt (0-1-2 available): IT DEPENDS ON THE PIN YOU USE FOR MOSI.
//  So read the doc (ie. Atmega328 port D7 = PCINT23 therefore it's PCIE2)
//  To make sure ONLY that pin triggers the interrupt, you have to enable it
//  and that is done with PCMSKx register
#define SOFTSPI_PCIE	2
#define SOFTSPI_IntVect	PCINT2_vect
//in the mask PCMSK2, the PCINT23 (D7) is the leftmost bit (0x80)
#define SOFTSPI_IntPinMask		PCMSK2
#define SOFTSPI_IntPinMaskBit	7
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

    	cli();
		char vBuff[15];
		itoa(val, vBuff, 5);
		USART_SendString(vBuff);

		USART_SendString(". ");
		sei();

		_delay_ms(1000);


    }
}
