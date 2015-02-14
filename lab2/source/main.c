/*!
 * @file main.c
 * @brief This is the main file for lab 2
 * @author Ryan Singzon 260397455
 * @author Jeffrey Tichelman 260446802
 * @date February 14, 2015
 */

#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"


void delay(float time);

// Prototypes for enabling peripheral power
void RCC_AHB1PeriphClockCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState);
void RCC_AHB2PeriphClockCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState);
void RCC_AHB3PeriphClockCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState);
void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);

// Global variables
GPIO_InitTypeDef GPIO_InitStructure;
static volatile uint_fast16_t ticks;


int main(){
	
	// ex: Enable the clock for the GPIOA port
	// This will allow use of the GPIO pins in bank A (PA0, PA1, ....)
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	// Start clock on GPIOD
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 

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
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_480Cycles);
	ADC_SoftwareStartConv(ADC1); //Starting Conversion, waiting for it to finish, clearing the flag, reading the result
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); //Could be through interrupts (Later)
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC); //EOC means End Of Conversion
	ADC_GetConversionValue(ADC1); // Result available in ADC1->DR
	
	
	
	
	/* Configure the LEDs on pins 12-15
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
	
	// Reset number of ticks
	ticks = 0;
	
	// Configure for 10ms period
	// NOTE: argument here must be less than 0xFFFFFF; //(24 bit timer)
	// At 168MHz, this just a bit slower than 100Hz
	
	// Number of ticks between two interrupts
	// or 1/Freq* SystemCoreClock
	SysTick_Config(10 * SystemCoreClock / 1000); 
	
	while(1){
		
		GPIO_SetBits(GPIOD,GPIO_Pin_12); // Turn on green
		
		// Wait for an interrupt
		while(ticks < 100);
		// Decrement ticks
		ticks = 0;
		// Do something!
		GPIO_SetBits(GPIOD,GPIO_Pin_13); // Turn on orange
		
		
		while(ticks < 100);
		ticks = 0;		
		GPIO_SetBits(GPIOD,GPIO_Pin_14); // Turn on red

		while(ticks < 100);
		ticks = 0;		
		GPIO_SetBits(GPIOD,GPIO_Pin_15); // Turn on blue
		
		while(ticks < 100);
		ticks = 0;		
		// Turn off all LEDs
		GPIO_ResetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
		
		printf("%d\n",ADC1->DR);
	}
	
	return 0;
}

//Interrupt handler for system tick
//This should happen every 10ms
void SysTick_Handler(){
	ticks++;
}



void delay(float time)
{
	// Do nothing
	while(time--){}
}