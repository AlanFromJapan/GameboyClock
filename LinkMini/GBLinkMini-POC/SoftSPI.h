/*
 * SoftSPI.h
 *   Software implementation of a SPI **SLAVE**.
 *
 *  Created on: Feb 1, 2024
 *      Author: alan
 */

#ifndef SOFTSPI_H_
#define SOFTSPI_H_

#include <stdint.h>


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
