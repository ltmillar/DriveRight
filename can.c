#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"
#include "core_cm4.h"
#include "stm32f4xx_can.h"
#include "can.h"



u8 can_init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	CAN_InitTypeDef    CAN_InitStruct;
	CAN_FilterInitTypeDef  CAN_FilterInitStruct;

	//enable the clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	//init GPIO A 12
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//init GPIO A 11
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    //init CAN unit
    CAN_InitStruct.CAN_TTCM = DISABLE;
    CAN_InitStruct.CAN_ABOM = DISABLE;
    CAN_InitStruct.CAN_AWUM = DISABLE;
    CAN_InitStruct.CAN_NART = ENABLE;
    CAN_InitStruct.CAN_RFLM = DISABLE;
    CAN_InitStruct.CAN_TXFP = DISABLE;
    CAN_InitStruct.CAN_Mode = mode;

    CAN_InitStruct.CAN_SJW = tsjw;
    CAN_InitStruct.CAN_BS1 = tbs1;
    CAN_InitStruct.CAN_BS2 = tbs2;
    CAN_InitStruct.CAN_Prescaler = brp;
    CAN_Init(CAN1, &CAN_InitStruct);

    CAN_FilterInitStruct.CAN_FilterNumber = 0;
    CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStruct.CAN_FilterScale=CAN_FilterScale_32bit;
    CAN_FilterInitStruct.CAN_FilterIdHigh=0x0000;
    CAN_FilterInitStruct.CAN_FilterIdLow=0x0000;
    CAN_FilterInitStruct.CAN_FilterMaskIdHigh=0x0000;
    CAN_FilterInitStruct.CAN_FilterMaskIdLow=0x0000;
    CAN_FilterInitStruct.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
    CAN_FilterInitStruct.CAN_FilterActivation=ENABLE;

    CAN_FilterInit(&CAN_FilterInitStruct);

    return 0;
}

u8 can_send_message(u8* msg, u8 len)
{
	u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
	TxMessage.StdId=0x12;
	TxMessage.ExtId=0x12;
	TxMessage.IDE=0;
	TxMessage.RTR=0;
	TxMessage.DLC=len;
	for(i=0;i<len;i++)
		TxMessage.Data[i] = msg[i];
	mbox= CAN_Transmit(CAN1, &TxMessage);
	i=0;
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;

	if(i>=0XFFF)return 1;

	return 0;
}

u8 can_receive_message(u8 *buf)
{
	u32 i;
	CanRxMsg RxMessage;
	if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	for(i=0;i<8;i++)
		buf[i]=RxMessage.Data[i];
	return RxMessage.DLC;
}
