
#ifndef __RTC1307_H__
#define __RTC1307_H__

//for sprintf()
#include <stdio.h>

#include "twi.h"

//for the TWI constants
#include <util/twi.h>

//set debug mode
//#define SERIAL_DEBUG

#ifdef SERIAL_DEBUG
#include "serialComm.h"
#endif

#define DS1307_I2C_ADDRESS 0x68  // This is the I2C address


typedef struct {
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
  uint8_t dayOfWeek;
  uint8_t dayOfMonth;
  uint8_t month;
  uint8_t year;
} 
Date;

// Convert normal decimal numbers to binary coded decimal
uint8_t decToBcd(uint8_t val)
{
  return ( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
uint8_t bcdToDec(uint8_t val)
{
  return ( (val/16*10) + (val%16) );
}

void readTime1307(Date* pTimeDate){
	uint8_t status = 0x00;

	TWIStart();
#ifdef SERIAL_DEBUG
	char vBuff[15];
	USART_Transmit('A');
#endif
	if (TWIGetStatus() != TW_START){
#ifdef SERIAL_DEBUG
USART_Transmit('B');
#endif
		//something wrong
		pTimeDate->hour = TWIGetStatus();
		pTimeDate->minute = pTimeDate->hour;
		pTimeDate->second = pTimeDate->minute;
		return;
	}

#ifdef SERIAL_DEBUG
USART_Transmit('C');
#endif

	TWIWrite((DS1307_I2C_ADDRESS << 1) | TW_WRITE);
#ifdef SERIAL_DEBUG
USART_Transmit('D');
#endif

	TWIWrite(0x00); //move to reg 0


	_delay_ms(1);
	status = TWIGetStatus();

#ifdef SERIAL_DEBUG
	itoa(status, vBuff, 16);
	USART_SendString(vBuff);
	USART_Transmit('E');
#endif
	TWIStop();
#ifdef SERIAL_DEBUG
USART_Transmit('F');
#endif

	_delay_ms(20); //in case

	TWIStart();
#ifdef SERIAL_DEBUG
USART_Transmit('G');
#endif

	TWIWrite((DS1307_I2C_ADDRESS << 1) | TW_READ);
#ifdef SERIAL_DEBUG
USART_Transmit('H');
#endif

	if (TWIGetStatus() != TW_MR_SLA_ACK){
#ifdef SERIAL_DEBUG
USART_Transmit('I');
#endif

		//something wrong
		pTimeDate->hour = TWIGetStatus();
		pTimeDate->minute = pTimeDate->hour;
		pTimeDate->second = pTimeDate->minute;

		//Finished the read phase
		TWIStop();

		return;
	}

	//Read 1: Seconds
	status = TWIReadNACK();
	if (TWIGetStatus() != TW_MR_DATA_NACK){
		//something wrong
		pTimeDate->hour = TWIGetStatus();
		pTimeDate->minute = pTimeDate->hour;
		pTimeDate->second = pTimeDate->minute;
		return;
	}

	pTimeDate->second = bcdToDec(status & 0x7f);
	status = TWIReadNACK();
	pTimeDate->minute = bcdToDec(status);
	status = TWIReadNACK();
	pTimeDate->hour = bcdToDec(status & 0x3f);
	status = TWIReadNACK();
	pTimeDate->dayOfWeek = bcdToDec(status);
	status = TWIReadNACK();
	pTimeDate->dayOfMonth = bcdToDec(status);
	status = TWIReadNACK();
	pTimeDate->month = bcdToDec(status);
	status = TWIReadNACK();
	pTimeDate->year = bcdToDec(status);

	//Finished the read phase
	TWIStop();

}

//=====================================================================================

void setTimeDate1307(Date* pDateTime){
	uint8_t status = 0;
#ifdef SERIAL_DEBUG
	char vBuff[15];

	USART_SendString("\nSet:");
#endif

	TWIInit();


	TWIStart();

	_delay_ms(1);
	status = TWIGetStatus();

#ifdef SERIAL_DEBUG
	itoa(status, vBuff, 16);
	USART_SendString(vBuff);

	USART_Transmit('a');
#endif
	TWIWrite((DS1307_I2C_ADDRESS << 1) | TW_WRITE);

	_delay_ms(1);
	status = TWIGetStatus();

#ifdef SERIAL_DEBUG
	itoa(status, vBuff, 16);
	USART_SendString(vBuff);

	USART_Transmit('b');
#endif

	//got to register 0x00
	TWIWrite(0x00);

	_delay_ms(1);
	status = TWIGetStatus();

#ifdef SERIAL_DEBUG
	itoa(status, vBuff, 16);
	USART_SendString(vBuff);

	USART_Transmit('c');
#endif

	//init ALL to 1 (7 values)
	for (uint8_t i = 0; i < 7; i++){
		TWIWrite(decToBcd(1));
	}

	TWIWrite(pDateTime->second);
	TWIWrite(pDateTime->minute);
	TWIWrite(pDateTime->hour);
	TWIWrite(pDateTime->dayOfWeek);
	TWIWrite(pDateTime->dayOfMonth);
	TWIWrite(pDateTime->month);
	TWIWrite(pDateTime->year);

#ifdef SERIAL_DEBUG
	USART_Transmit('d');
#endif


	//Finished the setup
	TWIStop();
}

//=====================================================================================

void setupDS1307(){
	uint8_t status = 0;
#ifdef SERIAL_DEBUG
	char vBuff[15];

	USART_SendString("\nInit:");
#endif

	TWIInit();

	TWIStart();

	_delay_ms(1);
	status = TWIGetStatus();

#ifdef SERIAL_DEBUG
	itoa(status, vBuff, 16);
	USART_SendString(vBuff);

	USART_Transmit('a');
#endif
	TWIWrite((DS1307_I2C_ADDRESS << 1) | TW_WRITE);

	_delay_ms(1);
	status = TWIGetStatus();

#ifdef SERIAL_DEBUG
	itoa(status, vBuff, 16);
	USART_SendString(vBuff);

	USART_Transmit('b');
#endif

	TWIWrite(0x0E);

	_delay_ms(1);
	status = TWIGetStatus();

#ifdef SERIAL_DEBUG
	itoa(status, vBuff, 16);
	USART_SendString(vBuff);

	USART_Transmit('c');
#endif

	//TWIWrite(0x1C);
	TWIWrite(0x1C);


#ifdef SERIAL_DEBUG
	USART_Transmit('d');
#endif

	TWIWrite(0x80);

#ifdef SERIAL_DEBUG
	USART_Transmit('e');
#endif

	//Finished the setup
	TWIStop();
}


/*********************************************************************/
/* Returns a string representation of the date                       */
/*********************************************************************/
void dateToString (char *buffer, Date *d){
	//sprintf(buffer, "%00d:%00d:%00d", d->hour, d->minute, d->second);
	sprintf(buffer, "%00d/%00d/%00d %00d:%00d:%00d", d->year, d->month, d->dayOfMonth, d->hour, d->minute, d->second);
}
#endif __RTC1307_H__
