#include "arducam.h"
#include "ov2640_regs.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_spi.h"

//first of all, set up all pins
void InitCAMPins()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
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
	GPIO_InitStructI2C.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_8;
	GPIO_InitStructI2C.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructI2C.GPIO_OType  = GPIO_OType_OD;
	GPIO_InitStructI2C.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructI2C.GPIO_PuPd =GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructI2C);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_I2C1);
	I2C_InitTypeDef I2C2_handle;//I2C setup
	I2C2_handle.I2C_Ack = I2C_Ack_Enable;
	I2C2_handle.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C2_handle.I2C_ClockSpeed = 10000;
	I2C2_handle.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C2_handle.I2C_Mode = I2C_Mode_I2C;
	I2C2_handle.I2C_OwnAddress1 = 0x00;
	I2C_Init(I2C1, &I2C2_handle);
	I2C_Cmd(I2C1, ENABLE);
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable the SPI clock */

	/* Enable GPIO clocks (they are all on GPIOB) */

	/* Connect SPI pins to AF5 */
	GPIO_PinAFConfig( CAM_SPI_GPIO_PORT, CAM_SPI_SCLK_SOURCE, CAM_SPI_AF );
	GPIO_PinAFConfig( CAM_SPI_GPIO_PORT, CAM_SPI_MISO_SOURCE, CAM_SPI_AF );
	GPIO_PinAFConfig( CAM_SPI_GPIO_PORT, CAM_SPI_MOSI_SOURCE, CAM_SPI_AF );

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* base clock is set to 50Mhz */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

	/* SPI SCK pin configuration */
	GPIO_InitStructure.GPIO_Pin = CAM_SPI_SCLK_PIN;
	GPIO_Init( CAM_SPI_GPIO_PORT, &GPIO_InitStructure );

	/* SPI  MISO pin configuration */
	GPIO_InitStructure.GPIO_Pin = CAM_SPI_MISO_PIN;
	GPIO_Init( CAM_SPI_GPIO_PORT, &GPIO_InitStructure );

	/* SPI  MOSI pin configuration */
	GPIO_InitStructure.GPIO_Pin =  CAM_SPI_MOSI_PIN;
	GPIO_Init( CAM_SPI_GPIO_PORT, &GPIO_InitStructure );

	/* SPI configuration -------------------------------------------------------*/
	SPI_I2S_DeInit( CAM_SPI );

	/* Initializes the SPI communication */
	SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;	/* actual speed of SPI bus is half of base clock */
	SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
	SPI_Init( CAM_SPI, &SPI_InitStructure );

	/* The Data transfer is performed in the SPI interrupt routine */
	SPI_Cmd( CAM_SPI, ENABLE );  /* Enable the SPI peripheral */
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
	SPI_Send(CAM_SPI,BURST_FIFO_READ);
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
  SPI_Send(CAM_SPI, address);
  SPI_Send(CAM_SPI, value);
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
  SPI_Send(CAM_SPI, address);
  value = SPI_Send(CAM_SPI, 0x00);
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
	while(I2C_GetFlagStatus(CAM_I2C, I2C_FLAG_BUSY));
	I2C_GenerateSTART(CAM_I2C, ENABLE);
	while(!I2C_CheckEvent(CAM_I2C, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(CAM_I2C, sensor_add, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(CAM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(CAM_I2C, regID);
	while(!I2C_CheckEvent(CAM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(CAM_I2C, regDat);
	while(!I2C_CheckEvent(CAM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP(CAM_I2C, ENABLE);
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
	  while(I2C_GetFlagStatus(CAM_I2C, I2C_FLAG_BUSY));
  I2C_GenerateSTART(CAM_I2C, ENABLE);
  	  while(!I2C_CheckEvent(CAM_I2C, I2C_EVENT_MASTER_MODE_SELECT));
  I2C_Send7bitAddress(CAM_I2C, sensor_add, I2C_Direction_Transmitter);
  	  while(!I2C_CheckEvent(CAM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C_SendData(CAM_I2C, regID);
  	  while(!I2C_CheckEvent(CAM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  I2C_GenerateSTOP(CAM_I2C, ENABLE);
  	  while(I2C_GetFlagStatus(CAM_I2C, I2C_FLAG_BUSY));
  //Wire.requestFrom((sensor_addr >> 1), 1);
  I2C_GenerateSTART(CAM_I2C, ENABLE);
  	  while(!I2C_CheckEvent(CAM_I2C, I2C_EVENT_MASTER_MODE_SELECT));
  I2C_Send7bitAddress(CAM_I2C, sensor_add|0x01, I2C_Direction_Receiver);
  	  while(!I2C_CheckEvent(CAM_I2C,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  I2C_AcknowledgeConfig(CAM_I2C, DISABLE);
  *regDat =I2C_ReceiveData(CAM_I2C);
	  while(!I2C_CheckEvent(CAM_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED));

  I2C_GenerateSTOP(CAM_I2C, ENABLE);
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



