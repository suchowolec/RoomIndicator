/*
 * mirf.c
 *
 *  Created on: 16-08-2015
 *      Author: Czarek
 */

#include "mirf.h"
#include "RF24L01.h"
#include "spi.h"
#include "led.h"
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

// Defines for setting the MiRF registers for transmitting or receiving mode
#define TX_POWERUP mirf_config_register(CONFIG, mirf_CONFIG | ( (1<<PWR_UP) | (0<<PRIM_RX) ) )
#define RX_POWERUP mirf_config_register(CONFIG, mirf_CONFIG | ( (1<<PWR_UP) | (1<<PRIM_RX) ) )
#define POWERDOWN  mirf_config_register(CONFIG, mirf_CONFIG | ( (0<<PWR_UP) | (1<<PRIM_RX) ) )

// Flag which denotes transmitting mode
volatile uint8_t PTX;
volatile uint8_t data_t[1];
/*******************************************************************************
 * Initializes pins ans interrupt to communicate with the MiRF module          *
 * Should be called in the early initializing phase at startup.                *
 ******************************************************************************/
void mirf_init()
{
    // Define CSN and CE as Output and set them to default
    DDRB |= ((1<<CSN)|(1<<CE));

    mirf_CE_lo;
    mirf_CSN_hi;

    // Initialize external interrupt on port PD2
	//EICRA |= _BV(ISC01);
	//EIMSK |= _BV(INT0);

    spi_init();

	// Set RF channel
	mirf_config_register(RF_CH,mirf_CH);

    // Set length of incoming payload
    mirf_config_register(RX_PW_P0, mirf_PAYLOAD);
}

/*******************************************************************************
 * Sets the important registers in the MiRF module and powers the module in    *
 * receiving mode                                                              *
 ******************************************************************************/
void mirf_config(){

    // Start receiver
    PTX = 0;        // Start in receiving mode

    //RX_POWERUP;     // Power up in receiving mode
    //mirf_CE_lo;     // Listening for pakets
    POWERDOWN;
    mirf_CE_hi;
}

/*******************************************************************************
 * Sets the receiving address                                                  *
 ******************************************************************************/
void mirf_set_RADDR(uint8_t * adr)
{
    mirf_CE_lo;
    mirf_write_register(RX_ADDR_P0,adr,5);
    mirf_CE_hi;
}

/*******************************************************************************
 * Sets the transmitting address                                               *
 ******************************************************************************/
void mirf_set_TADDR(uint8_t * adr)
{
    mirf_write_register(TX_ADDR, adr,5);
}

/*******************************************************************************
 * Checks if data is available for reading                                     *
 ******************************************************************************/
extern uint8_t mirf_data_ready()
{
    if (PTX) return 0;
    uint8_t status;
    // Read MiRF status
    mirf_CSN_lo;						// Pull down chip select
    status = spi_fast_shift(NOP);		// Read status register
    mirf_CSN_hi;						// Pull up chip select
    return status & (1<<RX_DR);
}

/*******************************************************************************
 * Reads mirf_PAYLOAD bytes into data array                                    *
 ******************************************************************************/
extern void mirf_get_data(uint8_t * data)
{
    mirf_CSN_lo;								// Pull down chip select
    spi_fast_shift(R_RX_PAYLOAD);				// Send cmd to read rx payload
    spi_transfer_sync(data,data,mirf_PAYLOAD);	// Read payload
    mirf_CSN_hi;								// Pull up chip select
    mirf_config_register(STATUS,(1<<RX_DR));	// Reset status register
}

/*******************************************************************************
 * Clocks only one byte into the given MiRF register                           *
 ******************************************************************************/
void mirf_config_register(uint8_t reg, uint8_t value)
{
    mirf_CSN_lo;
    spi_fast_shift(W_REGISTER | (REGISTER_MASK & reg));
    spi_fast_shift(value);
    mirf_CSN_hi;
}

/*******************************************************************************
 * Reads an array of bytes from the given start position in the MiRF registers *
 ******************************************************************************/
void mirf_read_register(uint8_t reg, uint8_t * value, uint8_t len)
{
    mirf_CSN_lo;
    spi_fast_shift(R_REGISTER | (REGISTER_MASK & reg));
    spi_transfer_sync(value,value,len);
    mirf_CSN_hi;
}

/*******************************************************************************
 * Writes an array of bytes into inte the MiRF registers.                      *
 ******************************************************************************/
void mirf_write_register(uint8_t reg, uint8_t * value, uint8_t len)
{
    mirf_CSN_lo;
    spi_fast_shift(W_REGISTER | (REGISTER_MASK & reg));
    spi_transmit_sync(value,len);
    mirf_CSN_hi;
}

uint8_t mirf_is_sending(){
	uint8_t status = mirf_get_status();
	if((status & ((1 << TX_DS)  | (1 << MAX_RT)))){
		return 0; /* false */
	}
	return 1;
}

uint8_t mirf_get_status(){

	uint8_t rv;
	mirf_CSN_lo;
	rv = spi_fast_shift(W_REGISTER | (REGISTER_MASK & NOP));
	mirf_CSN_hi;

	return rv;
}

/*******************************************************************************
 * Sends a data package to the default address. Be sure to send the correct    *
 * amount of bytes as configured as payload on the receiver                    *
 ******************************************************************************/
void mirf_send(uint8_t * value, uint8_t len)
{
    while (PTX) {}                  // Wait until last paket is send

    mirf_CE_lo;

    PTX = 1;                        // Set to transmitter mode
    TX_POWERUP;                     // Power up

    mirf_CSN_lo;                    // Pull down chip select
    spi_fast_shift( FLUSH_TX );     // Write cmd to flush tx fifo
    mirf_CSN_hi;                    // Pull up chip select

    mirf_CSN_lo;                    // Pull down chip select
    spi_fast_shift( W_TX_PAYLOAD ); // Write cmd to write payload
    spi_transmit_sync(value,len);   // Write payload
    mirf_CSN_hi;                    // Pull up chip select

    mirf_CE_hi;                     // Start transmission

    while(mirf_is_sending());

	mirf_CE_lo;                     // Deactivate transreceiver
	RX_POWERUP;                     // Power up in receiving mode

	mirf_CE_hi;                     // Listening for pakets
	PTX = 0;                        // Set to receiving mode

	// Reset status register for further interaction
	mirf_config_register(STATUS,(1<<TX_DS)|(1<<MAX_RT));

	//mirf_CE_lo;

	POWERDOWN;
}

/*******************************************************************************
 * IRQ Interrupt handler                                                       *
 ******************************************************************************/
ISR(INT0_vect){

}
