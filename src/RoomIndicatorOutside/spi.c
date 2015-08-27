/*
 * spi.c
 *
 *  Created on: 16-08-2015
 *      Author: Czarek
 *
 *  SPCR - SPI Control Register
 *  -------------------------------------------------------------
 *  Bit 7 	Bit 6 	Bit 5 	Bit 4 	Bit 3 	Bit 2 	Bit 1 	Bit 0
 *  SPIE	SPE		DORD	MSTR	CPOL	CPHA	SPR1	SPR0
 *  -------------------------------------------------------------
 *
 *  SPIE - SPI Interrupt Enable
 *         bit: Set SPIE to one if you want the SPI interrupt to be executed
 *         when a serial transfer is completed.
 *  SPE  - SPI Enable
 *  	   bit: If you want to use the SPI, you must set this bit.
 *  DORD - Data Order
 *  	   bit: You can choose in which order the data will be transmitted.
 *  	   Set DORD to one to send the least significant bit (LSB) first.
 *  	   Set DORD to zero to send the most significant bit (MSB) first.
 *  MSTR - Master/Slave Select
 *  	   bit: Set MSTR to configure the AVR as a Master SPI device.
 *  	   Clear MSTR to configure it as a Slave.
 *  CPOL - Clock Polarity and CPHA Clock Phase
 *         bits: As stated previously, Master and Slave must agree on how to
 *         interpret the clock signal. The first thing to do is to configure
 *         which logic level the clock will be in when the SPI is idle.
 *         If CPOL is set to one, SCK is high when idle, and if CPOL
 *         is set to zero, SCK is low when idle. The second thing is to
 *         configure during which clock transition the data will be sampled.
 *         Set CPHA to sample the data on the trailing (last) edge,
 *         and clear CPHA to sample the data in the leading (first) edge.
 *  ----------------------------------------------------------------------------
 *  SPSR - SPI Status Register
 *  -------------------------------------------------------------
 *  Bit 7 	Bit 6 	Bit 5 	Bit 4 	Bit 3 	Bit 2 	Bit 1 	Bit 0
 *  SPIF	WCOL	-		-		-		-		-		SPI2X
 *  -------------------------------------------------------------
 *  SPIF - SPI Interrupt Flag
 *  	   bit: This is a read only bit. It is set by hardware when a serial
 *  	   transfer is complete. SPIF is cleared by hardware when the
 *  	   SPI interrupt handling vector is executed, or when the SPIF bit and
 *  	   the SPDR register are read.
 *  WCOL - Write Colision Flag
 *  	   bit: This is a read only bit. The WCOL bit is set if the
 *  	   SPDR register is written to during a data transfer.
 *  	   The WCOL bit (and the SPIF bit) are cleared by first reading
 *  	   the SPI Status Register with WCOL set, and then accessing
 *  	   the SPI Data Register.
 *  SPI2x  Double SPI Speed
 *  	   bit: This feature is not implemented in all AVRs.
 *  	   When this bit is set to one, the SPI speed will be doubled when
 *  	   the SPI is in Master mode.
 */

#include "spi.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define PORT_SPI    PORTB
#define DDR_SPI     DDRB
#define DD_MISO     DDB4	// default 1
#define DD_MOSI     DDB3	// default 1
#define DD_SS       DDB2	// default 1
#define DD_SCK      DDB5	// default 0

void spi_init()
// Initialize pins for spi communication
{
    DDR_SPI &= ~((1<<DD_MOSI)|(1<<DD_MISO)|(1<<DD_SS)|(1<<DD_SCK));
    // Define the following pins as output
    DDR_SPI |= ((1<<DD_MOSI)|(1<<DD_SS)|(1<<DD_SCK));

    SPCR = ((0<<SPIE)|	// SPI Interupt Enable
    		(1<<SPE) |  // SPI Enable
            (0<<DORD)|  // Data Order (0:MSB first / 1:LSB first)
            (1<<MSTR)|  // Master/Slave select
            (0<<CPOL)|  // Clock Polarity (0:SCK low / 1:SCK hi when idle)
            (0<<CPHA)|	// Clock Phase (0:leading / 1:trailing edge sampling)
            (0<<SPR1)|  // SPI Clock Rate
            (0<<SPR0)); // SPI Clock Rate	Clock / 4

    SPSR = (1<<SPI2X);  // Double Clock Rate
}

void spi_transfer_sync (uint8_t * dataout, uint8_t * datain, uint8_t len)
// Shift full array through target device
{
	uint8_t i;
	for (i = 0; i < len; i++) {
		 SPDR = dataout[i];
		 while((SPSR & (1<<SPIF))==0);
		 datain[i] = SPDR;
	}
}

void spi_transmit_sync (uint8_t * dataout, uint8_t len)
// Shift full array to target device without receiving any byte
{
	uint8_t i;
	for (i = 0; i < len; i++) {
		 SPDR = dataout[i];
		 while((SPSR & (1<<SPIF))==0);
	}
}

uint8_t spi_fast_shift (uint8_t data)
// Clocks only one byte to target device and returns the received one
{
    SPDR = data;
    while((SPSR & (1<<SPIF))==0);
    return SPDR;
}
