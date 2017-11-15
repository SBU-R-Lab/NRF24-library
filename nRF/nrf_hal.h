#ifndef _NRF_HAL_H
#define _NRF_HAL_H

#define HIGH  1
#define LOW 0

void init_system(void);
void init_irq(void);
void init_gpio(void);
void init_ssp(void);

void setCE(unsigned char state);
void setCSN(unsigned char state);

unsigned char write_register(unsigned char command, unsigned char* data, unsigned int length);
unsigned char read_register(unsigned char command, unsigned char* data, unsigned int length);

#endif