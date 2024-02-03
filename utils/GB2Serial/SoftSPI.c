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

#define _DEBUG_BLINK


#ifndef SOFTSPI_PORT
	#define SOFTSPI_PORT 	PORTD
	#define SOFTSPI_DIR		DDRD
	#define SOFTSPI_PIN		PIND
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

//internal reception buffer queue
#define SOFTSPI_BUFLEN 		8
volatile uint8_t _spi_buf[SOFTSPI_BUFLEN];
volatile uint8_t _qhead = 0;
volatile uint8_t _qtail = 0;

//reception BYTE buffer (we receive the data bit by bit AND THEN put it in the reception queue when complete)
volatile uint8_t _reception_buf = 0;
//bit mask of which bit we update
volatile uint8_t _reception_buf_bitmask = 1;

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


//store the freshly received byte in the circular buffer
inline void _queueByte (const uint8_t b) {
	//limit case: is full if head + 1 == tail?
	if (((_qhead + 1) % SOFTSPI_BUFLEN) == _qtail){
		//FULL! drop first received byte
		_qtail = (_qtail + 1) % SOFTSPI_BUFLEN;
	}

	//there is space so write: write in head, then head move next
	_spi_buf[_qhead] = b;
	_qhead = (_qhead + 1) % SOFTSPI_BUFLEN;
}


/**
 * TRUE if there is data ready to read in the buffer
 */
uint8_t softspi_hasData() {
	return !(_qhead == _qtail);
}


/**
 * Returns the next byte available in the buffer, or 0.
 * Use softspi_hasData() to confirm if data is ready to read in the first place.
 */
uint8_t softspi_getByte(){
	if (!softspi_hasData()){
		return 0;
	}

	uint8_t b = _spi_buf[_qtail];
	//tail move next
	_qtail = (_qtail + 1) % SOFTSPI_BUFLEN;

	return b;
}



/**
 * Interrupt for pin change value
 *
 */
ISR(SOFTSPI_IntVect) {
#ifdef _DEBUG_BLINK
	//Blink! ~~~~~~~~
	//C0 on
	PORTC |= 0x01;
    //small delay
	_delay_ms(50);
	//C0 off
	PORTC &= ~0x01;
#endif //_DEBUG_BLINK

	//Read MOSI on CLK going UP (see https://mansfield-devine.com/speculatrix/2018/01/avr-basics-spi-on-the-atmega-part-1/)
	uint8_t mosi = SOFTSPI_PIN & (1 << SOFTSPI_MOSI);
	if (mosi != 0){
		//set the bit in the buffer byte (assume the bitmask is at the right place)
		_reception_buf |= _reception_buf_bitmask;
	}
	//else
		//optim: reception buffer is init at 0x00 so overwriting a zero by a zero can be skipped...



	//completed byte?
	if (_reception_buf_bitmask == 0x80){
		//received last bit: store and get ready for next byte

		//store in the circular buffer
		_queueByte(_reception_buf);

		//reset mask & reception buff
		_reception_buf_bitmask = 1;
		_reception_buf = 0;
	}
	else {
		//get ready for next bit
		_reception_buf_bitmask = _reception_buf_bitmask << 1;
	}


	//Not reti() on non-naked ISR (that causes the avr to reset in may case...)
	//reti();
}
