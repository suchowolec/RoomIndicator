/*
 * led.c
 *
 *  Created on: 16-08-2015
 *      Author: Czarek
 */

#include "led.h"
#include <util/delay.h>

void blink_red_led(uint8_t blinks) {
	for (uint8_t i = 0; i < blinks; i++){
		LED_RED_ON;
		_delay_ms(5);
		LED_RED_OFF;
		if (i + 1 < blinks){
			_delay_ms(100);
		}
	}
}

void blink_green_led(uint8_t blinks) {
	for (uint8_t i = 0; i < blinks; i++){
		LED_GREEN_ON;
		_delay_ms(5);
		LED_GREEN_OFF;
		if (i + 1 < blinks){
			_delay_ms(100);
		}
	}
}

void beep(uint8_t beeps) {
	for (uint8_t i = 0; i < beeps; i++){
		BUZZ_ON;
		_delay_ms(20);
		BUZZ_OFF;
		if (i + 1 < beeps){
			_delay_ms(100);
		}
	}
}

