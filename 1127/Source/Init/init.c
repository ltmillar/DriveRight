#include "arducam.h"
#include "GPS_parsing.h"
#include "bluetooth.h"
#include "tm_stm32f4_delay.h"
#include "defines.h"

//#include "accelerometer.h"


int modules_init()
{
	InitCAMPins();
	InitCAM(JPEG);
	OV2640_set_JPEG_size(OV2640_640x480);
	GPS_Init();
	//Accelerometer_init();
	//WeightSensor_init();
	//PiComm_init();
	TM_DELAY_Init();
}
