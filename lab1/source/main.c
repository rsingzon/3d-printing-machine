/**
 * ECSE 426 - Microprocessor Systems
 * Singzon, Ryan			
 * Tichelman, Jeffrey
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arm_math.h"
#include "kstate.h"
#include "array.h"

// Function prototypes
void update(kalman_state* kstate, float measurement, float* destination);
int Kalmanfilter_C(float* InputArray, float* OutputArray, kalman_state *kstate, int Length);

extern int Kalmanfilter_asm(float *inputArray, float *filteredArray, kalman_state *kstate, int arraySize);

void getDifferences(float* input1, float* input2, float* output, int Length);
void getAvgStdDev(float* differences, float* average, float* std, int Length);
float getCorrelation(float* inputArray, float* filteredArray, int Length);
void getConvolution(float* inputArray, float* filteredArray, int Length, float* convolution);

int getStatistics(float *inputArray, float *filteredArray, int arraySize);


int main()
{
	float inputArray[] = {1.5, 3.2, 2.5, 3.4, 5.3};
	uint16_t arraySize = sizeof(inputArray) / sizeof(float);	
	float filteredArray[arraySize];
	
	// Initialize the Kalman state
	float q = 0.1;
	float r = 0.1;

	kalman_state kstate = {q, r, 0.0, 0.0, 0.0};		
	
	// Call assembly implementation
	Kalmanfilter_asm(inputArray, filteredArray, &kstate, arraySize);
	
	// Call C implementation 
	Kalmanfilter_C(inputArray, filteredArray, &kstate, arraySize);
	return 0;
}


	/**
	 * Part III - CMSIS-DSP
	 */
int getStatistics(float *inputArray, float *filteredArray, int arraySize)
{
	float difference[arraySize];
	float average[arraySize];
	float stdDev[arraySize];
	float correlation[arraySize];
	float convolution[arraySize];
	
	// C Implementations
	getDifferences(inputArray, filteredArray, difference, arraySize);
	getAvgStdDev(difference, average, stdDev, arraySize);
	getCorrelation(inputArray, filteredArray, arraySize);
  getConvolution(inputArray, filteredArray, arraySize, convolution);
	
	// Implementation using the CMSIS-DSP library
	float differenceCMSIS[arraySize];
	float averageCMSIS[arraySize];
	float stdDevCMSIS[arraySize];
	float correlationCMSIS[arraySize];
	float convolutionCMSIS[arraySize];
	
	arm_sub_f32(inputArray, filteredArray, differenceCMSIS, arraySize);
	arm_mean_f32(differenceCMSIS, arraySize, averageCMSIS);
	arm_std_f32(differenceCMSIS, arraySize, stdDevCMSIS);
	arm_correlate_f32(inputArray, arraySize, filteredArray, arraySize, correlationCMSIS);
	arm_conv_f32(inputArray, arraySize, filteredArray, arraySize, convolutionCMSIS);
}



