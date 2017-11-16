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

  /*  Power on reset delay */
  nrf_hal_delay(100);
  
  nrf_config_crc(ENABLE, CRC_16BIT);
  nrf_set_retries(1500,15);
  nrf_set_data_rate(RATE_1MBPS);
  nrf_set_output_power(OUTPUT_12dBm);

  nrf_clear_interrupt_flags();
  nrf_set_rf_channel(76);
  nrf_flush_rx();
  nrf_flush_tx();
  nrf_set_power_state(POWER_UP);
  nrf_set_mode(TX);
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

  /* if powering up wait for oscillator startup */
  if(power == POWER_UP)
    nrf_hal_delay(5);
}

void nrf_config_crc(unsigned char enable, unsigned char scheme)
{
  unsigned char command = R_REGISTER | CONFIG;
  unsigned char length = 1;
  unsigned char data = 0;
  nrf_hal_read_register(command,&data,length);
  data = (enable == ENABLE) ? SetBit(data,EN_CRC) : ClrBit(data,EN_CRC);
  data = (scheme == CRC_16BIT) ? SetBit(data,CRCO) : ClrBit(data,CRCO);
  command = W_REGISTER | CONFIG;
  nrf_hal_write_register(command,&data,length);
}

void nrf_set_retries(unsigned short delay, unsigned char count)
{
  if(delay < 250)
    delay = 250;

  if(delay > 4000)
    delay = 4000;

  if(count > 15)
    count = 15;
  
  unsigned char delayCalculated = CalcRetransmitDelay(delay);
  unsigned char command = W_REGISTER | SETUP_RETR;
  unsigned char length = 1;
  unsigned char data = count | (delayCalculated << ARD);
  nrf_hal_write_register(command,&data,length);
}

void nrf_set_address_width(unsigned char width)
{
  if(width < 3)
    width = 3;
  if(width > 5)
    width = 5;

  unsigned char command = W_REGISTER | SETUP_AW;
  unsigned char length = 1;
  unsigned char data = width - 2;
  nrf_hal_write_register(command,&data,length);
}

void nrf_set_rf_channel(unsigned char channel)
{
  if(channel > 127)
    channel = 127;

  unsigned char command = W_REGISTER | RF_CH;
  unsigned char length = 1;
  unsigned char data = channel;
  nrf_hal_write_register(command,&data,length);
}

void nrf_set_data_rate(unsigned char rate)
{
  unsigned char command = R_REGISTER | RF_SETUP;
  unsigned char length = 1;
  unsigned char data = 0;
  nrf_hal_read_register(command,&data,length);

  data = (rate & 1) ? SetBit(data, RF_DR_HIGH) : ClrBit(data, RF_DR_HIGH);
  data = (rate & 2) ? SetBit(data, RF_DR_LOW) : ClrBit(data, RF_DR_LOW);

  command = W_REGISTER | RF_SETUP;
  nrf_hal_write_register(command,&data,length);
}

void nrf_set_output_power(unsigned char power)
{
  unsigned char command = R_REGISTER | RF_SETUP;
  unsigned char length = 1;
  unsigned char data = 0;
  nrf_hal_read_register(command,&data,length);

  data = data & 0xF8;
  data = data | (power << RF_PWR);

  command = W_REGISTER | RF_SETUP;
  nrf_hal_write_register(command,&data,length);
}

void nrf_clear_interrupt_flags()
{
  unsigned char command = W_REGISTER | STATUS;
  unsigned char length = 1;
  unsigned char data = SetBit(data, RX_DR) | SetBit(data, TX_DS) | SetBit(data, MAX_RT);
  nrf_hal_write_register(command,&data,length);
}
/*********************************************/



/************  Pipeline functions  ***********/
void nrf_config_tx_pipe(TX_CONFIG* conf){
  
}

void nrf_config_rx_pipe(RX_PIPE_CONFIG* conf){

}

void nrf_set_rx_pipe_en(unsigned char n,unsigned char en){

}

void nrf_set_tx_payload(unsigned char* data,unsigned char length){

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
/*********************************************/
