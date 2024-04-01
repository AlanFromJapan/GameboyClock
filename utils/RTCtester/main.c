/*
 * main.c
 *
 *  Created on: Mar 30, 2024
 *      Author: alan
 */


//System includes
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "serialComm.h"

#include "RTC-shared.h"
#include "DS1307.h"

#include <avr/eeprom.h>

//where to store the flag if time was set or not
#define EEPROM_FLAG_ADDR	((const uint8_t*)0x0000)
#define EEPROM_MAGIC_VALUE	((uint8_t)123)

/************************************************************************/
/* Setup                                                                */
/************************************************************************/
void mainSetup() {
	//Start Serial
	serialHardwareInit();

	//DS1307 init
	setupDS1307();
}


/************************************************************************/
/* At startup, checks if time was set at least once. If not does it.    */
/* ==> SEE THE README.MD ON HOW PRESERVE EEPROM BETWEEN PROG CYCLES <== */
/************************************************************************/
void setTimeOnce() {
	uint8_t flag;

	flag = eeprom_read_byte(EEPROM_FLAG_ADDR);

	if (flag != EEPROM_MAGIC_VALUE) {
		USART_SendString("**Setting time**\n");

		Date d;
		d.second = 40;
		d.minute= 59;
		d.hour = 23 ;
		d.dayOfMonth = 31;
		d.month = 3;
		d.year = 24;
		setTimeDate1307(&d);

		eeprom_update_byte(EEPROM_FLAG_ADDR, EEPROM_MAGIC_VALUE);
	}
}

/************************************************************************/
/* Main                                                                 */
/************************************************************************/
int main(void) {
	//Setup!
	mainSetup();

	cli();
	USART_SendString("\n\n------------------------\n");
	USART_SendString("Let's start.\n");

	setTimeOnce();

	Date d;

	while(1) {

		readTime1307(&d);

		char vBuff[20];
		dateToString(vBuff, &d);
		USART_SendString(vBuff);
		USART_SendString(" ");

		_delay_ms(1000);
	}

	return 0;
}
