/*
 * SoftSPI.c
 *   A software version of SPI **SLAVE** just by me, for the fun.
 *
 *  Created on: Feb 1, 2024
 *      Author: alan
 */


#include "SoftSPI.h"
#include "SoftSPI-config.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//#define _DEBUG_BLINK


//internal reception buffer queue
#define SOFTSPI_BUFLEN 		8
volatile uint8_t _spi_buf[SOFTSPI_BUFLEN];
volatile uint8_t _qhead = 0;
volatile uint8_t _qtail = 0;

//reception BYTE buffer (we receive the data bit by bit AND THEN put it in the reception queue when complete)
volatile uint8_t _reception_buf = 0;
//bit mask of which bit we update (must be initiated to the right value or will read strange values)
volatile uint8_t _reception_buf_bitmask =
#if SOFTSPI_BITORDER == SOFTSPI_BITORDER_LSB
		0x01;
#endif //SOFTSPI_BITORDER_LSB
#if SOFTSPI_BITORDER == SOFTSPI_BITORDER_MSB
		0x80;
#endif //SOFTSPI_BITORDER_MSB

//emission BYTE buffer (we send the data bit by bit)
volatile uint8_t _emission_buf = 0;
//bit mask of which bit we update (0x00 means nothing to send or done sending)
volatile uint8_t _emission_buf_bitmask = 0;


/**
 * Call once first to set ports
 */
void softspi_setup(){
	//CLOCK as input
	SOFTSPI_DIR &= ~(1 << SOFTSPI_CLK);
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
 * Sends one byte of data over the SPI (ASYNCH!). The functions returns immediately BUT the byte is not sent yet.
 * The clock is managed by the master (not us) so the byte is sent asynchronously.
 * Call softspi_bytesent() to know if the road is clear to send another one (this function will NOT check).
 */
void softspi_sendByte(uint8_t b){
	_emission_buf = b;

#if SOFTSPI_BITORDER == SOFTSPI_BITORDER_LSB
	_emission_buf_bitmask = 0x01;
#endif //SOFTSPI_BITORDER_LSB

#if SOFTSPI_BITORDER == SOFTSPI_BITORDER_MSB
	_emission_buf_bitmask = 0x80;
#endif //SOFTSPI_BITORDER_MSB

}

/**
 * Returns if the previous to-be-sent byte was sent and you can send another one.
 * (non blocking)
 */
int softspi_bytesent(){
	return _emission_buf_bitmask == 0;
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

	// ******************************** RECEPTION ********************************************
	//Read on rising, falling or both edges?
	if (SOFTSPI_CLK_READFLAG == SOFTSPI_CLK_READ_CHG
		|| (SOFTSPI_CLK_READFLAG == SOFTSPI_CLK_READ_UP && ((SOFTSPI_PIN & (1 << SOFTSPI_CLK))) != 0)
		|| (SOFTSPI_CLK_READFLAG == SOFTSPI_CLK_READ_DOWN && ((SOFTSPI_PIN & (1 << SOFTSPI_CLK))) == 0)) {

		uint8_t mosi = SOFTSPI_PIN & (1 << SOFTSPI_MOSI);
		if (mosi != 0){
			//set the bit in the buffer byte (assume the bitmask is at the right place)
			_reception_buf |= _reception_buf_bitmask;
		}
		//else
			//optim: reception buffer is init at 0x00 so overwriting a zero by a zero can be skipped...

#if SOFTSPI_BITORDER == SOFTSPI_BITORDER_LSB
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
#endif //SOFTSPI_BITORDER == SOFTSPI_BITORDER_LSB
#if SOFTSPI_BITORDER == SOFTSPI_BITORDER_MSB
		//completed byte?
		if (_reception_buf_bitmask == 0x01){
			//received last bit: store and get ready for next byte

			//store in the circular buffer
			_queueByte(_reception_buf);

			//reset mask & reception buff
			_reception_buf_bitmask = 0x80;
			_reception_buf = 0;
		}
		else {
			//get ready for next bit
			_reception_buf_bitmask = _reception_buf_bitmask >> 1;
		}
#endif //SOFTSPI_BITORDER == SOFTSPI_BITORDER_MSB

	}


	// ******************************** EMISSION ********************************************
	//Something to send and correct edge?
	//BEWARE It's the **OPPOSITE** edge of reading (read on failing => write on rising)!
	if (_emission_buf_bitmask != 0x00
		&&
		(
			SOFTSPI_CLK_READFLAG == SOFTSPI_CLK_READ_CHG
		|| (SOFTSPI_CLK_READFLAG == SOFTSPI_CLK_READ_UP && ((SOFTSPI_PIN & (1 << SOFTSPI_CLK))) == 0)
		|| (SOFTSPI_CLK_READFLAG == SOFTSPI_CLK_READ_DOWN && ((SOFTSPI_PIN & (1 << SOFTSPI_CLK))) != 0)
		)){


#if SOFTSPI_BITORDER == SOFTSPI_BITORDER_LSB
todo!!
#endif //SOFTSPI_BITORDER == SOFTSPI_BITORDER_LSB
#if SOFTSPI_BITORDER == SOFTSPI_BITORDER_MSB
		//Sending is ALSO the OPPOSITE BIT ORDER as receiving, so MSB (on receive) means sending LSB
		if ((_emission_buf & _emission_buf_bitmask) == 0){
			//send a zero
			SOFTSPI_PORT &= ~(1 << SOFTSPI_MISO);
		}
		else {
			//send a one
			SOFTSPI_PORT |= (1 << SOFTSPI_MISO);
		}

		//finished?
		if (_emission_buf_bitmask == 0x01) {
			//finished. This is a flag that will allow next byte to be put in the sending buffer
			_emission_buf_bitmask = 0;
		}
		else{
			//get ready for next byte
			_emission_buf_bitmask = _emission_buf_bitmask >> 1;
		}
#endif //SOFTSPI_BITORDER == SOFTSPI_BITORDER_MSB

	}


	//Not reti() on non-naked ISR (that causes the avr to reset in may case...)
	//reti();
}
