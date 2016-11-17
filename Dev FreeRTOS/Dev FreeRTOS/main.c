#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define LED_Pin_1 GPIO_Pin_12
#define LED_Pin_2 GPIO_Pin_13
#define LED_Pin_3 GPIO_Pin_14
#define LED_Pin_4 GPIO_Pin_15


void setup(void){

   bluetooth_setup();

}

void vTaskBluetooth(void *pvParameters) {
	for (;;) {
		vTaskDelay(10);
	}
}

void vTaskLED2(void *pvParameters) {
	for (;;) {
		GPIO_SetBits(GPIOD, LED_Pin_2);
		vTaskDelay(100);
		GPIO_ResetBits(GPIOD, LED_Pin_2);
		vTaskDelay(100);
	}
}



int main(void){

	setup();

	xTaskCreate( vTaskBluetooth, ( signed char * ) "Bluetooth", configMINIMAL_STACK_SIZE, NULL, 1,( xTaskHandle * ) NULL);

	vTaskStartScheduler();

	while (1)
	  {
	  }

}
