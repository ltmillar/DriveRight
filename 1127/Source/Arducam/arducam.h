
#ifndef __ARDUCAM__
#define __ARDUCAM__



#define OV2640_160x120 		0	//160x120
#define OV2640_176x144 		1	//176x144
#define OV2640_320x240 		2	//320x240
#define OV2640_352x288 		3	//352x288
#define OV2640_640x480		4	//640x480
#define OV2640_800x600 		5	//800x600
#define OV2640_1024x768		6	//1024x768
#define OV2640_1280x1024	7	//1280x1024
#define OV2640_1600x1200	8	//1600x1200
#define OV2640_BMP          9

#define RWBIT									0x80  //READ AND WRITE BIT IS BIT[7]

#define ARDUCHIP_TEST1       	0x00  //TEST register


#define ARDUCHIP_MODE      		0x02  //Mode register
#define MCU2LCD_MODE       		0x00
#define CAM2LCD_MODE       		0x01
#define LCD2MCU_MODE       		0x02

#define ARDUCHIP_TIM       		0x03  //Timming control

#define ARDUCHIP_FIFO      		0x04  //FIFO and I2C control
#define FIFO_CLEAR_MASK    		0x01
#define FIFO_START_MASK    		0x02
#define FIFO_RDPTR_RST_MASK     0x10
#define FIFO_WRPTR_RST_MASK     0x20

#define ARDUCHIP_GPIO			  0x06  //GPIO Write Register

#define BURST_FIFO_READ			0x3C  //Burst FIFO read operation
#define SINGLE_FIFO_READ		0x3D  //Single FIFO read operation

#define ARDUCHIP_REV       		0x40  //ArduCHIP revision
#define VER_LOW_MASK       		0x3F
#define VER_HIGH_MASK      		0xC0

#define ARDUCHIP_TRIG      		0x41  //Trigger source
#define VSYNC_MASK         		0x01
#define SHUTTER_MASK       		0x02
#define CAP_DONE_MASK      		0x08

#define FIFO_SIZE1				0x42  //Camera write FIFO size[7:0] for burst to read
#define FIFO_SIZE2				0x43  //Camera write FIFO size[15:8]
#define FIFO_SIZE3				0x44  //Camera write FIFO size[18:16]


typedef unsigned char byte;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned uint32_t;


void InitCAMPins();///
void InitCAM();
void selCAM();//
void deselCAM();//

void flush_fifo(void);//
void start_capture(void);//
void clear_fifo_flag(void);//
uint8_t read_fifo(void);//

uint8_t read_reg(uint8_t addr);//
void write_reg(uint8_t addr, uint8_t data);//

uint32_t read_fifo_length(void);//
void set_fifo_burst(void);//
void set_bit(uint8_t addr, uint8_t bit);//
void clear_bit(uint8_t addr, uint8_t bit);//
uint8_t get_bit(uint8_t addr, uint8_t bit);//
void set_mode(uint8_t mode);//

//I2C address 0x60 for write and 0x61 for read

/****************************************************************/
/* define a structure for sensor register initialization values */
/****************************************************************/
struct sensor_reg {
	uint16_t reg;
	uint16_t val;
};

int wrSensorRegs(const struct sensor_reg*);//
int wrSensorRegs8_8(const struct sensor_reg*);//

byte wrSensorReg(int regID, int regDat);//
byte wrSensorReg8_8(int regID, int regDat);//

byte rdSensorReg8_8(uint8_t regID, uint8_t* regDat);//
//

void OV2640_set_JPEG_size(uint8_t size);//
void set_format(byte fmt);

int bus_write(int address, int value);//
uint8_t bus_read(int address);//

#define JPEG  0x01
#define sensor_add  0x60

byte m_fmt;
byte sensor_model;

//

#endif


