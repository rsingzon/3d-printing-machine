#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"

#include "init.h"

#define DEBUG 1

// Global variables
int accelerometerReady = 0; 		// Interrupt flag for accelerometer

// Function prototypes
void setAccelerometerFlag(void);
void resetAccelerometerFlag(void);

int main(){
		
	initAccelerometer();
	initAccelerometerInterrupt();
	
	float rawAccValues[3]; 				// Raw values
	float adjustedAccValues[3];		// Calibrated values
	float angles[2];							// Roll and pitch in degrees
	
	int count = 0;
	while(1){
	//while(count < 250){
		
	
		// Wait for the accelerometer to set an interrupt
		if (accelerometerReady) {
			
			// Read accelerometer values
			LIS3DSH_ReadACC(rawAccValues);
			adjustAccValues(rawAccValues, adjustedAccValues);
			resetAccelerometerFlag();
			
			if (DEBUG) {
				printf("Raw values\n");
				printf("X: %f\n", rawAccValues[0]);
				printf("Y: %f\n", rawAccValues[1]);
				printf("Z: %f\n\n", rawAccValues[2]);
		
				printf("Adjusted values\n");
				printf("X: %f\n", adjustedAccValues[0]);
				printf("Y: %f\n", adjustedAccValues[1]);
				printf("Z: %f\n\n", adjustedAccValues[2]);
				
				// Convert values to angles in degrees
				toAngles(rawAccValues, angles);
				printf("Roll: %f\n", angles[0]);
				printf("Pitch: %f\n\n", angles[1]);
			}
			
			count++;		
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
