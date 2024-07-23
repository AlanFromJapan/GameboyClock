/*
 * DS1307.c
 *
 *  Created on: Apr 1, 2024
 *      Author: alan
 */


#ifndef __RTC1307_C__
#define __RTC1307_C__

#include "DS1307.h"

#include <avr/io.h>



//set debug mode
//#define SERIAL_DEBUG

#ifdef SERIAL_DEBUG
#include "serialComm.h"
#endif

extern void twi_disable();
extern void twi_init();
extern char write_data(unsigned char* data, char bytes);
extern char read_bytes(unsigned char* data, char bytes);



/************************************************************************/
/* READS time															*/
/************************************************************************/
void readTime1307(struct Date* pTimeDate){
	uint8_t i2cbuff [16];

	i2cbuff[0] = 0x00;
	write_data(i2cbuff, 1); //move to reg 0

	//read all at once
	read_bytes(i2cbuff, 7);

	//bit 7 is the CH flag so ignore it
	pTimeDate->second = bcdToDec(i2cbuff[0] & 0x7f);
	pTimeDate->minute = bcdToDec(i2cbuff[1]);
	//bit 7~6 are for control
	pTimeDate->hour = bcdToDec(i2cbuff[2] & 0x3f);
	pTimeDate->dayOfWeek = bcdToDec(i2cbuff[3]);
	pTimeDate->dayOfMonth = bcdToDec(i2cbuff[4]);
	pTimeDate->month = bcdToDec(i2cbuff[5]);

	pTimeDate->year = bcdToDec(i2cbuff[6]);


}

/************************************************************************/
/* SETS time															*/
/************************************************************************/
void setTimeDate1307(struct Date* pDateTime){
	uint8_t i2cbuff [16];

	//got to register 0x00 (seconds)
	i2cbuff[0] = 0x00;
	write_data(i2cbuff, 1); //move to reg 0


	//bit 7 of register 0x00 (seconds) is the CH (Clock Halt) -> if set to 1 then the clock stops so force to zero
	i2cbuff[0] = decToBcd(pDateTime->second) & 0x7f;
	i2cbuff[1] = decToBcd(pDateTime->minute);
	//bit 7 is ignored, bit 6 is 24h format: low means 24h mode, high means 12h AM/PM.
	i2cbuff[1] = decToBcd(pDateTime->hour) & 0x3f;
	i2cbuff[1] = decToBcd(pDateTime->dayOfWeek);
	i2cbuff[1] = decToBcd(pDateTime->dayOfMonth);
	i2cbuff[1] = decToBcd(pDateTime->month);
	i2cbuff[1] = decToBcd(pDateTime->year);


	write_data(i2cbuff, 7);

}

//=====================================================================================

void setupDS1307(){
	uint8_t i2cbuff [16];

	twi_init();

	//got to the control register at 0x07
	i2cbuff[0] = 0x07;
	//no square wave output
	i2cbuff[1] = 0x00;
	write_data(i2cbuff, 2);

}




#endif // __RTC1307_C__
