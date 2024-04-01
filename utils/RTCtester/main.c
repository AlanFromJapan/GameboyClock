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

#include "twi.h"
#include "DS1307.h"

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
/* Main                                                                 */
/************************************************************************/
int main(void) {
	//Setup!
	mainSetup();

	cli();
	USART_SendString("\n\n------------------------\n");
	USART_SendString("Let's start.\n");

	Date d;
//	d.second =55;
//	d.minute= 1;
//	d.hour = 2 ;
//	d.dayOfMonth = 31;
//	d.month = 3;
//	d.year = 24;
//	setTimeDate1307(&d);

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
