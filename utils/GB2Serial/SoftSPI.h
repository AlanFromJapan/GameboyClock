/*
 * SoftSPI.h
 *
 *  Created on: Feb 1, 2024
 *      Author: alan
 */

#ifndef SOFTSPI_H_
#define SOFTSPI_H_

#include <stdint.h>

#define SOFTSPI_CLK_READ_UP		1
#define SOFTSPI_CLK_READ_DOWN	2
#define SOFTSPI_CLK_READ_CHG 	0

#define SOFTSPI_BITORDER_LSB 0
#define SOFTSPI_BITORDER_MSB 1

/**
 * To function, define somewhere BEFORE including this .h file

#define SOFTSPI_PORT 	PORTD
#define SOFTSPI_DIR		DDRD
#define SOFTSPI_CLK		7
#define SOFTSPI_MOSI	6
#define SOFTSPI_MISO	5
//Used PCIEx interrupt (0-1-2 available): IT DEPENDS ON THE PIN YOU USE FOR MOSI.
//  So read the doc (ie. Atmega328 port D7 = PCINT23 therefore it's PCIE2)
#define SOFTSPI_PCIE	2
#define SOFTSPI_IntVect	PCINT2_vect
//in the mask PCMSK2, the PCINT23 (D7) is the leftmost bit (0x80)
#define SOFTSPI_IntPinMask		PCMSK2
#define SOFTSPI_IntPinMaskBit	7
 */

/**
 * Call once first to set ports
 */
void softspi_setup();

/**
 * TRUE if there is data ready to read in the buffer
 */
uint8_t softspi_hasData() ;

/**
 * Returns the next byte available in the buffer, or 0.
 * Use softspi_hasData() to confirm if data is ready to read in the first place.
 */
uint8_t softspi_getByte();

/**
 * Sends one byte of data over the SPI (ASYNCH!). The functions returns immediately BUT the byte is not sent yet.
 * The clock is managed by the master (not us) so the byte is sent asynchronously.
 * Call softspi_bytesent() to know if the road is clear to send another one (this function will NOT check).
 */
void softspi_sendByte(uint8_t b);

/**
 * Returns if the previous to-be-sent byte was sent and you can send another one.
 * (non blocking)
 */
int softspi_bytesent();


#endif /* SOFTSPI_H_ */
