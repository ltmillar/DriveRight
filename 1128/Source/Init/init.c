#include "arducam.h"
#include "GPS_parsing.h"
#include "bluetooth.h"
#include "tm_stm32f4_delay.h"
#include "defines.h"
#include "Accelerometer.h"
#include "weightsensor.h"
#include "Pi_comm.h"


int modules_init()
{
	//InitCAMPins();
	//InitCAM(JPEG);
	//OV2640_set_JPEG_size(OV2640_640x480);
	GPS_init();
	//acc_init();
	//ADC3_CH3_DMA_Config();
	Pi_comm_init();
	TM_DELAY_Init();
}
