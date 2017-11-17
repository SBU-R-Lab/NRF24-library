#include "nrf24.h"
#include "nrf_hal.h"


/*
 *  Status byte, Updated after sending command byte
 */
unsigned char status;

unsigned char rf_channel;


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

void nrf_mask_rx_dr_interrupt(unsigned char state)
{
  unsigned char command = R_REGISTER | CONFIG;
  unsigned char length = 1;
  unsigned char data = 0;
  nrf_hal_read_register(command,&data,length);
  data = (state == NOT_REFLECT) ? SetBit(data,MASK_RX_DR) : ClrBit(data,MASK_RX_DR);
  command = W_REGISTER | CONFIG;
  nrf_hal_write_register(command,&data,length);
}

void nrf_mask_tx_ds_interrupt(unsigned char state)
{
  unsigned char command = R_REGISTER | CONFIG;
  unsigned char length = 1;
  unsigned char data = 0;
  nrf_hal_read_register(command,&data,length);
  data = (state == NOT_REFLECT) ? SetBit(data,MASK_TX_DS) : ClrBit(data,MASK_TX_DS);
  command = W_REGISTER | CONFIG;
  nrf_hal_write_register(command,&data,length);
}

void nrf_mask_max_rt_interrupt(unsigned char state)
{
  unsigned char command = R_REGISTER | CONFIG;
  unsigned char length = 1;
  unsigned char data = 0;
  nrf_hal_read_register(command,&data,length);
  data = (state == NOT_REFLECT) ? SetBit(data,MASK_MAX_RT) : ClrBit(data,MASK_MAX_RT);
  command = W_REGISTER | CONFIG;
  nrf_hal_write_register(command,&data,length);
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

  rf_channel = channel;

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

  data = data & 0xF9;
  data = data | (power << RF_PWR);

  command = W_REGISTER | RF_SETUP;
  nrf_hal_write_register(command,&data,length);
}

void nrf_set_continuous_carrier_transmit(unsigned char cont_wave)
{
  unsigned char command = R_REGISTER | RF_SETUP;
  unsigned char length = 1;
  unsigned char data = 0;
  nrf_hal_read_register(command,&data,length);

  data = (cont_wave == ENABLE) ? SetBit(data, CONT_WAVE) : ClrBit(data, CONT_WAVE);

  command = W_REGISTER | RF_SETUP;
  nrf_hal_write_register(command,&data,length);
}

void nrf_set_pll_lock(unsigned char pll_lock)
{
  unsigned char command = R_REGISTER | RF_SETUP;
  unsigned char length = 1;
  unsigned char data = 0;
  nrf_hal_read_register(command,&data,length);

  data = (pll_lock == ENABLE) ? SetBit(data, PLL_LOCK) : ClrBit(data, PLL_LOCK);

  command = W_REGISTER | RF_SETUP;
  nrf_hal_write_register(command,&data,length);
}

void nrf_clear_interrupt_flags()
{
  unsigned char command = W_REGISTER | STATUS;
  unsigned char length = 1;
  unsigned char data = 0;
  data = SetBit(data, RX_DR) | SetBit(data, TX_DS) | SetBit(data, MAX_RT);
  nrf_hal_write_register(command,&data,length);
}

unsigned char nrf_get_status()
{
  unsigned char command = NOP;
  return nrf_hal_write_register(command,0,0);
}

unsigned char nrf_get_fifo_status()
{
  unsigned char command = R_REGISTER | FIFO_STATUS;
  unsigned char length = 1;
  unsigned char data = 0;
  nrf_hal_read_register(command,&data,length);
  return data;
}

unsigned char nrf_get_rpd_status()
{
  unsigned char command = R_REGISTER | RPD;
  unsigned char length = 1;
  unsigned char data = 0;
  nrf_hal_read_register(command,&data,length);
  return (data) ? CARRIER_DETECTED : CARRIER_NOT_DETECTED;
}

void nrf_get_tx_observation_status(unsigned char* plos_cnt, unsigned char* arc_cnt)
{
  unsigned char command = R_REGISTER | OBSERVE_TX;
  unsigned char length = 1;
  unsigned char data = 0;
  nrf_hal_read_register(command,&data,length);
  
  *plos_cnt = (data & 0xF0) >> PLOS_CNT;
  *arc_cnt  = (data & 0x0F) >> ARC_CNT;
}

void nrf_reset_lost_packets_counter()
{
  unsigned char command = W_REGISTER | RF_CH;
  unsigned char length = 1;
  unsigned char data = rf_channel;
  nrf_hal_write_register(command,&data,length);
}

void nrf_enable_dynamic_payload_length_feature(unsigned char state)
{
  unsigned char command = R_REGISTER | FEATURE;
  unsigned char length = 1;
  unsigned char data = 0;
  nrf_hal_read_register(command,&data,length);
  data = (state == ENABLE) ? SetBit(data, EN_DPL) : ClrBit(data, EN_DPL);
  command = W_REGISTER | FEATURE;
  nrf_hal_write_register(command, &data, length);
}

