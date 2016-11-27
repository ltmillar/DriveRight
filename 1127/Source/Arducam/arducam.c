#include "arducam.h"
#include "ov2640_regs.h"
#include "tm_stm32f4_spi.h"
#include "tm_stm32f4_delay.h"
#include "stm32f4xx_i2c.h"
#include "defines.h"


//first of all, set up all pins
void InitCAMPins()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructCS;
	//CS pin PB12
	GPIO_InitStructCS.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructCS.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructCS.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStructCS.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructCS.GPIO_PuPd =GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructCS);
	//I2C pins:I2C2 PB10 PB11
	GPIO_InitTypeDef GPIO_InitStructI2C;		//I2C GPIO
	GPIO_InitStructI2C.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructI2C.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructI2C.GPIO_OType  = GPIO_OType_OD;
	GPIO_InitStructI2C.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructI2C.GPIO_PuPd =GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructI2C);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_I2C2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_I2C2);
	I2C_InitTypeDef I2C2_handle;//I2C setup
	I2C2_handle.I2C_Ack = I2C_Ack_Enable;
	I2C2_handle.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C2_handle.I2C_ClockSpeed = 20000;
	I2C2_handle.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C2_handle.I2C_Mode = I2C_Mode_I2C;
	I2C2_handle.I2C_OwnAddress1 = 0x00;
	I2C_Init(I2C2, &I2C2_handle);
	I2C_Cmd(I2C2, ENABLE);
	//SPI pins SPI2 pack2 PB13 PB14 PB15
	TM_SPI_Init(SPI2, TM_SPI_PinsPack_2);
}

//Set corresponding bit
void set_bit(uint8_t addr, uint8_t bit)
{
  uint8_t temp;
  temp = read_reg(addr);
  write_reg(addr, temp | bit);

}

//Clear corresponding bit
void clear_bit(uint8_t addr, uint8_t bit)
{
  uint8_t temp;
  temp = read_reg(addr);
  write_reg(addr, temp & (~bit));
}

//Get corresponding bit status
uint8_t get_bit(uint8_t addr, uint8_t bit)
{
  uint8_t temp;
  temp = read_reg(addr);
  temp = temp & bit;
  return temp;
}

//Set ArduCAM working mode
//MCU2LCD_MODE: MCU writes the LCD screen GRAM
//CAM2LCD_MODE: Camera takes control of the LCD screen
//LCD2MCU_MODE: MCU read the LCD screen GRAM
void set_mode(uint8_t mode)
{
  switch (mode)
  {
    case MCU2LCD_MODE:
      write_reg(ARDUCHIP_MODE, MCU2LCD_MODE);
      break;
    case CAM2LCD_MODE:
      write_reg(ARDUCHIP_MODE, CAM2LCD_MODE);
      break;
    case LCD2MCU_MODE:
      write_reg(ARDUCHIP_MODE, LCD2MCU_MODE);
      break;
    default:
      write_reg(ARDUCHIP_MODE, MCU2LCD_MODE);
      break;
  }
}

//Reset the FIFO pointer to ZERO
void flush_fifo(void)
{
  write_reg(ARDUCHIP_FIFO, FIFO_CLEAR_MASK);
}

//Send capture command
void start_capture(void)
{
  write_reg(ARDUCHIP_FIFO, FIFO_START_MASK);
}

//Clear FIFO Complete flag
void clear_fifo_flag(void)
{
  write_reg(ARDUCHIP_FIFO, FIFO_CLEAR_MASK);
}

//Read FIFO single
uint8_t read_fifo(void)
{
  uint8_t data;
  data = bus_read(SINGLE_FIFO_READ);
  return data;
}

//Read Write FIFO length
//Support ArduCAM Mini only
uint32_t read_fifo_length(void)
{
  uint32_t len1, len2, len3, length = 0;
  len1 = read_reg(FIFO_SIZE1);
  len2 = read_reg(FIFO_SIZE2);
  len3 = read_reg(FIFO_SIZE3) & 0x7f;
  length = ((len3 << 16) | (len2 << 8) | len1) & 0x07fffff;
  return length;
}

//Send read fifo burst command
//Support ArduCAM Mini only
void set_fifo_burst()
{
	TM_SPI_Send(SPI2,BURST_FIFO_READ);
}
//chip select functions
void selCAM()
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET);
};

void deselCAM()
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET);
};


//Low level SPI write operation
int bus_write(int address, int value) {
  // take the SS pin low to select the chip:
  selCAM();
  //  send in the address and value via SPI:
  TM_SPI_Send(SPI2, address);
  TM_SPI_Send(SPI2, value);
  // take the SS pin high to de-select the chip:
  deselCAM();
  return 1;
}

