/*
 * SoftSPI-config.h
 *
 *  Created on: Feb 14, 2024
 *      Author: alan
 */

#ifndef SOFTSPI_CONFIG_H_
#define SOFTSPI_CONFIG_H_

/**
 * CONSTANTS
 */
#define SOFTSPI_CLK_READ_UP		1
#define SOFTSPI_CLK_READ_DOWN	2
#define SOFTSPI_CLK_READ_CHG 	0

#define SOFTSPI_BITORDER_LSB 0
#define SOFTSPI_BITORDER_MSB 1

/**
 * CONFIG TO BE EDITED DEPENDING ON YOUR SETUP
 * START ...
 */

//Soft SPI using default settings (override defines HERE before include command)
// YOU HAVE TO SET THEM ALL

#define SOFTSPI_PORT 	PORTD
#define SOFTSPI_DIR		DDRD
#define SOFTSPI_PIN		PIND
#define SOFTSPI_CLK		7
#define SOFTSPI_MOSI	6
#define SOFTSPI_MISO	5

//On which clock change to read
#define SOFTSPI_CLK_READFLAG 	SOFTSPI_CLK_READ_DOWN

//Expect to receive LSB first (fill byte buffer right to left) or MSB
#define SOFTSPI_BITORDER		SOFTSPI_BITORDER_MSB

//Used PCIEx interrupt (0-1-2 available): IT DEPENDS ON THE PIN YOU USE FOR MOSI.
//  So read the doc (ie. Atmega328 port D7 = PCINT23 therefore it's PCIE2)
//  To make sure ONLY that pin triggers the interrupt, you have to enable it
//  and that is done with PCMSKx register
#define SOFTSPI_PCIE			2
#define SOFTSPI_IntVect			PCINT2_vect
//in the mask PCMSK2, the PCINT23 (D7) is the leftmost bit (0x80)
#define SOFTSPI_IntPinMask		PCMSK2
#define SOFTSPI_IntPinMaskBit	7

/**
 * ... END
 * (CONFIG TO BE EDITED DEPENDING ON YOUR SETUP)
 */


#endif /* SOFTSPI_CONFIG_H_ */
