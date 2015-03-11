/**
  * @file    accelerometer.c
  * @author  Ryan Singzon, Jeffrey Tichelman
  * @date    March 11, 2015
  * @brief   Provides functions to calibrate and convert accelerometer angles
	*/

#include "accelerometer.h"

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
