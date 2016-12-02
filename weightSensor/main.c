#include "stm32f4xx_adc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

int ConvertedValue = 0; //Converted value readed from ADC


void adc_configure(){
 ADC_InitTypeDef ADC_init_structure; //Structure for adc confguration
 GPIO_InitTypeDef GPIO_initStructre1; //Structure for analog input pin
 GPIO_InitTypeDef GPIO_initStructre2; //Structure for analog input pin
 //Clock configuration
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//The ADC1 is connected the APB2 peripheral bus thus we will use its clock source
 RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIOBEN,ENABLE);//Clock for the ADC port!! Do not forget about this one ;)
 //Analog pin configuration
 GPIO_initStructre1.GPIO_Pin = GPIO_Pin_0;//The channel 10 is connected to PC0
 GPIO_initStructre1.GPIO_Mode = GPIO_Mode_AN; //The PC0 pin is configured in analog mode
 GPIO_initStructre1.GPIO_PuPd = GPIO_PuPd_NOPULL; //We don't need any pull up or pull down
 GPIO_Init(GPIOB,&GPIO_initStructre1);//Affecting the port with the initialization structure configuration

 GPIO_initStructre2.GPIO_Pin = GPIO_Pin_1;//The channel 10 is connected to PC0
 GPIO_initStructre2.GPIO_Mode = GPIO_Mode_AN; //The PC0 pin is configured in analog mode
 GPIO_initStructre2.GPIO_PuPd = GPIO_PuPd_NOPULL; //We don't need any pull up or pull down
 GPIO_Init(GPIOB,&GPIO_initStructre2);//Affecting the port with the initialization structure configuration
 //ADC structure configuration
 ADC_DeInit();
 ADC_init_structure.ADC_DataAlign = ADC_DataAlign_Right;//data converted will be shifted to right
 ADC_init_structure.ADC_Resolution = ADC_Resolution_12b;//Input voltage is converted into a 12bit number giving a maximum value of 4096
 ADC_init_structure.ADC_ContinuousConvMode = ENABLE; //the conversion is continuous, the input data is converted more than once
 ADC_init_structure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;// conversion is synchronous with TIM1 and CC1 (actually I'm not sure about this one :/)
 ADC_init_structure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//no trigger for conversion
 ADC_init_structure.ADC_NbrOfConversion = 1;//I think this one is clear :p
 ADC_init_structure.ADC_ScanConvMode = DISABLE;//The scan is configured in one channel
 ADC_Init(ADC1,&ADC_init_structure);//Initialize ADC with the previous configuration
 //Enable ADC conversion
 ADC_Cmd(ADC1,ENABLE);
 //Select the channel to be read from
 ADC_RegularChannelConfig(ADC1,ADC_Channel_8,1,ADC_SampleTime_144Cycles);
}
int adc_convert1(){
	 ADC_RegularChannelConfig(ADC1,ADC_Channel_8,1,ADC_SampleTime_144Cycles);
 ADC_SoftwareStartConv(ADC1);//Start the conversion
 while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//Processing the conversion
 return ADC_GetConversionValue(ADC1); //Return the converted data
}
int adc_convert2(){
	 ADC_RegularChannelConfig(ADC1,ADC_Channel_9,1,ADC_SampleTime_144Cycles);
 ADC_SoftwareStartConv(ADC1);//Start the conversion
 while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//Processing the conversion
 return ADC_GetConversionValue(ADC1); //Return the converted data
}
int is_pass1(){
	if(adc_convert1() > 2000)
		return 1;
	return 0;
}
int is_pass2(){
	if(adc_convert2() > 2000)
		return 1;
	return 0;
}
int main(void){
 adc_configure();//Start configuration
 int ispass1 = 0;
 int ispass2 = 0;
    while(1){//loop while the board is working
    	int test = adc_convert1();
    	int test2 = adc_convert2();
     ispass1 = is_pass1(); //B1
     ispass2 = is_pass2(); //B0
    }
}
