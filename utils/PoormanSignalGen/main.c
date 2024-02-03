/*
 * main.c - PoormanSignalGen(erator)
 *   To test my soft SPI I need some sample of signals, nothing fancy.
 *   I don't have a signal generator so this will do. Reusing my testboard for USB development, so it's a mega8a at 12Mhz.
 *
 *  Created on: Feb 3, 2024
 *      Author: alan
 */




//for delay.h >> defined in the compilation symbols
//#define F_CPU 12000000L

#include <stdlib.h>

//System includes
#include <avr/io.h>
#include <util/delay.h>
#include <avr/cpufunc.h>


#define DELAYMS 		120

/************************************************************************/
/* Setup                                                                */
/************************************************************************/
void mainSetup() {
	//all out
	DDRB = 0xff;
	DDRD = 0xff;
}

/************************************************************************/
/* Main                                                                 */
/************************************************************************/
int main(void) {
	//Setup!
	mainSetup();

	uint8_t b = 0xCA; //b11001010
	uint8_t mask = 0;

    while(1) {
    	//PORTB is led, PORTD is data
    	//light up
    	PORTB |= 0x01;

    	//put data on "MOSI" D1 (LSB first)
    	PORTD &= ~0x02;
    	PORTD |= ((b >> mask) & 0x01) << 1;

    	//Just in case, adding some time delay to be sure signal stabilizes (most likely useless)
    	_NOP();
    	_NOP();
    	_NOP();
    	_NOP();
    	_NOP();

    	//Clock UP D0
    	PORTD |= 0x01;

		_delay_ms(DELAYMS);
		//-------------------------------------------------------

		//led off
    	PORTB &= ~0x01;

    	//Clock DOWN D0
    	PORTD &= ~0x01;

    	//next bit
    	mask ++;
    	if (mask >= 8){
    		mask = 0;
    	}

		_delay_ms(DELAYMS);

    }
}
