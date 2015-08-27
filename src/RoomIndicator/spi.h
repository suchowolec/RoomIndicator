/*
 * spi.h
 *
 *  Created on: 16-08-2015
 *      Author: Czarek
 */

#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <util/delay.h>

extern void spi_init();
extern void spi_transfer_sync (uint8_t * dataout, uint8_t * datain, uint8_t len);
extern void spi_transmit_sync (uint8_t * dataout, uint8_t len);
extern uint8_t spi_fast_shift (uint8_t data);

#endif /* SPI_H_ */
