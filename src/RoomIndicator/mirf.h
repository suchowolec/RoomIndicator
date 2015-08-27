/*
 * mirf.h
 *
 *  Created on: 16-08-2015
 *      Author: Czarek
 */

#ifndef MIRF_H_
#define MIRF_H_

#include "RF24L01.h"
#include <avr/io.h>

#define mirf_CH         2
#define mirf_PAYLOAD    1
#define mirf_CONFIG     ( (1<<MASK_MAX_RT) | (0<<MASK_RX_DR) | (1<<MASK_TX_DS) | (1<<EN_CRC) | (0<<CRCO) )

#define mirf_MaxPower  (1<<RF_PWR_HIGH) | (1<<RF_PWR_LOW)

#define mirf_250KBIT    ( (1<<RF_DR_LOW)   | (0<<RF_DR_HIGH) | mirf_MaxPower)
#define mirf_1MBIT      ( (0<<RF_DR_LOW)   | (0<<RF_DR_HIGH) | mirf_MaxPower)
#define mirf_2MBIT      ( (0<<RF_DR_LOW)   | (1<<RF_DR_HIGH) | mirf_MaxPower)

#define CE  PC2
#define CSN PB2

#define mirf_CSN_hi     PORTB |=  (1<<CSN);
#define mirf_CSN_lo     PORTB &= ~(1<<CSN);
#define mirf_CE_hi      PORTC |=  (1<<CE);
#define mirf_CE_lo      PORTC &= ~(1<<CE);

extern void mirf_init();
extern void mirf_config();
extern void mirf_send(uint8_t * value, uint8_t len);
extern void mirf_set_RADDR(uint8_t * adr);
extern void mirf_set_TADDR(uint8_t * adr);
extern uint8_t mirf_data_ready();
extern void mirf_get_data(uint8_t * data);
extern uint8_t mirf_is_sending();
extern uint8_t mirf_get_status();
extern void mirf_config_register(uint8_t reg, uint8_t value);
extern void mirf_read_register(uint8_t reg, uint8_t * value, uint8_t len);
extern void mirf_write_register(uint8_t reg, uint8_t * value, uint8_t len);

#endif /* MIRF_H_ */
