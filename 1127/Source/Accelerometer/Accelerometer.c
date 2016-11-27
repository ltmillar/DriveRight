#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"

int acc_init()
{
	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	//gpio init
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd =GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	//enabler init
	GPIO_InitTypeDef GPIO_InitStruct_enabler;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd =GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStruct_enabler);
	ADC_InitTypeDef Accelerometer_ADC;
	Accelerometer_ADC.ADC_ContinuousConvMode = ENABLE;
	Accelerometer_ADC.ADC_Resolution = ADC_Resolution_10b;
	Accelerometer_ADC.ADC_DataAlign = ADC_DataAlign_Right;
	Accelerometer_ADC.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	Accelerometer_ADC.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
	Accelerometer_ADC.ADC_NbrOfConversion = 3£»
	Accelerometer_ADC.ADC_ScanConvMode = DISABLE;
	ADC_Init(Accelerometer_ADC);
	ADC_Cmd(ADC1,ENABLE);
}

int getX()
{
	int _temp;
	ADC_RegularChannelConfig(ADC1, 1, 1, 20);
	_temp = ADC_GetConversionValue(ADC1);
	return temp;

}

int getY()
{
	int _temp;
	ADC_RegularChannelConfig(ADC1, 2, 1, 20);
	_temp = ADC_GetConversionValue(ADC1);
	return temp;

}

int getZ()
{
	int _temp;
	ADC_RegularChannelConfig(ADC1, 3, 1, 20);
	_temp = ADC_GetConversionValue(ADC1);
	return temp;

}


int ADdetect()
{
	int gx, gy, gz;
	gx = getX();
	gy = getY();
	gz = getZ();
	if(gx < 10 || gx > 1000)
	{
		return 1;
	}
	else if(gy > 1000)
	{
		return 2;
	}
	else return 0;
}
