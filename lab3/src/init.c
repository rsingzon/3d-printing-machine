/**
  * @file    init.c
  * @author  Ryan Singzon, Jeffrey Tichelman
  * @date    February 28, 2015
  * @brief   Initializes peripheral devices
	*/

#include "init.h"

void initAccelerometer() 
{
	
	// Configure accelerometer parameters
	LIS3DSH_InitTypeDef accelerometer_init;
	
	accelerometer_init.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_100;				/* Power down or /active mode with output data rate 3.125 / 6.25 / 12.5 / 25 / 50 / 100 / 400 / 800 / 1600 HZ */
	accelerometer_init.Axes_Enable = LIS3DSH_X_ENABLE | LIS3DSH_Y_ENABLE | LIS3DSH_Z_ENABLE;		/* Enable all axes */
  accelerometer_init.Continous_Update = LIS3DSH_ContinousUpdate_Enabled;			/* Block or update Low/High registers of data until all data is read */
	accelerometer_init.AA_Filter_BW = LIS3DSH_AA_BW_50;													/* Choose anti-aliasing filter BW 800 / 400 / 200 / 50 Hz*/
  accelerometer_init.Full_Scale = LIS3DSH_FULLSCALE_2;       									/* Full scale 2 / 4 / 6 / 8 / 16 g */
  accelerometer_init.Self_Test = LIS3DSH_SELFTEST_NORMAL;    									/* Self test */
	
	// Initialize accelerometer
	LIS3DSH_Init(&accelerometer_init);
	
	// Enable interrupts for accelerometer
	LIS3DSH_DRYInterruptConfigTypeDef interrupt_init;
	
	interrupt_init.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;   /* Enable/Disable data ready interrupt */
  interrupt_init.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;      /* Interrupt Signal Active Low / Active High */
  interrupt_init.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED;            /* Interrupt type as latched or pulsed */ 
	
	// Initialize interrupts
	LIS3DSH_DataReadyInterruptConfig(&interrupt_init);
}

void initAccelerometerInterrupt(void)
{
	// Enable the clock for the external interrupt line (EXTI)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	// Select the pin from which interrupts should be read
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);	
	
	// Initialization struct
	EXTI_InitTypeDef interrupt_init;

	interrupt_init.EXTI_Line = EXTI_Line0; 							// Use EXTI Line 0 
  interrupt_init.EXTI_Mode = EXTI_Mode_Interrupt;     // Set the EXTI mode to interrupt
  interrupt_init.EXTI_Trigger = EXTI_Trigger_Rising; 	// Set the trigger to rising edge
  interrupt_init.EXTI_LineCmd = ENABLE;     					// Enable the EXTI line    

	// Enable interrupts
	EXTI_Init(&interrupt_init);
	
	//Enable the NVIC 
	NVIC_InitTypeDef NVIC_init; 
	
	NVIC_init.NVIC_IRQChannel = EXTI0_IRQn; 					//Use EXTI Line 0
	NVIC_init.NVIC_IRQChannelPreemptionPriority = 0; 	//Set preemption priority
	NVIC_init.NVIC_IRQChannelSubPriority = 1; 				//Set sub prioirity
	NVIC_init.NVIC_IRQChannelCmd = ENABLE; 						//Enable NVIC
	
	NVIC_Init(&NVIC_init); 														//Configure the NVIC for use with EXTI
}


void toAngles(float *accValues, float *angles)
{
    float x = accValues[0];
    float y = accValues[1];
    float z = accValues[2];
    
    float roll = 0; 
    float pitch = 0;
    
    roll = y*y + z*z; 							// Square denominator
    roll = sqrtf(roll); 						// Square root denominator
    roll = x / roll; 								// Divide by numerator
    roll = atanf(roll); 						// Take arctan
    
    roll = (float)(roll * RADIANS_TO_DEGREES); 	//Convert to degrees and return as float
    
    pitch = x*x + z*z; 							// Square denominator
    pitch = sqrtf(pitch); 					// Square root denominator
    pitch = y / pitch; 							// Divide by numerator
    pitch = atanf(pitch); 					// Take arctan
    
    pitch = (float)(pitch * RADIANS_TO_DEGREES);
    
    // Save angles in output array
    angles[0] = roll;
    angles[1] = pitch;
}
