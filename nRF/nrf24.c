#include "nrf24.h"
#include "nrf_hal.h"


/*
 *  Status byte, Updated after sending command byte
 */
unsigned char status;


/*********  Configuration functions  *********/

/*
 *  Initialize platform and the chip
 */
void nrf_init()
{
  nrf_hal_init_system();
  nrf_hal_init_gpio();
  nrf_hal_init_ssp();
}


void nrf_interrupt()
{

}

void nrf_set_mode(unsigned char mode)
{
  unsigned char command = R_REGISTER | CONFIG;
  unsigned char length = 1;
  unsigned char data = 0;
  nrf_hal_read_register(command,&data,length);
  data = (mode == RX) ? SetBit(data,PRIM_RX) : ClrBit(data,PRIM_RX);
  command = W_REGISTER | CONFIG;
  nrf_hal_write_register(command,&data,length);
}

void nrf_set_power_state(unsigned char power)
{
  unsigned char command = R_REGISTER | CONFIG;
  unsigned char length = 1;
  unsigned char data = 0;
  nrf_hal_read_register(command,&data,length);
  data = (power == POWER_UP) ? SetBit(data,PWR_UP) : ClrBit(data,PWR_UP);
  command = W_REGISTER | CONFIG;
  nrf_hal_write_register(command,&data,length);
}


/*********************************************/


void nrf_config_tx_pipe(TX_CONFIG* conf){
  
}

void nrf_config_rx_pipe(RX_PIPE_CONFIG* conf){

}

void nrf_set_rx_pipe_en(unsigned char n,unsigned char en){

}

void nrf_set_tx_payload(unsigned char* data,unsigned char length){
  unsigned char command = W_TX_PAYLOAD_NOACK ;
  nrf_hal_write_register(command,data,length) ;
}

void nrf_rx_payload(unsigned char* data){

}

void nrf_flush_tx(){
  unsigned char command = FLUSH_TX;
  nrf_hal_write_register(command,0,0) ;
}

void nrf_flush_rx(){
  unsigned char command = FLUSH_RX;
  nrf_hal_write_register(command,0,0) ;
}