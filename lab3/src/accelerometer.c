/**
  * @file    accelerometer.c
  * @author  Ryan Singzon, Jeffrey Tichelman
  * @date    March 11, 2015
  * @brief   Provides functions to calibrate and convert accelerometer angles
	*/

#include "accelerometer.h"

void readAcc(float *angles){
	
	float rawAccValues[3]; 				// Raw values
	float adjustedAccValues[3];		// Calibrated values
	
	// Define kalman states for each accelerometer output
	kalman_state x_state;
	kalman_state y_state;
	kalman_state z_state;
	
	// Assign initial values for the states
	x_state.q = 0.05;
	x_state.r = 1.0;
	x_state.x = 0.0;
	x_state.p = 0.0;
	x_state.k = 0.0;
	
	y_state.q = 0.05;
	y_state.r = 1.0;
	y_state.x = 0.0;
	y_state.p = 0.0;
	y_state.k = 0.0;
	
	z_state.q = 0.05;
	z_state.r = 1.0;
	z_state.x = 0.0;
	z_state.p = 0.0;
	z_state.k = 0.0;
	
	// Read accelerometer values
	LIS3DSH_ReadACC(rawAccValues);
	adjustAccValues(rawAccValues, adjustedAccValues);
	
	// Filter X, Y, and Z measurements
	kalman_update(&x_state, adjustedAccValues[0]);
	kalman_update(&y_state, adjustedAccValues[1]);
	kalman_update(&z_state, adjustedAccValues[2]);
	
	toAngles(adjustedAccValues, angles);
	
	printf("Raw values\n");
	printf("X: %f\n", rawAccValues[0]);
	printf("Y: %f\n", rawAccValues[1]);
	printf("Z: %f\n\n", rawAccValues[2]);
		
	printf("Adjusted values\n");
	printf("X: %f\n", adjustedAccValues[0]);
	printf("Y: %f\n", adjustedAccValues[1]);
	printf("Z: %f\n\n", adjustedAccValues[2]);
				
	printf("Roll: %f\n", angles[0]);
	printf("Pitch: %f\n\n", angles[1]);
	
}

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
