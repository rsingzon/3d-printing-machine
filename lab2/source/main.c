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
		
		// Wait for an interrupt
		delay_ms(1000);
		// Decrement ticks
		ticks = 0;
		// Do something!
		GPIO_SetBits(GPIOD,GPIO_Pin_13); // Turn on orange
		
		
		delay_ms(1000);
		ticks = 0;		
		GPIO_SetBits(GPIOD,GPIO_Pin_14); // Turn on red

		delay_ms(1000);
		ticks = 0;		
		GPIO_SetBits(GPIOD,GPIO_Pin_15); // Turn on blue
		
		delay_ms(1000);
		ticks = 0;		
		// Turn off all LEDs
		GPIO_ResetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
		
		printf("%d\n",ADC1->DR);
	}
	
	return 0;
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