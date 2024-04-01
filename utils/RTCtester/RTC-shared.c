/*
 * RTC-shared.c
 *
 *  Created on: Apr 1, 2024
 *      Author: alan
 */



#ifndef RTC_SHARED_C_
#define RTC_SHARED_C_

#include "RTC-shared.h"

/*********************************************************************/
/* Returns a string representation of the date                       */
/*********************************************************************/
void dateToString (char *buffer, Date *d);

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


/*********************************************************************/
/* Returns a string representation of the date                       */
/*********************************************************************/
void dateToString (char *buffer, Date *d){
	//sprintf(buffer, "%00d:%00d:%00d", d->hour, d->minute, d->second);
	sprintf(buffer, "%02d/%02d/%02d %02d:%02d:%02d", d->year, d->month, d->dayOfMonth, d->hour, d->minute, d->second);
}


#endif // RTC_SHARED_C_
