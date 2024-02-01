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



#endif /* SOFTSPI_H_ */
