#ifndef __PICOMM__
#define __PICOMM__

#include "GPS_parsing.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#define PI_BAUDRATE 19200


#define	PI_GPIO_CLK				RCC_AHB1Periph_GPIOD
#define PI_UART_CLK				RCC_APB1Periph_USART2
#define PI_GPIO_TX				GPIO_Pin_5
#define PI_GPIO_RX				GPIO_Pin_6
#define PI_GPIO_PORT			GPIOD
#define PI_GPIO_AF				GPIO_AF_USART2
#define PI_AF_PinSource_TX		GPIO_PinSource5
#define PI_AF_PinSource_RX		GPIO_PinSource6
#define PI_UART_PORT			USART2


typedef unsigned char byte;



int Pi_comm_init();
int Pi_SendBuf(byte* buf, int len);

int Pi_AggressiveDetected();
int Pi_PassengerDetected();
int Pi_SendGPS(struct GPS_info* GPS);

#endif
