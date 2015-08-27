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
#define LED_RED   		PD0
#define LED_GREEN 		PD1
#define BUZZ 			PD4
#define BTN				PD3
#define IRQ				PD2

#define LED_RED_ON 		PORT_LED |= _BV(LED_RED)
#define LED_RED_OFF 	PORT_LED &= ~(_BV(LED_RED))
#define LED_GREEN_ON 	PORT_LED |= _BV(LED_GREEN)
#define LED_GREEN_OFF	PORT_LED &= ~(_BV(LED_GREEN))
#define BUZZ_ON 		PORT_BUZZ |= _BV(BUZZ);
#define BUZZ_OFF		PORT_BUZZ &= ~(_BV(BUZZ));

extern void blink_red_led(uint8_t blinks);
extern void blink_green_led(uint8_t blinks);
extern void beep(uint8_t beeps);

#endif /* LED_H_ */
