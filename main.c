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

  while(1);
}
void EINT3_IRQHandler(){
  interrupt() ;
}

/*************************** End of file ****************************/
