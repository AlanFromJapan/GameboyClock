/*
 * SoftSPI.c
 *
 *  Created on: Feb 1, 2024
 *      Author: alan
 */


#include "SoftSPI.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#ifndef SOFTSPI_PORT
	#define SOFTSPI_PORT 	PORTD
	#define SOFTSPI_DIR		DDRD
	#define SOFTSPI_CLK		7
	#define SOFTSPI_MOSI	6
	#define SOFTSPI_MISO	5

	//Used PCIEx interrupt (0-1-2 available): IT DEPENDS ON THE PIN YOU USE FOR MOSI.
	//  So read the doc (ie. Atmega328 port D7 = PCINT23 therefore it's PCIE2)
	//  To make sure ONLY that pin triggers the interrupt, you have to enable it
	//  and that is done with PCMSKx register
	#define SOFTSPI_PCIE			2
	#define SOFTSPI_IntVect			PCINT2_vect
	//in the mask PCMSK2, the PCINT23 (D7) is the leftmost bit (0x80)
	#define SOFTSPI_IntPinMask		PCMSK2
	#define SOFTSPI_IntPinMaskBit	7
#endif


#define SOFTSPI_BUFLEN 		8
uint8_t _spi_buf[SOFTSPI_BUFLEN];
uint8_t _qhead = 0;

/**
 * Call once first to set ports
 */
void softspi_setup(){
	//MOSI as input
	SOFTSPI_DIR &= ~(1 << SOFTSPI_MOSI);
	//MISO as output
	SOFTSPI_DIR |= (1 << SOFTSPI_MISO);

	//use one of the PCINT (pin change) interrupt for reading data
	PCICR |= (1 << SOFTSPI_PCIE);

	//enable the pin in the mask register
	SOFTSPI_IntPinMask |= (1 << SOFTSPI_IntPinMaskBit);

	//let's roll: interrupts ON
	sei();
}

/**
 * Interrupt for pin change value
 *
 */
ISR(SOFTSPI_IntVect) {
	//Blink! ~~~~~~~~
	//C0 on
	PORTC |= 0x01;
    //small delay
	_delay_ms(50);
	//C0 off
	PORTC &= ~0x01;

	//Not reti() on non-naked ISR (that causes the avr to reset in may case...)
	//reti();
}
