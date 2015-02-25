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
#include "led.h"
#include "kstate.h"


#define AVG_SLOPE 2.5
#define AVG_SLOPE_INVERSE 1/AVG_SLOPE
#define SCALE 0.715

float to_celsius(int v_sense);
static volatile uint_fast16_t ticks;
void delay_ms(int ms);
int readADC();
void fadeLEDs();

 int main(){
	
	//Initialize the GPIOs and ADC
	initIO();
	initADC();
		
	// Reset the timer
	ticks = 0;
	
	// Configure SysTick to use 20ms period
	SysTick_Config(20 * SystemCoreClock / 1000); 
	
	kalman_state kstate = {15.0, 1.0, 0.0, 0.0, 0.0};
	 
	float temp;
	float filteredTemp = to_celsius(readADC());
	float referenceTemp = filteredTemp;
	
	int currentLED = 'B';
	GPIO_SetBits(GPIOD,GPIO_Pin_15); // Turn on blue
	
	while(1){
		
		// Read temperature values
		temp = to_celsius(readADC());
		
		// Filter temperature values	
		filteredTemp = kalman_update(&kstate, temp);	
		printf("%f\n", filteredTemp);
		
		// Display warning if temperature exceeds 50 degrees
		if (filteredTemp >= 40.0) {
			fadeLEDs();
			referenceTemp = filteredTemp;
		}
		
		// Illuminate LEDs in clockwise fashion if the temperature is increasing
		else if (filteredTemp - referenceTemp > 2) {
			GPIO_ResetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
			switch(currentLED){
				case 'B':
					GPIO_SetBits(GPIOD,GPIO_Pin_12); // Turn on green
					currentLED = 'G';
					break;
				case 'G':
					GPIO_SetBits(GPIOD,GPIO_Pin_13); // Turn on orange
					currentLED = 'O';
					break;
				case 'O': 
					GPIO_SetBits(GPIOD,GPIO_Pin_14); // Turn on red
					currentLED = 'R';
					break;
				case 'R': 
					GPIO_SetBits(GPIOD,GPIO_Pin_15); // Turn on blue
					currentLED = 'B';
					break;
			}
			referenceTemp = filteredTemp;
		} 
		
		// Illuminate the LEDs in a counter clockwise fashion if the temperature is decreasing
		else if (filteredTemp - referenceTemp < -2) {
			GPIO_ResetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
			switch(currentLED){
				case 'B':
					GPIO_SetBits(GPIOD,GPIO_Pin_14); // Turn on red
					currentLED = 'R';
					break;
				case 'R': 
					GPIO_SetBits(GPIOD,GPIO_Pin_13); // Turn on orange
					currentLED = 'O';
					break;
				case 'O': 
					GPIO_SetBits(GPIOD,GPIO_Pin_12); // Turn on green
					currentLED = 'G';
					break;
				case 'G':
					GPIO_SetBits(GPIOD,GPIO_Pin_15); // Turn on blue
					currentLED = 'B';
					break;
			}
			referenceTemp = filteredTemp;
		}
		else{
			
		}		
		
		// Wait until the next SysTick interrupt
		delay_ms(1);
		ticks = 0;				
	}
	return 0;
}

/**
* @brief Reads the temperature sensor 
* @retval integer 
*/
int readADC() 
{
	ADC_SoftwareStartConv(ADC1); //Starting Conversion, waiting for it to finish, clearing the flag, reading the result
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); //Could be through interrupts (Later)
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC); //EOC means End Of Conversion
	ADC_GetConversionValue(ADC1); // Result available in ADC1->DR
	return ADC1->DR;
}


/**
* @brief Converts ADC reading to temperature in celsius
* @retval integer 
*/
float to_celsius(int v_sense)
{
	float v_sense_f = (float) v_sense;
	v_sense_f = v_sense_f * SCALE;
	return ((v_sense_f - 760.0) * AVG_SLOPE_INVERSE) + 25;
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