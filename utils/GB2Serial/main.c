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


#include "SoftSPI.h"


#define DO_BLINK

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
inline static void test_SoftSPI_reception() {
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
		USART_SendString("=");
		USART_Transmit(val);
		USART_SendString(")\n");
	}
	_delay_ms(100);
}

/**
 * Testing function for SENDING on the SPI and echo send to Serial UART (PC)
 *
 */
static inline void test_SoftSPI_emission(const char c) {
	char vBuff[5];
	itoa(c, vBuff, 16);
	USART_SendString("0x");
	USART_SendString(vBuff);
	USART_SendString("(");
	itoa(c, vBuff, 10);
	USART_SendString(vBuff);
	USART_SendString(") ");

	while (!softspi_bytesent()){
		;
	}

#ifdef DO_BLINK
	//C0 on
	PORTC != 0x01;
#endif //DO_BLINK

	//road is clear
	softspi_sendByte(c);

	_delay_ms(20);

#ifdef DO_BLINK
	//C0 off
	PORTC &= ~0x01;
#endif //DO_BLINK

}


/**
 * SENDS a message one byte at a time and loop
 *
 */
void TEST_Send_Message(){
	// ******************* Testing EMISSION one byte at a time *******************
	USART_SendString("Testing EMISSION byte per byte\n");
	sei();

	while (1) {
		for (uint8_t i = 0; i < strlen(TEST_MESSAGE); i++){
			test_SoftSPI_emission(TEST_MESSAGE[i] - 'A' );
			//test_SoftSPI_emission(i+1);
		}
	}
}

/**
 * Sends continuously the same byte 172=0xAC=0b10101100
 *
 */
void TEST_Send_1byte(){
	// ******************* Testing EMISSION one SAME BYTE 10101100b = 172 *******************
	USART_SendString("Testing EMISSION same byte 172 (10101100b)\n");
	sei();

	while (1) {
		test_SoftSPI_emission((uint8_t)172);
	}
}


/**
 * Receives char by char from GB and echo to Serial UART
 *
 */
void TEST_Receive_Echo2Serial(){
	USART_SendString("Testing RECEPTION (from GB to muC + echo to Serial UART).\n");
	sei();

	while (1) {
		test_SoftSPI_reception();
	}
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


	// ******************* Testing EMISSION one SAME BYTE 10101100b = 172 *******************
	//TEST_Send_1byte();


	// ******************* Testing EMISSION one byte at a time *******************
	//TEST_Send_Message();


	// ******************* Testing RECEPTION *******************
	TEST_Receive_Echo2Serial();


	return 0;
}
