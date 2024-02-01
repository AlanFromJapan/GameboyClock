/*
 * SoftSPI.h
 *
 *  Created on: Feb 1, 2024
 *      Author: alan
 */

#ifndef SOFTSPI_H_
#define SOFTSPI_H_

/**
 * To function, define somewhere BEFORE including this .h file

#define SOFTSPI_PORT 	PORTD
#define SOFTSPI_DIR		DDRD
#define SOFTSPI_CLK		7
#define SOFTSPI_MOSI	6
#define SOFTSPI_MISO	5

 */

/**
 * Call once first to set ports
 */
void softspi_setup();



#endif /* SOFTSPI_H_ */
