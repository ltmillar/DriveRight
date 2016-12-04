

#ifndef  _CAN_H
#define  _CAN_H
#include "core_cm4.h"
#include "stm32f4xx.h"
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);

#define CAN_RX0_INT_ENABLE 0
u8 Can_Send_Msg(u8* msg,u8 len);

u8 Can_Receive_Msg(u8 *buf);


#endif
