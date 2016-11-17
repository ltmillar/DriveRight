#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "misc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "cmsis_boot/stm32f4xx.h"




int i,count;

//ErrorStatus HSEStartUpStatus;

void NVIC_Configuration(void);
void GPIO_Configuration(void);
void USART_Configuration(void);
void USART1_IRQHandler(void);
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount);

int main(void)
{


	usart_rxtx();
    while(1)
    {

    }
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles USARTx global interrupt request
  * @param  None
  * @retval None
  */
void delay(count)
{
	while(count--);
}



u8 bluetooth_init(void)
{
	u8 retry=10,t;
	int flag=2;											//we can check the state by flag
//restore default status
	while(retry--)
	{
		delay(10);
		GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET);		//KEY=1, enter into AT module
		delay(10);
		UARTSend("AT+ORGL\r\n",sizeof("AT+ORGL\r\n"));			//send AT test instruction
		GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_RESET);
		for(t=0;i<10;t++)								//wait 10*5 time to receive data from HC06
		{
			if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
				break;

			else delay(5);
		}

		if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET) //receive data from HC06
		{
			i=USART_ReceiveData(USART1);                //give data received to variable i
			flag=1;									//receive data
			UARTSend("restore default status -- success\r\n",sizeof("restore default status -- success\r\n"));	// Send message to UART1
		}
		else
		{
			flag=0;
			UARTSend("restore default status --fail\r\n",sizeof("restore default status --fail\r\n"));	// Send message to UART1
		}
		flag=2;
	}

//set the name of "DriveRight"
	while(retry--)
	{
		delay(10);
		GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET);		//KEY=1, enter into AT module
		delay(10);
		UARTSend("AT+NAME=DriveRight\r\n",sizeof("AT+NAME=DriveRight\r\n"));			//send AT test instruction
		GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_RESET);
		for(t=0;i<10;t++)								//wait 10*5 time to receive data from HC06
		{
			if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
				break;

			else delay(5);
		}

		if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET) //receive data from HC06
		{
			i=USART_ReceiveData(USART1);                //give data received to variable i
			flag=1;									//receive data
			UARTSend("set the name -- success\r\n",sizeof("set the name -- success\r\n"));	// Send message to UART1
		}
		else
		{
			flag=0;
			UARTSend("set the name --fail\r\n",sizeof("set the name --fail\r\n"));	// Send message to UART1
		}
		flag=2;
	}

//set the pin code

	while(retry--)
		{
			delay(10);
			GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET);		//KEY=1, enter into AT module
			delay(10);
			UARTSend("AT+PSWD=1234\r\n",sizeof("AT+PSWD=1234\r\n"));			//send AT test instruction
			GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_RESET);
			for(t=0;i<10;t++)								//wait 10*5 time to receive data from HC06
			{
				if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
					break;

				else delay(5);
			}

			if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET) //receive data from HC06
			{
				i=USART_ReceiveData(USART1);                //give data received to variable i
				flag=1;									//receive data
				UARTSend("set the pin code -- success\r\n",sizeof("set the pin code -- success\r\n"));	// Send message to UART1
			}
			else
			{
				flag=0;
				UARTSend("set the pin code --fail\r\n",sizeof("set the pin code --fail\r\n"));	// Send message to UART1
			}
			flag=2;
		}

//initialize spp profile lib

	while(retry--)
			{
				delay(10);
				GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET);		//KEY=1, enter into AT module
				delay(10);
				UARTSend("AT+INIT\r\n",sizeof("AT+INIT\r\n"));			//send AT test instruction
				GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_RESET);
				for(t=0;i<10;t++)								//wait 10*5 time to receive data from HC06
				{
					if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
						break;

					else delay(5);
				}

				if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET) //receive data from HC06
				{
					i=USART_ReceiveData(USART1);                //give data received to variable i
					flag=1;									//receive data
					UARTSend("initialize spp profile lib -- success\r\n",sizeof("initialize spp profile lib -- success\r\n"));	// Send message to UART1
				}
				else
				{
					flag=0;
					UARTSend("initialize spp profile lib --fail\r\n",sizeof("initialize spp profile lib --fail\r\n"));	// Send message to UART1
				}
				flag=2;
			}

