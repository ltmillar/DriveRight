#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_fatfs.h"
#include "tm_stm32f4_spi.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "Bluetooth.h"
#include "semphr.h"
#include "GPS_parsing.h"
#include "Arducam.h"
#include "defines.h"


/*
 * Global variables
 */
struct GPS_info GPS_data;	// Used for GPS data storage



/*
 * Semaphores Declaration
 */
xSemaphoreHandle xSemaphoreBluetooth;
xSemaphoreHandle xSemaphoreGPS;
xSemaphoreHandle xSemaphoreArducam;
xSemaphoreHandle xSemaphorePiComm;
xSemaphoreHandle xSemaphoreSD;
/*
 *  Functions that are 'better' defined here
 */
void delay_unit(int time)
{
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	for(;time>0;time--)
	{
		vTaskDelayUntil(&xLastWakeTime,(1/portTICK_RATE_MS));
	}
}


void myCAMSaveToSDFile(FIL* file)
{
	  int i = 0;
	  byte buf[128];
	  uint8_t temp = 0,temp_last=0;
	  unsigned int br;
	  //Flush the FIFO
	  flush_fifo();
	  //Clear the capture done flag
	  clear_fifo_flag();
	  //Start capture
	  start_capture();
	  while(!get_bit(ARDUCHIP_TRIG , CAP_DONE_MASK));

	  selCAM();
	  set_fifo_burst();
	  temp=TM_SPI_Send(SPI2, 0x00);
	  //Read JPEG data from FIFO

	  while ( (temp !=0xD9) | (temp_last !=0xFF))
	  {
		  temp_last = temp;
		  temp = TM_SPI_Send(SPI2, 0x00);
		  //Write image data
		  if( i < 128)
			 buf[i++] = temp;
		  else
		  {
			deselCAM();
			f_write (file, buf, 128, &br);
			i = 0;
			buf[i++] = temp;
			selCAM();
			set_fifo_burst();
		  }
		 Delay(10);
	  }
	  if(i > 0)
	  {
		  deselCAM();
		  f_write (file, buf, i, &br);
	  }
 }

FRESULT open_append (
    FIL* fp,            /* [OUT] File object to create */
    const char* path    /* [IN]  File name to be opened */
)
{
    FRESULT fr;

    /* Opens an existing file. If not exist, creates a new file. */
    fr = f_open(fp, path, FA_WRITE | FA_OPEN_ALWAYS);
    if (fr == FR_OK) {
        /* Seek to end of the file to append data */
        fr = f_lseek(fp, f_size(fp));
        if (fr != FR_OK)
            f_close(fp);
    }
    return fr;
}


/*
 * bluetooth related tasks
 */

void vTaskBluetooth(void *pvParameters)
{
	while(1)
	{
		if(xSemaphoreBluetooth != NULL);
		{
			xSemaphoreTake(xSemaphoreBluetooth,100);
			const unsigned char B[]="emergency\r\n";
			Bluetooth_init();
			Bluetooth_send_data(B,sizeof(B));
		}
	}
}

void vTaskEmergency(void *pvParam)
{

	while(1)
	{
		xSemaphoreGive(xSemaphoreBluetooth);
		delay_unit(600000);
	}
}


/*
 * GPS related tasks
 */
void vTaskGPSwake(void *pcParam)
{
	char *pcTaskName;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	while(1)
	{
		xSemaphoreGive(xSemaphoreGPS);
		xSemaphoreTake( xSemaphorePiComm, portMAX_DELAY );
		Pi_SendGPS(GPS_data);
		xSemaphoreGive ( xSemaphorePiComm );
		vTaskDelayUntil( &xLastWakeTime, ( 1000 / portTICK_RATE_MS ) );
	}
}

void vTaskGPSreceive(void *pcParam)
{
	char _raw_data[256];
	FATFS FatFs;
	FIL fil;
	FRESULT fr;
	static int node = 0;
	while(1)
		{
			if(xSemaphoreGPS != NULL);
			{
				xSemaphoreTake(xSemaphoreGPS,100);
				GPS_get_rawdata(_raw_data, 256);
				GPS_parsing(&GPS_data, _raw_data);

				xSemaphoreTake( xSemaphoreSD, portMAX_DELAY );
				if (f_mount(&FatFs, "", 1) == FR_OK)
				{
					fr = open_append(&fil, "testDrive.txt");
					if (fr == FR_OK)
					{
						f_printf(&fil,"%d %lf %lf\n", node++, GPS_data.latitudeDegrees, GPS_data.longitude);
						f_close(&fil);
					}
				f_mount(0, "", 1);
				}
				xSemaphoreGive( xSemaphoreSD );
			}
		}
}


/*
 * weight Sensor and Arducam related tasks
 */
void vTaskReadWeightSensorData()
{
	char *pcTaskName;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	while(1)
	{
		// Read weight sensor data
		/*
		 * insert code here
		 */
		xSemaphoreGive(xSemaphoreArducam);
		xSemaphoreTake( xSemaphorePiComm, portMAX_DELAY );
		Pi_PassengerDetected();
		xSemaphoreGive( xSemaphorePiComm );
		vTaskDelayUntil( &xLastWakeTime, ( 1000 / portTICK_RATE_MS ) );
	}
}

void vTaskArducamCapture()
{
	FATFS FatFs;
	Fil fil;
	while(1)
	{
		xSemaphoreTake( xSemaphoreSD, portMAX_DELAY );
		if (f_mount(&FatFs, "", 1) == FR_OK)
		{
			if (f_open(&fil, "testDriveCAM.jpg", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK)
			{
				myCAMSaveToSDFile(&fil);
				f_close(&fil);
			}
		f_mount(0, "", 1);
		}
		xSemaphoreGive( xSemaphoreSD );
	}
}


/*
 * accelerometer related tasks
 */

void vTaskReadAccelerometer()
{
	char *pcTaskName;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	while(1)
	{
		 if(ADdetect())
		 {
			 xSemaphoreTake(xSemaphorePiComm, portMAX_DELAY );
			 sendUart2PI(ADDetected);
			 xSemaphoreGive(xSemaphorePiComm);
		 }
		vTaskDelayUntil( &xLastWakeTime, ( 250 / portTICK_RATE_MS ) );
	}
}



int main(void){

	// Semaphore Declaration!!
	vSemaphoreCreateBinary(xSemaphoreBluetooth);
	vSemaphoreCreateBinary(xSemaphoreGPS);
	vSemaphoreCreateBinary(xSemaphoreArducam);
	xSemaphoreSD = xSemaphoreCreateMutex();
	xSemaphorePiComm = xSemaphoreCreateMutex();
/*
 *  Create tasks
 */

	xTaskCreate( vTaskBluetooth, "Bluetooth", 1000, NULL, 1, NULL);
	xTaskCreate( vTaskEmergency, "Emergency", 1000, NULL, 2, NULL);

/*
 *	Create GPS logging file
 *	Create Aggressive Driving logging file
 */
	FATFS FatFs;
	FIL fil;
	if (f_mount(&FatFs, "", 1) == FR_OK)
	{
		if (f_open(&fil, "testDriveGPS.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK)
		{
			f_puts("format: NodeNumber Latitude Longitude\n", &fil);
			f_close(&fil);
		}
		if (f_open(&fil, "testDriveAD.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK)
		{
			f_puts("format: Time Aggressive driving type\n", &fil);
			f_close(&fil);
		}
		f_mount(0, "", 1);
	}

	vTaskStartScheduler();

	while (1)
	  {
	  }

}
