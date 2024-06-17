/*
 * config.h
 *
 *  Created on: Mar 25, 2024
 *      Author: alan
 */

#ifndef CONFIG_H_
#define CONFIG_H_

//includes in that order
#include "RTC/RTC-shared.h"
//Selected RTC. Include one only, it defines the macros for time (rtc...())
#include "RTC/DS1307.h"


//Led are on D3 & D5
#define LED_1		0x08
#define LED_2		0x20
#define LED_MASK	(LED_1 | LED_2)
#define LED_PORT	PORTD
#define LED_DDR		DDRD




//where to store the flag if time was set or not
#define EEPROM_FLAG_ADDR	((uint8_t*)0x0000)
#define EEPROM_MAGIC_VALUE	((uint8_t)123)



#endif /* CONFIG_H_ */
