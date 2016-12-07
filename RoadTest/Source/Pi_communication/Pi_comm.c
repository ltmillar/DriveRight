#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"
#include "PI_comm.h"
#include "GPS_parsing.h"

int Pi_comm_init()
{
	//clock
	RCC_AHB1PeriphClockCmd(PI_GPIO_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(PI_UART_CLK, ENABLE);
	//gpio init
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = PI_GPIO_TX;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd =GPIO_PuPd_NOPULL;
	GPIO_Init(PI_GPIO_PORT, &GPIO_InitStruct);
	GPIO_PinAFConfig(PI_GPIO_PORT,PI_AF_PinSource_TX,PI_GPIO_AF);
	//GPIO_PinAFConfig(PI_GPIO_PORT,PI_AF_PinSource_RX,PI_GPIO_AF);
	//uart init
	USART_InitTypeDef UART_handle;
	UART_handle.USART_BaudRate = PI_BAUDRATE;
	UART_handle.USART_StopBits = USART_StopBits_1;
	UART_handle.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	UART_handle.USART_Mode = USART_Mode_Tx;
	UART_handle.USART_Parity =USART_Parity_No;
	UART_handle.USART_WordLength =USART_WordLength_8b;
	USART_Init(PI_UART_PORT, &UART_handle);
	USART_Cmd(PI_UART_PORT, ENABLE);

	return 0;
}

int Pi_sendBuf(byte* buf, int len)
{
	int i;
	for(i = 0; i < len; i++)
	{
		while(!USART_GetFlagStatus(PI_UART_PORT,USART_FLAG_TC));
		USART_SendData(PI_UART_PORT, (uint16_t)buf[i]);
	}

	return len;
}


int Pi_AggressiveDetected()
{
	char sendBuf[10];
	sendBuf[0] = 0xFF;  // dummy byte
	sendBuf[1] = 0xAA;  // start indicator
	sendBuf[2] = 0xAD;	// type: aggressive driving
	sendBuf[3] = 0xED;	// end byte
	Pi_sendBuf(sendBuf, 4);
	return 4;
}


int Pi_PassengerDetected()
{
	char sendBuf[10];
	sendBuf[0] = 0xFF;  // dummy byte
	sendBuf[1] = 0xAA;  // start indicator
	sendBuf[2] = 0xCD;	// type: Passenger Detected
	sendBuf[3] = 0xED;	// end byte
	Pi_sendBuf(sendBuf, 4);
	return 4;
}

int Pi_SendGPS(struct GPS_info* GPS)
{
	char sendBuf[20];
	sendBuf[0] = 0xFF;  // dummy byte
	sendBuf[1] = 0xAA;  // start indicator
	sendBuf[2] = 0xCD;	// type: GPS_sending
	sendBuf[3] = GPS->lat;  //latitude
	sendBuf[4] = GPS->lon;	//longitude
	sendBuf[5] = GPS->year;	// time format YY-MM-DD-HH-MIN
	sendBuf[6] = GPS->month;
	sendBuf[7] = GPS->day;
	sendBuf[8] = GPS->hour;
	sendBuf[9] = GPS->minute;
	sendBuf[10] = 0xED;	// end byte
	Pi_sendBuf(sendBuf, 10);
	return 10;
}





