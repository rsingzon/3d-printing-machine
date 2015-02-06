/**
 * ECSE 426 - Microprocessor Systems
 * Singzon, Ryan				260397455
 * Tichelman, Jeffrey		260446802
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
float getCorrelation(float* inputArray, float* filteredArray, int Length, float* correlation);
void getConvolution(float* inputArray, float* filteredArray, int Length, float* convolution);

int getStatistics(float *inputArray, float *filteredArray, int arraySize);

uint16_t arraySize = sizeof(testVector) / sizeof(float);	
float filteredArray[1257];

float difference[1257];
float average[1257];
float stdDev[1257];
float correlation[1257];
float convolution[2513];
	
float differenceCMSIS[1257];
float averageCMSIS[1257];
float stdDevCMSIS[1257];
float correlationCMSIS[1257];
float convolutionCMSIS[2513];

int main()
{	
	// Initialize the Kalman state
	float q = 0.005;
	float r = 5;

	kalman_state kstate = {q, r, 0.0, 0.0, 0.0};		
	
	// Call assembly implementation
	Kalmanfilter_asm(testVector, filteredArray, &kstate, arraySize);
	int i=0;
	for (i=0;i<100;i++){
		printf("%f\n",filteredArray[i]);
	}
	// Call C implementation 
	Kalmanfilter_C(testVector, filteredArray, &kstate, arraySize);
	
	// DSP Functions
	getStatistics(testVector, filteredArray, arraySize);
	return 0;
}


	/**
	 * Part III - CMSIS-DSP
	 */
int getStatistics(float *inputArray, float *filteredArray, int arraySize)
{	
	// C Implementations
	getDifferences(inputArray, filteredArray, difference, arraySize);
	getAvgStdDev(difference, average, stdDev, arraySize);
	getCorrelation(inputArray, filteredArray, arraySize, correlation);
  getConvolution(inputArray, filteredArray, arraySize, convolution);
	
	// Implementation using the CMSIS-DSP library	
	arm_sub_f32(inputArray, filteredArray, differenceCMSIS, arraySize);
	arm_mean_f32(differenceCMSIS, arraySize, averageCMSIS);
	arm_std_f32(differenceCMSIS, arraySize, stdDevCMSIS);
	arm_correlate_f32(inputArray, arraySize, filteredArray, arraySize, correlationCMSIS);
	arm_conv_f32(inputArray, arraySize, filteredArray, arraySize, convolutionCMSIS);
	return 0;
}



