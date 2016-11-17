/**
  ******************************************************************************
  * @file    bluetooth.h
  * @author  Die Hu & Ping Yu
  * @date    17-November-2016
  * @brief   This file contains all the functions prototypes for the bluetooth
  * configuration and application functions.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#ifdef __cplusplus
 extern "C" {
#endif

 /* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"


int i,count;

u8 bluetooth_init(void);
void delay(count);
void bluetooth_setup(void);
void usart_rxtx(void);
void NVIC_Configuration(void);
void GPIO_Configuration(void);
void USART_Configuration(void);
void USART1_IRQHandler(void);
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STM32F4xx_H */
