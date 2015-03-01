#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"

#include "init.h"

// Global variables
int accelerometerReady = 0; 		// Interrupt flag for accelerometer

// Function prototypes
void setAccelerometerFlag(void);
void resetAccelerometerFlag(void);

int main(){
	
	initAccelerometer();
	initAccelerometerInterrupt();
	
	float rawAccValues[3]; 				// Raw values
	float angles[2];							// Roll and pitch in degrees
	
	while(1){
		
		// Wait for the accelerometer to set an interrupt
		if (accelerometerReady) {
			
			// Read accelerometer values
			LIS3DSH_ReadACC(rawAccValues);
			resetAccelerometerFlag();
			printf("X: %f\n", rawAccValues[0]);
			printf("Y: %f\n", rawAccValues[1]);
			printf("Z: %f\n\n", rawAccValues[2]);
		
			// Convert values to angles in degrees
			toAngles(rawAccValues, angles);
			printf("Roll: %f\n", angles[0]);
			printf("Pitch: %f\n\n", angles[1]);
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
