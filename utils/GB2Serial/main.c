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
#include <string.h>

//System includes
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "serialComm.h"

//Soft SPI using default settings (override defines HERE before include command)
// YOU HAVE TO SET THEM ALL or NONE

////On which clock change to read => GB SENDS on falling edge and READS on rising edge
//#define SOFTSPI_CLK_READFLAG 	SOFTSPI_CLK_READ_DOWN
//
////Expect to receive LSB first (fill byte buffer right to left) or MSB => GB uses MSB
//#define SOFTSPI_BITORDER		SOFTSPI_BITORDER_MSB

#include "SoftSPI.h"

//test message to send
const char TEST_MESSAGE[] = "ABCDEFGH";

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


/**
 * Testing function for RECEPTION on the SPI and echo send to Serial UART (PC)
 *
 */
inline void test_SoftSPI_reception() {
	uint8_t val = 0;
	while (softspi_hasData()) {
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

/**
 * Testing function for SENDING on the SPI and echo send to Serial UART (PC)
 *
 */
static inline void test_SoftSPI_emission(const char c) {
	USART_Transmit(c);
	USART_SendString(" ");

	while (!softspi_bytesent()){
		;
	}
	//road is clear
	softspi_sendByte(c);

	_delay_ms(200);
}


/************************************************************************/
/* Main                                                                 */
/************************************************************************/
int main(void) {
	//Setup!
	mainSetup();

	cli();
	USART_SendString("\n\n------------------------\n");
	USART_SendString("Let's start.\n");


	// ******************* Testing EMISSION *******************
	USART_SendString("Testing EMISSION (from muC to GB + echo to Serial UART).\n");
	sei();

	while (1) {
		for (uint8_t i = 0; i < strlen(TEST_MESSAGE); i++){
			test_SoftSPI_emission(TEST_MESSAGE[i]);
		}
		_delay_ms(1000);
	}

/*
	// ******************* Testing RECEPTION *******************
	USART_SendString("Testing RECEPTION (from GB to muC + echo to Serial UART).\n");
	sei();

	while (1) {
		test_SoftSPI_reception();
	}
*/
	return 0;
}
