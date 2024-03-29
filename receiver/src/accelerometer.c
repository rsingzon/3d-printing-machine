/**
  * @file    accelerometer.c
  * @author  Ryan Singzon, Jeffrey Tichelman
  * @date    March 11, 2015
  * @brief   Provides functions to initialize accelerometer and calibrate and convert accelerometer angles
	*/

#include "accelerometer.h"

/**
  * @brief  Initializes on board accelerometer
  * @retval None
  */
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

/**
  * @brief  Configures an NVIC interrupt to accelerometer line
  * @retval None
  */
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

/**
  * @brief  Read accelerometer values, filter and calculate pitch and roll angle
	* @param  angles : pointer to location to store calcuated pitch and roll angles
	* @param  x_state : current filter state of acc's x value
	* @param  y_state : current filter state of acc's y value
	* @param  z_state : current filter state of acc's z value
  * @retval None
  */
void readAcc(float *angles, kalman_state *x_state, kalman_state *y_state, kalman_state *z_state){
	
	float rawAccValues[3]; 				// Raw values
	float adjustedAccValues[3];		// Calibrated values
		
	// Read accelerometer values
	LIS3DSH_ReadACC(rawAccValues);
	adjustAccValues(rawAccValues, adjustedAccValues);
	
	// Filter X, Y, and Z measurements
	kalman_update(x_state, adjustedAccValues[0]);
	kalman_update(y_state, adjustedAccValues[1]);
	kalman_update(z_state, adjustedAccValues[2]);
	
	toAngles(adjustedAccValues, angles);
	
//	
//	printf("Raw values\n");
//	printf("X: %f\n", rawAccValues[0]);
//	printf("Y: %f\n", rawAccValues[1]);
//	printf("Z: %f\n\n", rawAccValues[2]);
//		
//	printf("Adjusted values\n");
//	printf("X: %f\n", adjustedAccValues[0]);
//	printf("Y: %f\n", adjustedAccValues[1]);
//	printf("Z: %f\n\n", adjustedAccValues[2]);
//				
//	printf("Roll: %f\n", angles[0]);
//	printf("Pitch: %f\n\n", angles[1]);
//	
	
}

/**
  * @brief  Adjusts accelerometer values based on measured bias
	* @param  rawValues : pointer to location of raw accelerometer readings
	* @param  adjustedValues : pointer to location to store adjusted reading
  * @retval None
  */
void adjustAccValues(float *rawValues, float *adjustedValues){
		
		// Add or subtract the offset to each of the values for the axes
		// X Value
		if (rawValues[0] < 1000) {
				adjustedValues[0] = rawValues[0] + X_OFFSET;
		} else if (rawValues[0] > 1000){
				adjustedValues[0] = rawValues[0] - X_OFFSET;
		}
	
		// Y Value
		if (rawValues[1] < 1000) {
				adjustedValues[1] = rawValues[1] + Y_OFFSET;
		} else if (rawValues[1] > 1000){
				adjustedValues[1] = rawValues[1] - Y_OFFSET;
		}
		
		// Z Value
		if (rawValues[2] < 1000) {
				adjustedValues[2] = rawValues[2] + Z_OFFSET;
		} else if (rawValues[2] > 1000){
				adjustedValues[2] = rawValues[2] - Z_OFFSET;
		}
}

/**
  * @brief  Calculates roll and pitch angles from accelerometer readings
	* @param  accValues : pointer to values given by accelerometer
	* @param  angles : pointer to location to store calculated roll and pitch angles
  * @retval None
  */
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
