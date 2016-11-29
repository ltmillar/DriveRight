#ifndef __accelerometer
#define __accelerometer
#include "stm32f4xx_rcc.h"


#define ACC_GPIO_ENABLER_CLK		RCC_AHB1Periph_GPIOB
#define ACC_GPIO_ADC_CLK			RCC_AHB1Periph_GPIOC
#define ACC_ADC_CLK					RCC_APB2Periph_ADC1
#define ACC_ADC_PinX				GPIO_Pin_1
#define ACC_ADC_PinY				GPIO_Pin_2
#define ACC_ADC_PinZ				GPIO_Pin_3
#define ACC_ADC_PORT				GPIOC
#define ACC_ENB_Pin					GPIO_Pin_8
#define ACC_ENB_PORT				GPIOB
#define ACC_ADC_SEL					ADC1
#define ADC_TIMEOUT 0xFFF
int acc_init();
//rawinput
int getX();
int getY();
int getZ();

float getgX();
float getgY();
float getgZ();

int ADdetect();

#endif
