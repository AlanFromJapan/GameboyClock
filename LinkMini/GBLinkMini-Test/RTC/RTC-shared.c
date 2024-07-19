/*
 * RTC-shared.c
 *
 *  Created on: Apr 1, 2024
 *      Author: alan
 */



#ifndef RTC_SHARED_C_
#define RTC_SHARED_C_

#include "RTC-shared.h"


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



#endif // RTC_SHARED_C_
