#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"

#include "init.h"
#include "7seg.h"
#include "keypad.h"

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
	
	//initAccelerometer();
	//initAccelerometerInterrupt();
	
//	float rawAccValues[3]; 				// Raw values
//	float angles[2];							// Roll and pitch in degrees
//	
//	while(1){
//		
//		// Wait for the accelerometer to set an interrupt
//		if (accelerometerReady) {
//			
//			// Read accelerometer values
//			LIS3DSH_ReadACC(rawAccValues);
//			resetAccelerometerFlag();
//			printf("X: %f\n", rawAccValues[0]);
//			printf("Y: %f\n", rawAccValues[1]);
//			printf("Z: %f\n\n", rawAccValues[2]);
//		
//			// Convert values to angles in degrees
//			toAngles(rawAccValues, angles);
//			printf("Roll: %f\n", angles[0]);
//			printf("Pitch: %f\n\n", angles[1]);
//		}
//	}

		// Enable GPIO
		initIO();
		
		// Initialize timer and enable interrupts
		initTimer();
		
		// Turn on degree symbol
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
	
		while(1) {
//			if(interruptCounter==5000){
//				value = 69.9;
//			}
//			else if(interruptCounter==10000){
//				value = 10.0;
//			}
//			else if(interruptCounter==25000){
//				value = 999;
//				interruptCounter=0;
//			}
			
			read_value();
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

