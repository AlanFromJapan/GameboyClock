/*
 * main.c
 *
 *  Created on: Jun 12, 2024
 *      Author: alan
 */

#include "avr/io.h"
#include "avr/delay.h"

void main (){
	DDRB = 0x05;

	while (1){
		PORTB = 0x01;
		_delay_ms(500);

		PORTB = 0x04;
		_delay_ms(500);
	}

}
