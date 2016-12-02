#include "stm32f4xx_adc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

int ConvertedValue = 0; //Converted value readed from ADC


void adc_configure(){
 ADC_InitTypeDef ADC_init_structure_acc2; //Structure for adc confguration
 ADC_InitTypeDef ADC_init_structure_acc3; //Structure for adc confguration
 GPIO_InitTypeDef GPIO_initStructre1_acc; //Structure for analog input pin
 GPIO_InitTypeDef GPIO_initStructre2_acc; //Structure for analog input pin
 GPIO_InitTypeDef GPIO_initStructre3_acc; //Structure for analog input pin
 //Clock configuration
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);//The ADC1 is connected the APB2 peripheral bus thus we will use its clock source
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);//The ADC1 is connected the APB2 peripheral bus thus we will use its clock source
 RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOCEN,ENABLE);//Clock for the ADC port!! Do not forget about this one ;)
 //Analog pin configuration
 GPIO_initStructre1_acc.GPIO_Pin = GPIO_Pin_1;//The channel 10 is connected to PC0
 GPIO_initStructre1_acc.GPIO_Mode = GPIO_Mode_AN; //The PC0 pin is configured in analog mode
 GPIO_initStructre1_acc.GPIO_PuPd = GPIO_PuPd_NOPULL; //We don't need any pull up or pull down
 GPIO_Init(GPIOC,&GPIO_initStructre1_acc);//Affecting the port with the initialization structure configuration

 GPIO_initStructre2_acc.GPIO_Pin = GPIO_Pin_2;//The channel 10 is connected to PC0
 GPIO_initStructre2_acc.GPIO_Mode = GPIO_Mode_AN; //The PC0 pin is configured in analog mode
 GPIO_initStructre2_acc.GPIO_PuPd = GPIO_PuPd_NOPULL; //We don't need any pull up or pull down
 GPIO_Init(GPIOC,&GPIO_initStructre2_acc);//Affecting the port with the initialization structure configuration

 GPIO_initStructre3_acc.GPIO_Pin = GPIO_Pin_0;
 GPIO_initStructre3_acc.GPIO_Mode = GPIO_Mode_OUT;
 GPIO_initStructre3_acc.GPIO_OType = GPIO_OType_PP;
 GPIO_initStructre3_acc.GPIO_Speed = GPIO_Speed_100MHz;
 GPIO_initStructre3_acc.GPIO_PuPd = GPIO_PuPd_NOPULL;
 GPIO_Init(GPIOC, &GPIO_initStructre3_acc);
 //ADC structure configuration
 ADC_init_structure_acc2.ADC_DataAlign = ADC_DataAlign_Right;//data converted will be shifted to right
 ADC_init_structure_acc2.ADC_Resolution = ADC_Resolution_12b;//Input voltage is converted into a 12bit number giving a maximum value of 4096
 ADC_init_structure_acc2.ADC_ContinuousConvMode = ENABLE; //the conversion is continuous, the input data is converted more than once
 ADC_init_structure_acc2.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;// conversion is synchronous with TIM1 and CC1 (actually I'm not sure about this one :/)
 ADC_init_structure_acc2.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//no trigger for conversion
 ADC_init_structure_acc2.ADC_NbrOfConversion = 1;//I think this one is clear :p
 ADC_init_structure_acc2.ADC_ScanConvMode = DISABLE;//The scan is configured in one channel
 ADC_Init(ADC2,&ADC_init_structure_acc2);//Initialize ADC with the previous configuration
 //Enable ADC conversion
 ADC_Cmd(ADC2,ENABLE);
 //Select the channel to be read from
 ADC_RegularChannelConfig(ADC2,ADC_Channel_11,1,ADC_SampleTime_144Cycles);

 ADC_init_structure_acc3.ADC_DataAlign = ADC_DataAlign_Right;//data converted will be shifted to right
 ADC_init_structure_acc3.ADC_Resolution = ADC_Resolution_12b;//Input voltage is converted into a 12bit number giving a maximum value of 4096
 ADC_init_structure_acc3.ADC_ContinuousConvMode = ENABLE; //the conversion is continuous, the input data is converted more than once
 ADC_init_structure_acc3.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;// conversion is synchronous with TIM1 and CC1 (actually I'm not sure about this one :/)
 ADC_init_structure_acc3.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//no trigger for conversion
 ADC_init_structure_acc3.ADC_NbrOfConversion = 1;//I think this one is clear :p
 ADC_init_structure_acc3.ADC_ScanConvMode = DISABLE;//The scan is configured in one channel
 ADC_Init(ADC3,&ADC_init_structure_acc3);//Initialize ADC with the previous configuration
 //Enable ADC conversion
 ADC_Cmd(ADC3,ENABLE);
 //Select the channel to be read from
 ADC_RegularChannelConfig(ADC3,ADC_Channel_12,1,ADC_SampleTime_144Cycles);
}
int adc_convert1(){
	 //ADC_RegularChannelConfig(ADC1,ADC_Channel_11,1,ADC_SampleTime_144Cycles);
 ADC_SoftwareStartConv(ADC2);//Start the conversion
 while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC));//Processing the conversion
 return ADC_GetConversionValue(ADC2); //Return the converted data
}
int adc_convert2(){
	 //ADC_RegularChannelConfig(ADC1,ADC_Channel_12,1,ADC_SampleTime_144Cycles);
 ADC_SoftwareStartConv(ADC3);//Start the conversion
 while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC));//Processing the conversion
 return ADC_GetConversionValue(ADC3); //Return the converted data
}
int DDy_detect(){
	static old = 0;
	int current = adc_convert1();
	if(((old > 3000) && (current > 3000)) || ((old < 1000) && (current < 1000))){
		old = current;
		return 1;
	}
	old = current;
	return 0;
}
int DDx_detect(){
	static old = 0;
	int current = adc_convert2();
	if(((old > 3000) && (current > 3000)) || ((old < 1000) && (current < 1000))){
		old = current;
		return 1;
	}
	old = current;
	return 0;
}
/*
int main(void){
	 ADC_DeInit();
 adc_configure();//Start configuration
 GPIO_SetBits(GPIOC, GPIO_Pin_0);
 int DDx = 0;
 int DDy = 0;
    while(1){//loop while the board is working
    	int test = adc_convert1(); //C1
    	//add delay here
    	int test2 = adc_convert2(); //C2
    	//add delay here
     DDx = DDx_detect(); //B1
     DDy = DDy_detect(); //B0
    }
}
*/
