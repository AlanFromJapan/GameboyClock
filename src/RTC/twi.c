/*
  twi.c - TWI/I2C library 
  
  http://www.embedds.com/programming-avr-i2c-interface/
  */

#include <math.h>
#include <stdlib.h>
#include <inttypes.h>
//System includes
#include <avr/io.h>
#include <util/delay.h>


#include "twi.h"


/* ------------------------------------------------------------------------- */
/* Init 																	 */
/* ------------------------------------------------------------------------- */
void _TWIInit(const uint8_t prescaler, const uint8_t bitrate)
{
	TWSR = prescaler;
	TWBR = bitrate;

	//enable TWI
	TWCR = (1<<TWEN);
}
void TWIInitPreset(const uint8_t preset)
{
	_TWIInit(0x00, preset);
}
void TWIInit(void)
{
	//With a 16MHz CPU, sets SCL to 100kHz - normal speed
	_TWIInit(0x00, 0x48);
}

/* ------------------------------------------------------------------------- */
void TWIStart(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
}
//send stop signal
void TWIStop(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void TWIWrite(uint8_t u8data)
{
	TWDR = u8data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
}

uint8_t TWIReadACK(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while ((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}
//read byte with NACK
uint8_t TWIReadNACK(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}

uint8_t TWIGetStatus(void)
{
	uint8_t status;
	//mask status
	status = TWSR & 0xF8;
	return status;
}

