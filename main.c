/*********************************************************************
*               SEGGER MICROCONTROLLER GmbH & Co. KG                 *
*       Solutions for real time microcontroller applications         *
**********************************************************************
*                                                                    *
*       (c) 2014 - 2017  SEGGER Microcontroller GmbH & Co. KG        *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start
*/

#include <stdio.h>
#include <stdlib.h>
#include "lpc17xx_ssp.h"
#include "LPC17xx.h"
#include "./nRF/nrf24.h"
SSP_CFG_Type sspconfig = {
8,
1,
1,
SSP_MASTER_MODE,
SSP_FRAME_SPI,
400000
};


/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/
void main(void) {
  nrf_init() ;
  nrf_power_up() ;
  nrf_TX_Mode() ;
  TX_CONFIG tcfg ;
  tcfg.setup_retr = 0x00 ;
  unsigned char addr[5] = {0x1,0,0,0,0} ;
  tcfg.tx_addr = addr ;
  config_tx_pipe(&tcfg);
  flush_tx();
  unsigned char payload[1] ;
  payload[0] = 0x65   ;
  set_tx_payload(payload,1) ;
  
  while(1){
    
  }
}
void EINT3_IRQHandler(){
  LPC_GPIOINT->IO0IntClr = (1 << 5);
  interrupt() ;
}

/*************************** End of file ****************************/
