#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"
#include "accelerometer.h"


int acc_init()
{
	RCC_APB1PeriphClockCmd(ACC_GPIO_ENABLER_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(ACC_GPIO_ADC_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(ACC_ADC_CLK, ENABLE);
	//gpio init
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = ACC_ADC_PinX|ACC_ADC_PinY|ACC_ADC_PinZ;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd =GPIO_PuPd_NOPULL;
	GPIO_Init(ACC_ADC_PORT, &GPIO_InitStruct);
	//enabler init
	GPIO_InitTypeDef GPIO_InitStruct_enabler;
	GPIO_InitStruct_enabler.GPIO_Pin = ACC_ENB_Pin;
	GPIO_InitStruct_enabler.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStruct_enabler.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStruct_enabler.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct_enabler.GPIO_PuPd =GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStruct_enabler);
	ADC_InitTypeDef Accelerometer_ADC;
	Accelerometer_ADC.ADC_ContinuousConvMode = ENABLE;
	Accelerometer_ADC.ADC_Resolution = ADC_Resolution_10b;
	Accelerometer_ADC.ADC_DataAlign = ADC_DataAlign_Right;
	Accelerometer_ADC.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	Accelerometer_ADC.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
	Accelerometer_ADC.ADC_NbrOfConversion = 3;
	Accelerometer_ADC.ADC_ScanConvMode = DISABLE;
	ADC_Init(ACC_ADC_SEL, &Accelerometer_ADC);
	ADC_Cmd(ACC_ADC_SEL,ENABLE);
}

int getX()
{
	int _temp;
	ADC_RegularChannelConfig(ACC_ADC_SEL, ACC_ADC_PinX, 1, 20);
	_temp = ADC_GetConversionValue(ADC1);
	return _temp;

}

int getY()
{
	int _temp;
	ADC_RegularChannelConfig(ACC_ADC_SEL, ACC_ADC_PinY, 1, 20);
	_temp = ADC_GetConversionValue(ADC1);
	return _temp;

}

int getZ()
{
	int _temp;
	ADC_RegularChannelConfig(ACC_ADC_SEL, ACC_ADC_PinZ, 1, 20);
	_temp = ADC_GetConversionValue(ADC1);
	return _temp;

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
