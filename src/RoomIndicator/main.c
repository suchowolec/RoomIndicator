/*
 * main.c
 *
 *  Created on: 23-06-2015
 *      Author: Czarek
 */

#include "led.h"
#include "mirf.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#define BTN				PD3
#define IRQ				PD2

volatile uint8_t flag;
volatile uint8_t eflag;

void system_sleep(){
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sleep_mode();
}

ISR(INT1_vect){
	sleep_disable();
	cli();
	eflag = eflag == 0 ? 1 : 0;
	if (eflag){
		EICRA |= _BV(ISC10);
	}
	else {
		EICRA &= ~(_BV(ISC10));
	}
	sei();
}

ISR(BADISR_vect) {}

uint8_t temp;
uint8_t i = 0;

uint8_t data_array[1];

uint8_t tx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
uint8_t rx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};


void setup(){
	// setup pin configuration
	// 1 - output 0 - input

	DDRB = 0xEF;        // PB4 - MISO as input
	PORTB = 0x04;		//

	DDRC = 0xFF;		// Set all pins as output
	PORTC = 0x00;		// Set low for all pins

	DDRD |=  _BV(LED_BLUE) | _BV(BUZZ) |
			// not used as outputs
			_BV(PD0) | _BV(PD5) | _BV(PD6) | _BV(PD7);

	DDRD &= ~(_BV(BTN) | _BV(IRQ));

	PORTD &= ~( _BV(LED_BLUE) |_BV(BUZZ) |
			// not used set low
			_BV(PD0) | _BV(PD5) | _BV(PD6) | _BV(PD7));

	PORTD |= _BV(BTN) | _BV(IRQ);

	EICRA |= _BV(ISC11);
	EIMSK |= _BV(INT1);	// Enable INT1

	mirf_init();

	mirf_set_RADDR(rx_address);
	mirf_set_TADDR(tx_address);

	mirf_config();

	_delay_ms(50);

	sei();
}

void sendMessage(uint8_t value){
    data_array[0] = value;

    mirf_send(data_array, 1);

    blink_blue_led(1);
}

int main(void) {
	setup();

	while(1){
		if(eflag)
		{
			sendMessage(0xFF);
		}
		else
		{
			sendMessage(0x00);
		}
		system_sleep();
	}
	return 0;
}
