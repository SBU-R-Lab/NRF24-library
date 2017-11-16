#include "lpc17xx_ssp.h"
#include "nrf_hal.h"

void nrf_hal_init_system()
{

}

void nrf_hal_init_gpio()
{

  LPC_GPIO0->FIODIR |= (1<<17);
  nrf_hal_set_ce(LOW);

  LPC_GPIO0->FIODIR |= (1<<6);
  nrf_hal_set_csn(HIGH);
}

void nrf_hal_init_ssp()
{
  LPC_PINCON->PINSEL0 |= 0x2<<14 ; //SCK1
  LPC_PINCON->PINSEL0 |= 0x2<<16 ; //MISO1
  LPC_PINCON->PINSEL0 |= 0x2<<18 ; //MOSI1
    

  SSP_CFG_Type cfg ;
 
  
  SSP_ConfigStructInit(&cfg) ;
  SSP_Init(LPC_SSP1,&cfg) ;
  SSP_Cmd(LPC_SSP1,ENABLE) ;
}

void nrf_hal_set_ce(unsigned char state)
{
  if(state)
    LPC_GPIO0->FIOSET |= (1<<17);
  else
    LPC_GPIO0->FIOCLR |= (1<<17);
}

void nrf_hal_set_csn(unsigned char state)
{
  if(state)
    LPC_GPIO0->FIOSET |= (1<<6);
  else
    LPC_GPIO0->FIOCLR |= (1<<6);
}


unsigned char nrf_hal_write_register(unsigned char command, unsigned char* data, unsigned int length)
{ 
  unsigned char status = 0;
  nrf_hal_set_csn(LOW) ;
  
  SSP_SendData(LPC_SSP1,command) ;
  status = SSP_ReceiveData(LPC_SSP1) ;
  for (int i = 0 ; i < length ;i++){
    SSP_SendData(LPC_SSP1,data[i]) ;
  }
  
  while (SSP_GetStatus(LPC_SSP1,SSP_STAT_BUSY) );

  nrf_hal_set_csn(HIGH) ;
  
  return status;
}

unsigned char nrf_hal_read_register(unsigned char command, unsigned char* data, unsigned int length)
{
  unsigned char status = 0;
  nrf_hal_set_csn(LOW) ;
  
  SSP_SendData(LPC_SSP1,command) ;
  status = SSP_ReceiveData(LPC_SSP1) ;
  for (int i = length -1 ;i >= 0 ;i--){
      SSP_SendData(LPC_SSP1,0xFF) ;
      while (SSP_GetStatus(LPC_SSP1,SSP_STAT_BUSY) );
        data[i] = (uint8_t)SSP_ReceiveData(LPC_SSP1) ;
  }

  while (SSP_GetStatus(LPC_SSP1,SSP_STAT_BUSY) );

  nrf_hal_set_csn(HIGH) ;

  return status;
}
