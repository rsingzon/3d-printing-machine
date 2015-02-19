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
	
	kalman_state kstate = {0.5, 0.5, 0.0, 0.0, 0.0};
	float temp;
	float filteredTemp;
	float referenceTemp = readADC();
	int currentLED = 'B';
	GPIO_SetBits(GPIOD,GPIO_Pin_15); // Turn on blue
	
	while(1){
		
		// Read temperature values
		temp = to_celsius(readADC());
		
		fadeLEDs();
		/*
		kalman_state kstate_1 = {2.0, 0.5, 0.0, 0.0, 0.0};
		kalman_state kstate_2 = {5.0, 0.005, 0.0, 0.0, 0.0};
		kalman_state kstate_3 = {2.0, 0.005, 0.0, 0.0, 0.0};
		kalman_state kstate_4 = {3.0, 0.005, 0.0, 0.0, 0.0};
		kalman_state kstate_5 = {4.0, 0.005, 0.0, 0.0, 0.0};
		kalman_state kstate_6 = {5.0, 0.010, 0.0, 0.0, 0.0};
		

		float filteredTemp1 = kalman_update(&kstate_1, temp);		
		float filteredTemp2 = kalman_update(&kstate_2, temp);		
		float filteredTemp3 = kalman_update(&kstate_3, temp);		
		float filteredTemp4 = kalman_update(&kstate_4, temp);		
		float filteredTemp5 = kalman_update(&kstate_5, temp);		
		float filteredTemp6 = kalman_update(&kstate_6, temp);		
		
		printf("%f,%f,%f,%f,%f,%f,%f\n",temp,filteredTemp1, filteredTemp2, filteredTemp3,  filteredTemp4, filteredTemp5, filteredTemp6);
		
		
		// Display warning if temperature exceeds 50 degrees
		if (filteredTemp >= 50.0) {
			fadeLEDs();
		}
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
		*/
		
		
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
* @brief Fades the LEDs in and out 
* @retval None
*/
void fadeLEDs()
{	
	int period = 7500;
	int dutyCycle;
	int count = 0;
	
	// Fade in LEDs
	for (dutyCycle = 0; dutyCycle < 2500; dutyCycle++) {
		while (count < dutyCycle) {
			GPIO_SetBits(GPIOD,GPIO_Pin_12); 
			GPIO_SetBits(GPIOD,GPIO_Pin_13); 
			GPIO_SetBits(GPIOD,GPIO_Pin_14); 
			GPIO_SetBits(GPIOD,GPIO_Pin_15); 
			count++;
		}
		while (count < period){
			GPIO_ResetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
			count++;
		}
		count = 0;
	}
	
	// Fade out LEDs
	for (dutyCycle = 2500; dutyCycle > 0; dutyCycle--) {
		while (count < dutyCycle) {
			GPIO_SetBits(GPIOD,GPIO_Pin_12); 
			GPIO_SetBits(GPIOD,GPIO_Pin_13); 
			GPIO_SetBits(GPIOD,GPIO_Pin_14); 
			GPIO_SetBits(GPIOD,GPIO_Pin_15); 
			count++;
		}
		while (count < period){
			GPIO_ResetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
			count++;
		}
		count = 0;
	}
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