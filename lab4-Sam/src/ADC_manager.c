#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "ADC_manager.h"

//Initializes the Analog to digital converter and temperature sensor
void configureADC(){
	printf("Starting ADC configuration");
	ADC_InitTypeDef adc_init_s;
	ADC_CommonInitTypeDef adc_common_init_s;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //Enable peripheral clock for ADC1
	adc_common_init_s.ADC_Mode = ADC_Mode_Independent;
	adc_common_init_s.ADC_Prescaler = ADC_Prescaler_Div2; //Indicates the frequency of the ADC clock
	adc_common_init_s.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //Disable Direct Memory Access
	adc_common_init_s.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //Configures the delay between two sampling phases
	ADC_CommonInit(&adc_common_init_s); //Initialization of common properties of ADC
	ADC_TempSensorVrefintCmd(ENABLE); //Wakes up temperature sensor on the board
	adc_init_s.ADC_Resolution = ADC_Resolution_12b; //Specifies the number of bits for conversion 
	adc_init_s.ADC_ScanConvMode = DISABLE; //set the conversion to be done on a single channel (disable scanning)
	adc_init_s.ADC_ContinuousConvMode = DISABLE; //Disable continuous mode so we can initialize the conversion every 20 ms
	adc_init_s.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //we don't trigger conversion externally so none
	adc_init_s.ADC_DataAlign = ADC_DataAlign_Right;
	adc_init_s.ADC_NbrOfConversion = 1; //we will do 1 conversion each time 
	ADC_Init(ADC1, &adc_init_s); //Initialization
	ADC_Cmd(ADC1, ENABLE); //Enable Module
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_480Cycles); //Setting up the channel we want to convert from 
}

//Gets the voltage coverted by the ADC
uint16_t getADCValue(){
	uint16_t conversion_value = 0;
	ADC_SoftwareStartConv(ADC1); //Starting Conversion, waiting for it to finish, clearing the flag, reading the result
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); 
	ADC_ClearFlag (ADC1, ADC_FLAG_EOC); //Clear the end of conversion flag
	conversion_value = ADC_GetConversionValue(ADC1); // Result available in ADC1->DR*/

	return conversion_value;
}