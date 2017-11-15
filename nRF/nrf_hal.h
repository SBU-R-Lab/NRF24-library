#include "lpc17xx_ssp.h"

#define HIGH  1
#define LOW 0


void init_system(void);
void init_irq(void);
void init_gpio(void);
void init_ssp(void);

void setCE(unsigned char state);
void setCSN(unsigned char state);