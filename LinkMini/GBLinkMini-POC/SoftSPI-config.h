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
//Read from Master (= receive) on clock RISING edge
#define SOFTSPI_CLK_READ_UP		1
//Read from Master (= receive) on clock FALLING edge
#define SOFTSPI_CLK_READ_DOWN	2
//Read from Master (= receive) on clock Changing edge (both rise n fall)
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
#define SOFTSPI_CLK		2
#define SOFTSPI_MOSI	3
#define SOFTSPI_MISO	4

//On which clock change to read and assumes writes TO Master (= send) on the opposite edge
#define SOFTSPI_CLK_READFLAG 	SOFTSPI_CLK_READ_DOWN

//Expect to receive LSB first (fill byte buffer right to left) or MSB
#define SOFTSPI_BITORDER		SOFTSPI_BITORDER_MSB

//Used PCIEx interrupt (0-1-2 available): IT DEPENDS ON THE PIN YOU USE FOR *CLK*.
//  So read the doc (ie. Atmega328 port D2 = PCINT18 therefore it's PCIE2)
//  To make sure ONLY that pin triggers the interrupt, you have to enable it
//  and that is done with PCMSKx register

// HERE values for the GBLinkClock PCB board with Tiny2313 and CLK on D2 => it's INT0
// GBLinkClock board https://github.com/AlanFromJapan/pcb-design/tree/master/GameboyLink
//#define SOFTSPI_PCIE			0
#define SOFTSPI_IntVect			INT0_vect
//#define SOFTSPI_IntPinMask		PCMSK0
//#define SOFTSPI_IntPinMaskBit	2

/**
 * ... END
 * (CONFIG TO BE EDITED DEPENDING ON YOUR SETUP)
 */


#endif /* SOFTSPI_CONFIG_H_ */
