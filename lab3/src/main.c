#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"

#include "init.h"
#include "accelerometer.h"
#include "kstate.h"
#include "keypad.h"
#include "7seg.h"

// Global variables
int accelerometerReady = 0; 		// Interrupt flag for accelerometer

// Function prototypes
void setAccelerometerFlag(void);
void resetAccelerometerFlag(void);
void TIM3_IRQHandler(void);

// Initialize values for display
int digit=3;
float value = 0.00;

int interruptCounter = 0;

int main(){
		
	// Enable GPIO
	initIO();
		
	// Initialize timer and enable interrupts
	initTimer();
		
	// Turn on degree symbol
	GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);
	GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
	
	initAccelerometer();
	initAccelerometerInterrupt();
	
	float angles[2];
	
	while(1){		
	
		// Wait for the accelerometer to set an interrupt
		if (accelerometerReady) {
			resetAccelerometerFlag();
			
			// Read accelerometers and set the display to the pitch
			readAcc(angles);
			value = angles[1];

		}
	}
	
	
	return 0;
}

/**
*@brief Sets the accelertometer flag when a sample can be read
*@retval None
*/
void setAccelerometerFlag(void)
{
	accelerometerReady = 1;
}

/**
*@brief Resets the accelerometer flag
*@retval None
*/
void resetAccelerometerFlag(void)
{
	accelerometerReady = 0;
}


/**
*@brief Interupt handler for EXTI0.  Informs uP that a sample is ready
*@retval None
*/
void EXTI0_IRQHandler(void)
{
	setAccelerometerFlag(); 						//Set the flag
	EXTI_ClearITPendingBit(EXTI_Line0); //Clear the EXTI0 interupt flag
}

/**
*@brief Interupt handler for TIM3, generates interrupts with configured frequency
*@retval None
*/
void TIM3_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);			// Clears incoming interrupt bit
	displayValue(value, digit);
	if(digit == 1)
		digit = 4;
	digit--;
	interruptCounter++;
}
