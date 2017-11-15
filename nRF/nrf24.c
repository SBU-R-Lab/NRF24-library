#include "nrf24.h"
#include "nrf_hal.h"


/**********************/

unsigned char* BufferPointer[6][2];

nRF_REGISTERS registers ;

unsigned char TX_ADDR_GLOBAL[5] ;


/**********************/

void nrf_init()
{
  registers.TX_ADDR_R = TX_ADDR_GLOBAL ;
  init_system();
  init_irq();
  init_gpio();
  init_ssp();
  init_registers();
  unsigned char dat = 0xF0;
  write_register(W_REGISTER | STATUS, &dat,1);
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
  /////////
  command = R_REGISTER | TX_ADDR ;
  read_register(command,registers.TX_ADDR_R,5) ;
} 


/**********************/

void config_tx_pipe(TX_CONFIG* conf){
  registers.SETUP_RETR_R = conf->setup_retr ;
  registers.TX_ADDR_R = conf->tx_addr ;
  unsigned char command ;
  unsigned char length = 1 ;
  command = W_REGISTER | SETUP_RETR ;
  write_register(command,&registers.SETUP_RETR_R,length) ;
  command = W_REGISTER | FEATURE;
  registers.FEATURE_R = 1;
  write_register(command, &registers.FEATURE_R,length);
////  length = 5 ;
////  command = W_REGISTER | TX_ADDR ;
////  write_register(command,registers.TX_ADDR_R,length) ;
//  registers.EN_RXADDR_R = 0x01 ;
//  command = W_REGISTER | EN_RXADDR ;
//  write_register(command,&registers.EN_RXADDR_R,1) ;
//  command = W_REGISTER | RX_ADDR_P0 ;
//  write_register(command,&registers.TX_ADDR_R,length) ;
  
}

void config_rx_pipe(RX_PIPE_CONFIG* conf){
  unsigned int n = conf -> pipe_number ;

  if (conf->en_ack)
    registers.EN_AA_R |= (1<<n) ;
  else
    registers.EN_AA_R &= ~(1<<n) ;

  registers.RX_ADDR_R[n] = conf->addr ;
  registers.RX_PW_R[n] = conf->paylaod_size ;

  if (conf->en_dynpld)
    registers.DYNPD_R |=  (1<<n) ;
  else
    registers.DYNPD_R &=  ~(1<<n) ;

  BufferPointer[n][0] = conf->recieve_buffer;
  BufferPointer[n][1] = conf->transmit_buffer;
  

  unsigned char command ;
  unsigned char length = 1 ;
  command = W_REGISTER | EN_AA ;
  write_register(command,&registers.EN_AA_R,length) ;
  command = W_REGISTER | RX_ADDR_P0 + n ;
  length = (n / 2 == 0) ? 5 : 1 ;
  write_register(command,registers.RX_ADDR_R[n],length) ;
  command = W_REGISTER | RX_PW_P0 + n ;
  length = 1 ;
  write_register(command,&registers.RX_PW_R[n],length) ;
  command = W_REGISTER | DYNPD ;
  write_register(command,&registers.DYNPD_R,length) ;
  
}
void set_rx_pipe_en(unsigned char n,unsigned char en){
  unsigned char command ;
  unsigned char length = 1 ;
  if (en)
    registers.EN_RXADDR_R |= (1<<n) ;
  else
    registers.EN_RXADDR_R &= ~(1<<n) ;
  command = W_REGISTER | EN_RXADDR ;
  write_register(command,&registers.EN_RXADDR_R,length) ;
}
//void nrf_init_RX(){
//  unsigned char command ;
//  unsigned int length = 1 ;
//  //setting pipe line  
//  //setting payload size
//  command = W_REGISTER | RX_PW_P0 ;
//  //setting address
//  
//}
void set_tx_payload(unsigned char* data,unsigned char length){
  unsigned char command = W_TX_PAYLOAD_NOACK ;
  write_register(command,data,length) ;
}



void nrf_RX_Mode(){
  unsigned char command = W_REGISTER | CONFIG ;
  unsigned int length = 1 ;
  //registers.CONFIG_R |= (1 << PWR_UP) ;
  registers.CONFIG_R |= (1 << PRIM_RX) ;
  write_register(command,&registers.CONFIG_R,length) ;
  setCE(HIGH) ;
}
void nrf_TX_Mode(){
  unsigned char command = W_REGISTER | CONFIG ;
  unsigned int length = 1 ;
  //registers.CONFIG_R |= (1 << PWR_UP) ;
  registers.CONFIG_R &= ~(1 << PRIM_RX) ;
  write_register(command,&registers.CONFIG_R,length) ;
  setCE(HIGH) ;
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

void rx_payload(unsigned char* data){
  unsigned char command = R_REGISTER | STATUS ;
  read_register(command,&registers.STATUS_R,1) ;
  unsigned char rx_pipe_n = (registers.STATUS_R & 0xE) >> 1 ; 
  command = R_REGISTER | RX_PW_P0 + rx_pipe_n ;
  unsigned char rx_payload_size = 0 ;
  read_register(command,&rx_payload_size,1) ;
  command = R_RX_PAYLOAD ;
  read_register(command,data,rx_payload_size) ;
}

void flush_tx(){
  unsigned char command = FLUSH_TX ;
  write_register(command,NULL,0) ;
}

void interrupt(){
  read_register(NOP,&registers.STATUS_R,0);
  read_register(R_REGISTER | OBSERVE_TX,&registers.OBSERVE_TX_R,1);
  unsigned char dat = 0xF0;
  write_register(W_REGISTER | STATUS, &dat,1);
  dat = 0x02;
  write_register(W_REGISTER | RF_CH, &dat,1);
}

void write_register(unsigned char command, unsigned char* data, unsigned int length)
{ 
  setCSN(LOW) ;
  
  SSP_SendData(LPC_SSP1,command) ;
  registers.STATUS_R = SSP_ReceiveData(LPC_SSP1) ;
  for (int i = 0 ; i < length ;i++){
    SSP_SendData(LPC_SSP1,data[i]) ;
  }
  
  while (SSP_GetStatus(LPC_SSP1,SSP_STAT_BUSY) );

  setCSN(HIGH) ;
}

void read_register(unsigned char command, unsigned char* data, unsigned int length)
{
  setCSN(LOW) ;
  
  SSP_SendData(LPC_SSP1,command) ;
  registers.STATUS_R = SSP_ReceiveData(LPC_SSP1) ;
  for (int i = length -1 ;i >= 0 ;i--){
      SSP_SendData(LPC_SSP1,0x00) ;
      while (SSP_GetStatus(LPC_SSP1,SSP_STAT_BUSY) );
        data[i] = (uint8_t)SSP_ReceiveData(LPC_SSP1) ;
  }

  while (SSP_GetStatus(LPC_SSP1,SSP_STAT_BUSY) );

  setCSN(HIGH) ;
}
