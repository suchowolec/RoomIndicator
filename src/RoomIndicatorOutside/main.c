/*
 * main.c
 *
 *  Created on: 23-06-2015
 *      Author: Czarek
 */

#include "led.h"
#include "mirf.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BTN				PD3
#define IRQ				PD2

volatile uint8_t flag = 0;

void system_sleep(){
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sleep_mode();
}

uint8_t temp;
uint8_t i = 0;
uint8_t data_array[1];
uint8_t tx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
uint8_t rx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};

void sendMessage(uint8_t value){
	mirf_config();
    data_array[0] = value;
    mirf_send(data_array, 1);
}


void setup(){
	// setup pin configuration
	// 1 - output 0 - input

	DDRB = 0xEF;        // PB4 - MISO as input
	PORTB = 0x04;		//

	DDRC = 0xFF;		// Set all pins as output
	PORTC = 0x00;		// Set low for all pins

	DDRD |= _BV(LED_RED) | _BV(LED_GREEN) | _BV(BUZZ) |
			// not used as outputs
			_BV(PD5) | _BV(PD6) | _BV(PD7);

	DDRD &= ~(_BV(BTN) | _BV(IRQ));

	PORTD &= ~(_BV(LED_RED) | _BV(LED_GREEN) |_BV(BUZZ) |
			// not used set low
			_BV(PD5) | _BV(PD6) | _BV(PD7));

	PORTD |= _BV(BTN) | _BV(IRQ);

	EICRA |= _BV(ISC11); // Falling edge
	EIMSK |= _BV(INT1);   // Enable INT1

	mirf_init();

	_delay_ms(50);

	sei();

	mirf_set_RADDR(rx_address);
	mirf_set_TADDR(tx_address);

	mirf_config();
}

// button
ISR(INT1_vect){
	flag = 1;
}

ISR(BADISR_vect) {}

int main(void) {
	setup();

	while(1){
		if (flag == 1){
			sendMessage(0xFF);
			beep(1);
			flag = 0;
		}

		if(mirf_data_ready())
		{
			mirf_get_data(data_array);

			if (data_array[0] == 0xFF){
				LED_GREEN_OFF;
				LED_RED_ON;
			}
			else {
				LED_GREEN_ON;
				LED_RED_OFF;
			}
		}
	}
	return 0;
}
