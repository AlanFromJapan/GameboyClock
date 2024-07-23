/*
 * RTC-shared.h
 *
 *  Created on: Apr 1, 2024
 *      Author: alan
 */

#ifndef RTC_SHARED_H_
#define RTC_SHARED_H_

#include <avr/io.h>

struct Date {
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
  uint8_t dayOfWeek;
  uint8_t dayOfMonth;
  uint8_t month;
  uint8_t year;
}
;


// Convert normal decimal numbers to binary coded decimal
uint8_t decToBcd(uint8_t val);

// Convert binary coded decimal to normal decimal numbers
uint8_t bcdToDec(uint8_t val);


#endif /* RTC_SHARED_H_ */
