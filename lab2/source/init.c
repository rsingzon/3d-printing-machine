/*!
 * @file init.h
 * @brief Header for the initialization code 
 * @author Ryan Singzon 260397455
 * @author Jeffrey Tichelman 260446802
 * @date February 14, 2015
 */
 
#include "init.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

/**
* @brief Initializes the IO ports
* @details This function sets pins 12-15 as outputs on GPIOD
* @details Also enables the clocks on GPIO banks A & D
* @retval None
*/
void initIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// Enable the clock for the GPIOA and GPIOD ports
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	/* Configure the LEDs on pins 12-15 to be outputs
	 * 12 = green/left
	 * 13 = orange/top
	 * 14 = red/right
	 * 15 = blue/bottom
	 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/**
* @brief Initializes the ADC
* @details This function sets pins 12-15 as outputs on GPIOD
* @retval None
*/
void initADC(void)
{
	ADC_InitTypeDef adc_init_s;
	ADC_CommonInitTypeDef adc_common_init_s;
	
	// Start clock for the ADC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	
	// Configure common ADC settings
	adc_common_init_s.ADC_Mode = ADC_Mode_Independent;
	adc_common_init_s.ADC_Prescaler = ADC_Prescaler_Div2;
	adc_common_init_s.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	adc_common_init_s.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	
	// Initialize ADC
	ADC_CommonInit(&adc_common_init_s);
	
	// Configure other ADC settings
	adc_init_s.ADC_Resolution = ADC_Resolution_12b;
	adc_init_s.ADC_ScanConvMode = DISABLE;
	adc_init_s.ADC_ContinuousConvMode = DISABLE;
	adc_init_s.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	adc_init_s.ADC_DataAlign = ADC_DataAlign_Right;
	adc_init_s.ADC_NbrOfConversion = 1;
	
	// Initialize ADC
	ADC_Init(ADC1, &adc_init_s);
	
	// Enable module
	ADC_Cmd(ADC1, ENABLE); 
	
	// Set ADC channel and sample time
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_480Cycles);
	
	
	ADC_TempSensorVrefintCmd(ENABLE); //Enable the connections to the temp sensor
	ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_480Cycles); //Cofigure the ADC Channel
}