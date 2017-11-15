#include "lpc17xx_ssp.h"
#include "nrf_hal.h"

void init_system()
{

}

void init_irq()
{
  LPC_GPIO0->FIODIR &= !(1<<5) ;
  LPC_GPIOINT->IO0IntEnF |=(1<<5) ;
  NVIC_EnableIRQ(EINT3_IRQn);
}


void init_gpio()
{

  LPC_GPIO0->FIODIR |= (1<<17);
  setCE(LOW);

  LPC_GPIO0->FIODIR |= (1<<6);
  setCSN(HIGH);
}

void init_ssp()
{
  LPC_PINCON->PINSEL0 |= 0x2<<14 ; //SCK1
  LPC_PINCON->PINSEL0 |= 0x2<<16 ; //MISO1
  LPC_PINCON->PINSEL0 |= 0x2<<18 ; //MOSI1
    

  SSP_CFG_Type cfg ;
 
  
  SSP_ConfigStructInit(&cfg) ;
  SSP_Init(LPC_SSP1,&cfg) ;
  SSP_Cmd(LPC_SSP1,ENABLE) ;
}

void setCE(unsigned char state)
{
  if(state)
    LPC_GPIO0->FIOSET |= (1<<17);
  else
    LPC_GPIO0->FIOCLR |= (1<<17);
}

void setCSN(unsigned char state)
{
  if(state)
    LPC_GPIO0->FIOSET |= (1<<6);
  else
    LPC_GPIO0->FIOCLR |= (1<<6);
}
