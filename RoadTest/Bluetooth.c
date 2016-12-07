#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "Bluetooth.h"

int Bluetooth_init()
{
	//clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	//gpio init
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd =GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4);
	//uart init
	USART_InitTypeDef UART_handle;
	UART_handle.USART_BaudRate = 9600;
	UART_handle.USART_StopBits = USART_StopBits_1;
	UART_handle.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	UART_handle.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	UART_handle.USART_Parity =USART_Parity_No;
	UART_handle.USART_WordLength =USART_WordLength_8b;
	USART_Init(UART4, &UART_handle);
	USART_Cmd(UART4, ENABLE);

	return 0;
}

void Bluetooth_send_data(const unsigned char *buf, unsigned long length)
{
	while(length--)
	{
		while(!USART_GetFlagStatus(UART4,USART_FLAG_TXE)){}
		USART_SendData(UART4, (uint16_t)*buf++);
	}
}




