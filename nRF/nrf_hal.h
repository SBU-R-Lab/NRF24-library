#ifndef _NRF_HAL_H
#define _NRF_HAL_H

#define HIGH  1
#define LOW 0

void nrf_hal_init_system(void);
void nrf_hal_init_gpio(void);
void nrf_hal_init_ssp(void);

void nrf_hal_set_ce(unsigned char state);
void nrf_hal_set_csn(unsigned char state);

unsigned char nrf_hal_write_register(unsigned char command, unsigned char* data, unsigned int length);
unsigned char nrf_hal_read_register(unsigned char command, unsigned char* data, unsigned int length);

#endif