/*
  twi.h - TWI/I2C library 
  
  http://www.embedds.com/programming-avr-i2c-interface/
  */

#ifndef twi_h
#define twi_h

//for the constants
#include "util/twi.h"

//wellknown preset settings for a given MCU the TWI bus clock speed
#define TWI_CLOCK_PRESET_CPU16MHZ_TWI100KHZ		0x48
#define TWI_CLOCK_PRESET_CPU16MHZ_TWI400KHZ		0x0C

void TWIInit();
void TWIInitPreset(const uint8_t preset);

void TWIStart(void);
//send stop signal
void TWIStop(void);

void TWIWrite(uint8_t u8data);

uint8_t TWIReadACK(void);
//read byte with NACK
uint8_t TWIReadNACK(void);

uint8_t TWIGetStatus(void);

#endif

