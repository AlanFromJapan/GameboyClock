
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


/************************************************************************/
/* READS time															*/
/************************************************************************/
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

	_delay_ms(10); //in case

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
	status = TWIReadACK();
	if (TWIGetStatus() != TW_MR_DATA_ACK){
#ifdef SERIAL_DEBUG
USART_Transmit('J');
#endif
		//something wrong
		pTimeDate->hour = TWIGetStatus();
		pTimeDate->minute = pTimeDate->hour;
		pTimeDate->second = pTimeDate->minute;
		return;
	}

	//bit 7 is the CH flag so ignore it
	pTimeDate->second = bcdToDec(status & 0x7f);
	status = TWIReadACK();
	pTimeDate->minute = bcdToDec(status);
	status = TWIReadACK();
	//bit 7~6 are for control
	pTimeDate->hour = bcdToDec(status & 0x3f);
	status = TWIReadACK();
	pTimeDate->dayOfWeek = bcdToDec(status);
	status = TWIReadACK();
	pTimeDate->dayOfMonth = bcdToDec(status);
	status = TWIReadACK();
	pTimeDate->month = bcdToDec(status);

	//for the life of me I don't know why the last one has to be NACK and not ACK.
	//if set as ACK, the code returns fine BUT you can't re-enter the TWI bus and TWIStart()
	//waits forever... Future me, good luck, but it works now. (sorry)
	status = TWIReadNACK();
	pTimeDate->year = bcdToDec(status);

	//Finished the read phase
	TWIStop();

}

/************************************************************************/
/* SETS time															*/
/************************************************************************/
void setTimeDate1307(Date* pDateTime){
	uint8_t status;
#ifdef SERIAL_DEBUG
	char vBuff[15];

	USART_SendString("\nSet:");
#endif

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


	//bit 7 of register 0x00 (seconds) is the CH (Clock Halt) -> if set to 1 then the clock stops so force to zero
	TWIWrite(decToBcd(pDateTime->second) & 0x7f);
	TWIWrite(decToBcd(pDateTime->minute));
	//bit 7 is ignored, bit 6 is 24h format: low means 24h mode, high means 12h AM/PM.
	TWIWrite(decToBcd(pDateTime->hour) & 0x3f);
	TWIWrite(decToBcd(pDateTime->dayOfWeek));
	TWIWrite(decToBcd(pDateTime->dayOfMonth));
	TWIWrite(decToBcd(pDateTime->month));
	TWIWrite(decToBcd(pDateTime->year));

#ifdef SERIAL_DEBUG
	USART_Transmit('d');
#endif


	//Finished the setup
	TWIStop();
}

//=====================================================================================

void setupDS1307(){
	uint8_t status;
#ifdef SERIAL_DEBUG
	char vBuff[15];

	USART_SendString("\nInit:");
#endif

	//DS1307 supports 100KHz clock max only
	// Change this value if the CPU is not at 16MHz!
	TWIInitPreset(TWI_CLOCK_PRESET_CPU16MHZ_TWI100KHZ);

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

	//got to the control register at 0x07
	TWIWrite(0x07);

	_delay_ms(1);
	status = TWIGetStatus();

#ifdef SERIAL_DEBUG
	itoa(status, vBuff, 16);
	USART_SendString(vBuff);

	USART_Transmit('c');
#endif

	//no square wave output
	TWIWrite(0x00);


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
	sprintf(buffer, "%02d/%02d/%02d %02d:%02d:%02d", d->year, d->month, d->dayOfMonth, d->hour, d->minute, d->second);
}
#endif // __RTC1307_H__
