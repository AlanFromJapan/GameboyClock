
#ifndef __RTC1307_H__
#define __RTC1307_H__

#include "RTC-shared.h"

/************************************************************************/
/* EVERY RTC has to define the below macros                             */
/************************************************************************/
#define rtcRead(d)		readTime1307(d)
#define rtcWrite(d)		setTimeDate1307(d)
#define rtcSetup()		setupDS1307();


#define DS1307_I2C_ADDRESS 0x68  // This is the I2C address

/************************************************************************/
/* READS time															*/
/************************************************************************/
void readTime1307(struct Date* pTimeDate);

/************************************************************************/
/* SETS time															*/
/************************************************************************/
void setTimeDate1307(struct Date* pDateTime);

/************************************************************************/
/* SETUP the DS1307 and bus (call ONCE at each startup)                 */
/************************************************************************/
void setupDS1307();



#endif // __RTC1307_H__