//set to pairable state

	while(retry--)
			{
				delay(10);
				GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET);		//KEY=1, enter into AT module
				delay(10);
				UARTSend("AT+INQ\r\n",sizeof("AT+INQ\r\n"));			//send AT test instruction
				GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_RESET);
				for(t=0;i<10;t++)								//wait 10*5 time to receive data from HC06
				{
					if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
						break;

					else delay(5);
				}

				if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET) //receive data from HC06
				{
					i=USART_ReceiveData(USART1);                //give data received to variable i
					flag=1;									//receive data
					UARTSend("set to pairable state -- success\r\n",sizeof("set to pairable state -- success\r\n"));	// Send message to UART1
				}
				else
				{
					flag=0;
					UARTSend("set to pairable state --fail\r\n",sizeof("set to pairable state --fail\r\n"));	// Send message to UART1
				}
				flag=2;
			}

//if you want to set bt to be master, then "AT+ROLE=1"
//if you want to set bt to be slave, then "AT+ROLE=0"
	while(retry--)
				{
					delay(10);
					GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET);		//KEY=1, enter into AT module
					delay(10);
					UARTSend("AT+ROLE=1\r\n",sizeof("AT+ROLE=1\r\n"));			//send AT test instruction
					GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_RESET);
					for(t=0;i<10;t++)								//wait 10*5 time to receive data from HC06
					{
						if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
							break;

						else delay(5);
					}

					if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET) //receive data from HC06
					{
						i=USART_ReceiveData(USART1);                //give data received to variable i
						flag=1;									//receive data
						UARTSend("Module role set to master -- success\r\n",sizeof("Module role set to master -- success\r\n"));	// Send message to UART1
					}
					else
					{
						flag=0;
						UARTSend("Module role set to master --fail\r\n",sizeof("Module role set to master --fail\r\n"));	// Send message to UART1
					}
					flag=2;
				}


}




void USART1_IRQHandler(void)
{
	uint8_t data[20];
	uint8_t counter = 0;
	u8 retry=10,t;
    if ((USART1->SR & USART_FLAG_RXNE) != (u16)RESET)
	{

		i = USART_ReceiveData(USART1);
		if(i == '1')
		{
			GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET);		// Set '1' on PA8
			UARTSend("LED ON\r\n",sizeof("LED ON\r\n"));	// Send message to UART1
		}
		else if(i == '0')
		{
			GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_RESET);		// Set '0' on PA8
			UARTSend("LED OFF\r\n",sizeof("LED OFF\r\n"));
		}

	}
}

void usart_rxtx(void)
{
    const unsigned char welcome_str[] = " Welcome to Bluetooth!\r\n";

    /* Enable USART1 and GPIOA clock */
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /* NVIC Configuration */
    NVIC_Configuration();

    /* Configure the GPIOs */
    GPIO_Configuration();

    /* Configure the USART1 */
    USART_Configuration();

    /* Enable the USART1 Receive interrupt: this interrupt is generated when the
         USART1 receive data register is not empty */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    /* print welcome information */
    UARTSend(welcome_str, sizeof(welcome_str));
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure (PA.8) as output */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure); // Save

  /* Configure USART1 Tx (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART1 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : USART_Configuration
* Description    : Configures the USART1
*******************************************************************************/
void USART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;

/* USART1 configuration ------------------------------------------------------*/
  USART_InitStructure.USART_BaudRate = 9600;		// Baud Rate
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure);

  /* Enable USART1 */
  USART_Cmd(USART1, ENABLE);
}

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : UARTSend
* Description    : Send a string to the UART.
* Input          : - pucBuffer: buffers to be printed.
*                : - ulCount  : buffer's length
*******************************************************************************/
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount)
{
    //
    // Loop while there are more characters to send.
    //
    while(ulCount--)
    {
        USART_SendData(USART1, (uint16_t) *pucBuffer++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        {
        }
    }
}



