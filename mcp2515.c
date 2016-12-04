#include "spi.h"
#include "mcp2515.h"

u8 dummy = 0;
u8 number=0,number1=0,number2=0,j,number3=0,number4=0,number5=0,number6=0;

void delay(int i)
{
	for(i;i>0;i--){}
}

void WriteSPI(u8 ch)
{
	SPI3_ReadWriteByte(ch);
}


//write the data to the exact address
void SPIByteWrite(u8 addr, u8 value)
{
	GPIOA->BSRRL = GPIO_Pin_15;
	WriteSPI(CAN_WRITE);
	WriteSPI(addr);
	WriteSPI(value);
	GPIOA->BSRRH = GPIO_Pin_15;
}

void SPIReset()
{
	GPIOA->BSRRL = GPIO_Pin_15;
	SPI3_ReadWriteByte(CAN_RESET);
	GPIOA->BSRRH = GPIO_Pin_15;
}


u8 SPIByteRead(u8 addr)
{
	u8 tempdata;

	GPIOA->BSRRL = GPIO_Pin_15;
	WriteSPI(CAN_READ);
	WriteSPI(addr);
	tempdata=ReadSPI();
	GPIOA->BSRRH = GPIO_Pin_15;
	return tempdata;
}

u8 ReadSPI(void)
{
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI3, 0xff);

	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI3);
}




void mcp_2515_init()
{
	SPIReset();
	delay(10);

	SPIByteWrite(RXM0SIDH,0x00);
	SPIByteWrite(RXM0SIDL,0x00);

	SPIByteWrite(CNF1,CAN_125Kbps);
	SPIByteWrite(CNF2,0x80|PHSEG1_3TQ|PRSEG_1TQ);
	SPIByteWrite(CNF3,PHSEG2_3TQ);

	SPIByteWrite(TXB0SIDH,0xFF);
	SPIByteWrite(TXB0SIDL,0xE0);

	SPIByteWrite(RXB0SIDH,0x00);
	SPIByteWrite(RXB0SIDL,0x60);
	SPIByteWrite(RXB0CTRL,0x60);
	SPIByteWrite(RXB0DLC,DLC_8);

	SPIByteWrite(RXF0SIDH,0xFF);
	SPIByteWrite(RXF0SIDL,0xE0);
	SPIByteWrite(RXM0SIDH,0xFF);
	SPIByteWrite(RXM0SIDL,0xE0);

	SPIByteWrite(CANINTF,0x00);
	SPIByteWrite(CANINTE,0x01);

	//set the normal work style
	SPIByteWrite(CANCTRL,REQOP_NORMAL|CLKOUT_ENABLED);
	dummy=SPIByteRead(CANSTAT);

	//judge whether it get into normal work style
	if(OPMODE_NORMAL!=(dummy&&0xE0))
	{
	  SPIByteWrite(CANCTRL,REQOP_NORMAL|CLKOUT_ENABLED);
	}
}

void CAN_Send_anylength(u8 *CAN_TX_Buf,u8 length1)
{
	//u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);

  unsigned char tempdata,j;
  tempdata=SPIByteRead(CAN_RD_STATUS);
  SPIByteWrite(TXB0DLC,length1);
  for(j=0;j<length1;j++)
  {
    SPIByteWrite(TXB0D0+j,CAN_TX_Buf[j]);
  }

  //judge for TXREQ
  if(tempdata&0x04)
  {
    delay(10);
    //clean TXREQ
    SPIByteWrite(TXB0CTRL,0);
    //wait until TXREQ
    while(SPIByteRead(CAN_RD_STATUS)&0x04);
  }
	GPIOA->BSRRL = GPIO_Pin_15;
  //send send request
  WriteSPI(CAN_RTS_TXB0);
	GPIOA->BSRRH = GPIO_Pin_15;
}
