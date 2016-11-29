#ifndef __PICOMM__
#define __PICOMM__

#include "GPS_parsing.h"
#include "stm32f4xx_rcc.h"
#define PI_BAUDRATE 19200


#define PI_GPIO_CLK_TX			RCC_AHB1Periph_GPIOC
#define	PI_GPIO_CLK_RX			RCC_AHB1Periph_GPIOD
#define PI_UART_CLK				RCC_APB1Periph_UART5
#define PI_GPIO_TX				GPIO_Pin_12
#define PI_GPIO_RX				GPIO_Pin_2
#define PI_GPIO_PORT_TX			GPIOC
#define PI_GPIO_PORT_RX			GPIOD
#define PI_GPIO_AF				GPIO_AF_UART5
#define PI_AF_PinSource_TX		GPIO_PinSource12
#define PI_AF_PinSource_RX		GPIO_PinSource2
#define PI_UART_PORT			UART5


typedef unsigned char byte;



int Pi_comm_init();
int Pi_SendBuf(byte* buf, int len);

int Pi_AggressiveDetected();
int Pi_PassengerDetected();
int Pi_SendGPS(struct GPS_info* GPS);

#endif
