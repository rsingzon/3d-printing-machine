/*!
 * @file led.c
 * @brief Functions to control the LEDs
 * @author Ryan Singzon 260397455
 * @author Jeffrey Tichelman 260446802
 * @date February 14, 2015
 */
 
#include "init.h"
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

/**
* @brief Fades the LEDs in and out 
* @retval None
*/
void fadeLEDs()
{	
	// The fade method uses the clock to count the period
	// Core clock = 168MHz
	int period = 7500;
	int dutyCycle;
	int count = 0;
	
	// Fade in LEDs
	for (dutyCycle = 0; dutyCycle < 2500; dutyCycle++) {
		while (count < dutyCycle) {
			GPIO_SetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15); 
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
			GPIO_SetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15); 
			count++;
		}
		while (count < period){
			GPIO_ResetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
			count++;
		}
		count = 0;
	}
}