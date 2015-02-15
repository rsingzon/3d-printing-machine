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
#include "init.h"

#define AVG_SLOPE 2.5
#define AVG_SLOPE_INVERSE 1/AVG_SLOPE
#define SCALE 0.7624

float to_celsius(int v_sense);
static volatile uint_fast16_t ticks;
void delay_ms(int ms);

int main(){
	
	//Initialize the GPIOs and ADC
	initIO();
	initADC();
		
	// Reset the timer
	ticks = 0;
	
	// Configure SysTick to use 1ms period
	SysTick_Config(SystemCoreClock / 1000); 
	
	while(1){
		
		GPIO_SetBits(GPIOD,GPIO_Pin_12); // Turn on green
		
		GPIO_SetBits(GPIOD,GPIO_Pin_13); // Turn on orange
		
		GPIO_SetBits(GPIOD,GPIO_Pin_14); // Turn on red
	
		GPIO_SetBits(GPIOD,GPIO_Pin_15); // Turn on blue
		
		delay_ms(20);
		ticks = 0;		
		// Turn off all LEDs
		GPIO_ResetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
		
		// Read temperature values
		ADC_SoftwareStartConv(ADC1); //Starting Conversion, waiting for it to finish, clearing the flag, reading the result
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); //Could be through interrupts (Later)
		ADC_ClearFlag(ADC1, ADC_FLAG_EOC); //EOC means End Of Conversion
		ADC_GetConversionValue(ADC1); // Result available in ADC1->DR
		printf("%f\n",to_celsius(ADC1->DR));
		//printf("%d\n",ADC1->DR);
	}
	return 0;
}

/**
* @brief Converts ADC reading to temperature in celsius
* @retval integer 
*/
float to_celsius(int v_sense)
{
	v_sense = v_sense * SCALE;
	return ((v_sense - 760) * AVG_SLOPE_INVERSE) + 25;
}


/**
* @brief Interrupt handler for SysTick
* @details Called every 1 ms when the interrupt occurs
* @retval None
*/
void SysTick_Handler()
{
	ticks++;
}

/**
* @brief Delays specified number of milliseconds
* @details Called every 1 ms when the interrupt occurs
* @param Delay in milliseconds
* @retval None
*/
void delay_ms(int ms)
{
	while(ticks < ms);
}