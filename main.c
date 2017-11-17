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
#include "LPC17xx.h"
#include "./nRF/nrf24.h"
#include "./nRF/nrf_hal.h"

void init_irq_pin();


/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/
void main(void) {
  init_irq_pin();
  nrf_init() ;
 
  while(1){

  }
}

void init_irq_pin()
{
  LPC_GPIO0->FIODIR &= ~(1 << 5) ;
  LPC_GPIOINT->IO0IntEnF |=(1 << 5) ;
  NVIC_EnableIRQ(EINT3_IRQn);
}

void EINT3_IRQHandler(){
  LPC_GPIOINT->IO0IntClr = (1 << 5);
  /* TODO: Handle Interrupt */
}

/*************************** End of file ****************************/
