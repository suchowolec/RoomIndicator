/*
 * led.h
 *
 *  Created on: 16-08-2015
 *      Author: Czarek
 */

#ifndef LED_H_
#define LED_H_

#include <avr/io.h>

#define PORT_LED    	PORTD
#define PORT_BUZZ   	PORTD
#define LED_BLUE 		PD1
#define BUZZ 			PD4

#define LED_BLUE_ON 	PORT_LED |= _BV(LED_BLUE)
#define LED_BLUE_OFF	PORT_LED &= ~(_BV(LED_BLUE))
#define BUZZ_ON 		PORT_BUZZ |= _BV(BUZZ);
#define BUZZ_OFF		PORT_BUZZ &= ~(_BV(BUZZ));

extern void blink_blue_led(uint8_t blinks);
extern void beep(uint8_t beeps);

#endif /* LED_H_ */
