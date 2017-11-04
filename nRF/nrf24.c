#include "nrf24.h"
#include "lpc17xx_ssp.h"


/**********************/



nRF_REGISTERS registers ;

void init_system();
void init_irq(){
  LPC_GPIO0->FIODIR &= !(1<<5) ;
  LPC_GPIOINT->IO0IntEnF |=(1<<5) ;
  NVIC_EnableIRQ(EINT3_IRQn);
}


void init_gpio(){

  NRF24L01_CE_OUT;
  NRF24L01_CE_LOW;
 
  NRF24L01_CSN_OUT ;
  NRF24L01_CSN_HIGH ;

  

}
void init_ssp(){
  
  
  LPC_PINCON->PINSEL0 |= 0x2<<14 ; //SCK1
  LPC_PINCON->PINSEL0 |= 0x2<<16 ; //MISO1
  LPC_PINCON->PINSEL0 |= 0x2<<18 ; //MOSI1
    

  SSP_CFG_Type cfg ;
 
  
  SSP_ConfigStructInit(&cfg) ;
  SSP_Init(LPC_SSP1,&cfg) ;
  SSP_Cmd(LPC_SSP1,ENABLE) ;

  

}


void init_registers(){
  unsigned char command = R_REGISTER ;
  unsigned int length = 1 ;
  ///////////////READ CONFIG/////////////////
  command = R_REGISTER | CONFIG ;
  read_register(command,&registers.CONFIG_R,1) ;
  ///////////////READ EN_AA/////////////////
  command = R_REGISTER | EN_AA ;
  read_register(command,&registers.EN_AA_R,1) ;
  ///////////////READ EN_RXADDR/////////////////
  command = R_REGISTER | EN_RXADDR ;
  read_register(command,&registers.EN_RXADDR_R,1) ;
  ///////////////READ SETUP_AW/////////////////
  command = R_REGISTER | SETUP_AW ;
  read_register(command,&registers.SETUP_AW_R,1) ;
  ///////////////READ SETUP_RETR/////////////////
  command = R_REGISTER | SETUP_RETR ;
  read_register(command,&registers.SETUP_RETR_R,1) ;
  ///////////////READ RF_CH/////////////////
  command = R_REGISTER | RF_CH ;
  read_register(command,&registers.RF_CH_R,1) ;
  ///////////////READ RF_SETUP/////////////////
  command = R_REGISTER | RF_SETUP ;
  read_register(command,&registers.RF_SETUP_R,1) ;
  ///////////////READ STATUS/////////////////
  command = R_REGISTER | STATUS ;
  read_register(command,&registers.STATUS_R,1) ;
  ///////////////READ OBSERVE_TX/////////////////
  command = R_REGISTER | OBSERVE_TX ;
  read_register(command,&registers.OBSERVE_TX_R,1) ;
  ///////////////READ RPD/////////////////
  command = R_REGISTER | RPD ;
  read_register(command,&registers.RPD_R,1) ;
  ///////////////READ FIFO_STATUS/////////////////
  command = R_REGISTER | FIFO_STATUS ;
  read_register(command,&registers.FIFO_STATUS_R,1) ;
  ///////////////READ DYNPD/////////////////
  command = R_REGISTER | DYNPD ;
  read_register(command,&registers.DYNPD_R,1) ;
  ///////////////READ FEATURE/////////////////
  command = R_REGISTER | FEATURE ;
  read_register(command,&registers.FEATURE_R,1) ;


} 


/**********************/

void nrf_init()
{
  //init_system();
  init_irq();
  init_gpio();
  init_ssp();
}



void nrf_power_up(){
  unsigned char command = W_REGISTER | CONFIG ; 
  unsigned int length = 1 ;
  registers.CONFIG_R |= (1 << PWR_UP) ;
  write_register(command, &registers.CONFIG_R,length) ;

}

void nrf_power_down(){
  unsigned char command = W_REGISTER | CONFIG ; 
  unsigned int length = 1 ;
  registers.CONFIG_R &= ~(1 << PWR_UP) ;
  write_register(command, &registers.CONFIG_R,length) ;
}

void nrf_payload(unsigned char* data, unsigned int length){
    
}

void intrupt(){
  
}

void write_register(unsigned char command, unsigned char* data, unsigned int length)
{
  NRF24L01_CSN_LOW ;
  
  SSP_SendData(LPC_SSP1,command) ;
  registers.STATUS_R = SSP_ReceiveData(LPC_SSP1) ;
  for (int i = 0 ; i < length ;i++){
    SSP_SendData(LPC_SSP1,data[i]) ;
  }
  
  while (SSP_GetStatus(LPC_SSP1,SSP_STAT_BUSY) );

  NRF24L01_CSN_HIGH ;
}

void read_register(unsigned char command, unsigned char* data, unsigned int length)
{
  NRF24L01_CSN_LOW ;
  
  SSP_SendData(LPC_SSP1,command) ;
  registers.STATUS_R = SSP_ReceiveData(LPC_SSP1) ;
  for (int i = length -1 ;i >= 0 ;i--){
    SSP_SendData(LPC_SSP1,0x00) ;
    data[i] = (uint8_t)SSP_ReceiveData(LPC_SSP1) ;
  }

  while (SSP_GetStatus(LPC_SSP1,SSP_STAT_BUSY) );

  NRF24L01_CSN_HIGH ;
}