//Low level SPI read operation
uint8_t bus_read(int address) {
  uint8_t value = 0;
  // take the SS pin low to select the chip:
  selCAM();
  //  send in the address and value via SPI:
  TM_SPI_Send(SPI2, address);
  value = TM_SPI_Send(SPI2, 0x00);
  // take the SS pin high to de-select the chip:
  deselCAM();
  return value;
}

//Write ArduChip internal registers
void write_reg(uint8_t addr, uint8_t data)
{
  bus_write(addr | 0x80, data);
}

//Read ArduChip internal registers
uint8_t read_reg(uint8_t addr)
{
  uint8_t data;
  data = bus_read(addr & 0x7F);
  return data;
}


//I2C sending command data
byte wrSensorReg8_8(int regID, int regDat)
{
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C2, sensor_add, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2C2, regID);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(I2C2, regDat);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP(I2C2, ENABLE);
  return (1);
}


int wrSensorRegs8_8(const struct sensor_reg reglist[])
{
  int err = 0;
  uint16_t reg_addr = 0;
  uint16_t reg_val = 0;
  int i = 0;
  while ((reg_addr != 0xff) | (reg_val != 0xff))
  {
    reg_addr = reglist[i].reg;
    reg_val = reglist[i].val;
    err = wrSensorReg8_8(reg_addr, reg_val);
    i++;
  }

  return 1;
}
//I2C Read 8bit address, 8bit data
byte rdSensorReg8_8(uint8_t regID, uint8_t* regDat)
{

  //Wire.beginTransmission(sensor_addr >> 1);
  //Wire.write(regID & 0x00FF);
  //Wire.endTransmission();
	  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
  I2C_GenerateSTART(I2C2, ENABLE);
  	  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
  I2C_Send7bitAddress(I2C2, sensor_add, I2C_Direction_Transmitter);
  	  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C_SendData(I2C2, regID);
  	  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  I2C_GenerateSTOP(I2C2, ENABLE);
  	  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
  //Wire.requestFrom((sensor_addr >> 1), 1);
  I2C_GenerateSTART(I2C2, ENABLE);
  	  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
  I2C_Send7bitAddress(I2C2, sensor_add|0x01, I2C_Direction_Receiver);
  	  while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  I2C_AcknowledgeConfig(I2C2, DISABLE);
  *regDat =I2C_ReceiveData(I2C2);
	  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
  Delay(1);

  I2C_GenerateSTOP(I2C2, ENABLE);
  return (1);
}


void OV2640_set_JPEG_size(uint8_t size)
{
  switch (size)
  {
    case OV2640_160x120:
      wrSensorRegs8_8(OV2640_160x120_JPEG);
      break;
    case OV2640_176x144:
      wrSensorRegs8_8(OV2640_176x144_JPEG);
      break;
    case OV2640_320x240:
      wrSensorRegs8_8(OV2640_320x240_JPEG);
      break;
    case OV2640_352x288:
      wrSensorRegs8_8(OV2640_352x288_JPEG);
      break;
    case OV2640_640x480:
      wrSensorRegs8_8(OV2640_640x480_JPEG);
      break;
    case OV2640_800x600:
      wrSensorRegs8_8(OV2640_800x600_JPEG);
      break;
    case OV2640_1024x768:
      wrSensorRegs8_8(OV2640_1024x768_JPEG);
      break;
    case OV2640_1280x1024:
      wrSensorRegs8_8(OV2640_1280x1024_JPEG);
      break;
    case OV2640_1600x1200:
      wrSensorRegs8_8(OV2640_1600x1200_JPEG);
      break;
    default:
      wrSensorRegs8_8(OV2640_QVGA);
      break;
  }
}


void InitCAM(int m_fmt)
{
	byte rtn = 0;
	byte reg_val;
	uint16_t val;
      wrSensorReg8_8(0xff, 0x01);
      wrSensorReg8_8(0x12, 0x80);
      Delayms(100);
      if (m_fmt == 1)
      {
    	wrSensorRegs8_8(OV2640_JPEG_INIT);
        wrSensorRegs8_8(OV2640_YUV422);
        wrSensorRegs8_8(OV2640_JPEG);
        wrSensorReg8_8(0xff, 0x01);
        wrSensorReg8_8(0x15, 0x00);
        wrSensorRegs8_8(OV2640_320x240_JPEG);
        //wrSensorReg8_8(0xff, 0x00);
        //wrSensorReg8_8(0x44, 0x32);
      }
      else
      {
        wrSensorRegs8_8(OV2640_QVGA);
      }
}