void nrf_enable_payload_with_ack_feature(unsigned char state)
{
  unsigned char command = R_REGISTER | FEATURE;
  unsigned char length = 1;
  unsigned char data = 0;
  nrf_hal_read_register(command,&data,length);
  data = (state == ENABLE) ? SetBit(data, EN_ACK_PAY) : ClrBit(data, EN_ACK_PAY);
  command = W_REGISTER | FEATURE;
  nrf_hal_write_register(command, &data, length);
}

void nrf_enable_dynamic_ack_feature(unsigned char state)
{
  unsigned char command = R_REGISTER | FEATURE;
  unsigned char length = 1;
  unsigned char data = 0;
  nrf_hal_read_register(command,&data,length);
  data = (state == ENABLE) ? SetBit(data, EN_DYN_ACK) : ClrBit(data, EN_DYN_ACK);
  command = W_REGISTER | FEATURE;
  nrf_hal_write_register(command, &data, length);
}

void nrf_enable_chip(unsigned char state)
{
  nrf_hal_set_ce(state);
}
/*********************************************/



/************  Pipeline functions  ***********/

void nrf_enable_rx_pipe_auto_ack(unsigned char pipe, unsigned char state)
{
  if(pipe > 5)
    return;

  unsigned char command = R_REGISTER | EN_AA;
  unsigned char length = 1;
  unsigned char data = 0;
  nrf_hal_read_register(command, &data, length);
  data = (state == ENABLE) ? SetBit(data, pipe) : ClrBit(data,pipe);
  command = W_REGISTER | EN_AA;
  nrf_hal_write_register(command,&data,length);
}

void nrf_enable_rx_pipe(unsigned char pipe, unsigned char state)
{
  if(pipe > 5)
    return;

  unsigned char command = R_REGISTER | EN_RXADDR;
  unsigned char length = 1;
  unsigned char data = 0;
  nrf_hal_read_register(command, &data, length);
  data = (state == ENABLE) ? SetBit(data, pipe) : ClrBit(data,pipe);
  command = W_REGISTER | EN_RXADDR;
  nrf_hal_write_register(command,&data,length);
}

void nrf_set_tx_pipe_address(unsigned char* address)
{
  unsigned char command = W_REGISTER | TX_ADDR;
  unsigned char length = 5;
  nrf_hal_write_register(command,address,length);
}

void nrf_set_rx_pipe_address(unsigned char pipe, unsigned char* address)
{
  if (pipe > 5)
    return;

  unsigned char command = W_REGISTER | (RX_ADDR_P0 + pipe);
  unsigned char length = (pipe < 2) ? 5 : 1;
  nrf_hal_write_register(command,address,length);
}

void nrf_enable_dynamic_payload_length_pipe(unsigned char pipe, unsigned char state)
{
  if(pipe > 5)
    return;

  unsigned char command = R_REGISTER | DYNPD;
  unsigned char length = 1;
  unsigned char data = 0;
  nrf_hal_read_register(command, &data, length);
  data = (state == ENABLE) ? SetBit(data, pipe) : ClrBit(data,pipe);
  command = W_REGISTER | DYNPD;
  nrf_hal_write_register(command,&data,length);
}

void nrf_set_rx_pipe_size(unsigned char pipe, unsigned char size)
{
  if (pipe > 5)
    return;

  if(size > 32)
    size = 32;

  unsigned char command = W_REGISTER | (RX_PW_P0 + pipe);
  unsigned char length = 1;
  unsigned char data = size;
  nrf_hal_write_register(command,&data,length);
}

unsigned char nrf_read_rx_payload_size()
{
  unsigned char command = R_RX_PL_WID;
  unsigned char length = 1;
  unsigned char data = 0;
  nrf_hal_read_register(command, &data, length);
  return data;
}

void nrf_read_rx_payload(unsigned char* data, unsigned char length)
{
  unsigned char command = R_RX_PAYLOAD;
  nrf_hal_read_register(command, data, length);
}

void nrf_write_tx_payload(unsigned char mode, unsigned char length, unsigned char* data)
{
   unsigned char command = 0;
   switch (mode)
   {
      case NORMAL:
        command = W_TX_PAYLOAD;
        break;

      case NOACK_PAYLOAD:
        command = W_TX_PAYLOAD_NOACK;
        break;
   }

   nrf_hal_write_register(command, data, length);
}

void nrf_write_ack_payload(unsigned char pipe, unsigned char length, unsigned char* data)
{
  if(pipe > 5)
    return;

  unsigned char command = W_ACK_PAYLOAD | pipe;
  nrf_hal_write_register(command,data,length);
}

void nrf_reuse_tx_payload()
{
  unsigned char command = REUSE_TX_PL;
  nrf_hal_write_register(command,0,0) ;
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
