#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "GPS_parsing.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"

//inits
//send messages
//receive sentences
//extracting first sentence legal.
//parsing the sentnce
//sending it to another UART peripheral 



int GPS_init()
{
	//clock
	RCC_APB1PeriphClockCmd(GPS_UART_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(GPS_GPIO_CLK, ENABLE);
	//gpio init
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPS_GPIO_TX|GPS_GPIO_RX;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd =GPIO_PuPd_NOPULL;
	GPIO_Init(GPS_GPIO_PORT, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPS_GPIO_PORT,GPS_AF_PinSource_TX,GPS_GPIO_AF);
	GPIO_PinAFConfig(GPS_GPIO_PORT,GPS_AF_PinSource_RX,GPS_GPIO_AF);
	//uart init
	USART_InitTypeDef UART_handle;
	UART_handle.USART_BaudRate = GPS_BAUDRATE;
	UART_handle.USART_StopBits = USART_StopBits_1;
	UART_handle.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	UART_handle.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	UART_handle.USART_Parity =USART_Parity_No;
	UART_handle.USART_WordLength =USART_WordLength_8b;
	USART_Init(GPS_UART_PORT, &UART_handle);
	USART_Cmd(GPS_UART_PORT, ENABLE);

	return 0;
}


int GPS_get_rawdata(char* buf, int length)
{
	int i = 0;
	while( i < length )
	{
		while(!USART_GetFlagStatus(GPS_UART_PORT,USART_FLAG_RXNE)){}
		buf[i] = (char)USART_ReceiveData(GPS_UART_PORT);
		i++;
	}
	return 0;
}

int GPS_parsing(struct GPS_info* GPS, char* nmea)
{
	char* buf;
	char* _t;
	int32_t degree;
  long minutes;
  char degreebuff[10];
///////////////////
	buf = strstr(nmea, "$GPRMC");
	if(!buf) return -1;
	_t = strchr(buf, '*');
	if(!_t) return -1;
	char *p = buf;
// get time
  p = strchr(p, ',')+1;
  float timef = atof(p);
  uint32_t time = timef;
  GPS->hour = time / 10000;
  GPS->minute = (time % 10000) / 100;
  GPS->seconds = (time % 100);

  GPS->milliseconds = (timef - (float)time) * 1000;

    p = strchr(p, ',')+1;
    // Serial->println(p);
    if (p[0] == 'A') 
      GPS->GPS_status = 'A';
    else if (p[0] == 'V')
			GPS->GPS_status = 'V';
    else
      return -1;

    // parse out latitude
    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      strncpy(degreebuff, p, 2);
      p += 2;
      degreebuff[2] = '\0';
      long degree = atol(degreebuff) * 10000000;
      strncpy(degreebuff, p, 2); // minutes
      p += 3; // skip decimal point
      strncpy(degreebuff + 2, p, 4);
      degreebuff[6] = '\0';
      long minutes = 50 * atol(degreebuff) / 3;
      GPS->latitude_fixed = degree + minutes;
      GPS->latitude = degree / 100000 + minutes * 0.000006F;
      GPS->latitudeDegrees = (GPS->latitude-100*(int)(GPS->latitude/100))/60.0;
      GPS->latitudeDegrees += (int)(GPS->latitude/100);
    }
    
    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      if (p[0] == 'S') GPS->latitudeDegrees *= -1.0;
      if (p[0] == 'N') GPS->lat = 'N';
      else if (p[0] == 'S') GPS->lat = 'S';
      else if (p[0] == ',') GPS->lat = 0;
      else return -1;
    }
    
    // parse out longitude
    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      strncpy(degreebuff, p, 3);
      p += 3;
      degreebuff[3] = '\0';
      degree = atol(degreebuff) * 10000000;
      strncpy(degreebuff, p, 2); // minutes
      p += 3; // skip decimal point
      strncpy(degreebuff + 2, p, 4);
      degreebuff[6] = '\0';
      minutes = 50 * atol(degreebuff) / 3;
      GPS->longitude_fixed = degree + minutes;
      GPS->longitude = degree / 100000 + minutes * 0.000006F;
      GPS->longitudeDegrees = (GPS->longitude-100*(int)(GPS->longitude/100))/60.0;
      GPS->longitudeDegrees += (int)(GPS->longitude/100);
    }
    
    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      if (p[0] == 'W') GPS->longitudeDegrees *= -1.0;
      if (p[0] == 'W') GPS->lon = 'W';
      else if (p[0] == 'E') GPS->lon = 'E';
      else if (p[0] == ',') GPS->lon = 0;
      else return -1;
    }
    // speed
    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      GPS->speed = atof(p);
    }
    
    // angle
    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      GPS->angle = atof(p);
    }
    
    p = strchr(p, ',')+1;
    if (',' != *p)
    {
      uint32_t fulldate = atof(p);
      GPS->day = fulldate / 10000;
      GPS->month = (fulldate % 10000) / 100;
      GPS->year = (fulldate % 100);
    }
    // we dont parse the remaining, yet!
    return 1;
}
