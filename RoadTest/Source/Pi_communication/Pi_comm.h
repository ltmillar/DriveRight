#ifndef __PICOMM__
#define __PICOMM__

#include "GPS_parsing.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#define PI_BAUDRATE 19200


#define	PI_GPIO_CLK				RCC_AHB1Periph_GPIOD
#define PI_UART_CLK				RCC_APB1Periph_USART3
#define PI_GPIO_TX				GPIO_Pin_8
#define PI_GPIO_RX				GPIO_Pin_9
#define PI_GPIO_PORT			GPIOD
#define PI_GPIO_AF				GPIO_AF_USART3
#define PI_AF_PinSource_TX		GPIO_PinSource8
#define PI_AF_PinSource_RX		GPIO_PinSource9
#define PI_UART_PORT			USART3


typedef unsigned char byte;



int Pi_comm_init();
int Pi_SendBuf(byte* buf, int len);

int Pi_AggressiveDetected();
int Pi_PassengerDetected();
int Pi_SendGPS(struct GPS_info* GPS);

#endif
