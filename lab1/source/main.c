/**
 * ECSE 426 - Microprocessor Systems
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "arm_math.h"
#include "kstate.h"
#include <string.h>

//typedef signed long int32_t;

void update(kalman_state* kstate, float measurement, float* destination);
int Kalmanfilter_C(float* InputArray, float* OutputArray, kalman_state* kstate, int Length);
int getDifferenceCMSIS(float* inputArray, float* filteredArray, float32_t *difference, uint32_t arraySize);
void GetConvolution(float* inputArray, float* filteredArray, int Length, float* convolution);
int getConvolutionCMSIS(float *inputArray, float *filteredArray, float *convolution, int length);


int main()
{
	float q = 0.1;
	float r = 0.1;
		
	kalman_state kstate = {q, r, 0.0, 0.0, 0.0};
	
	float input[] = {1.5, 3.2, 2.5, 3.4, 5.3};
	uint16_t arraySize = sizeof(input) / sizeof(float);
	//float output[] = {0.0, 0.0, 0.0, 0.0, 0.0};
	float *output = (float*) malloc(sizeof(float) * arraySize);
	
	// Call assembly implementation
	
	// Call C implementation 
	Kalmanfilter_C(input, output, &kstate, 5);
	
	/**
	 * Part III - CMSIS-DSP
	 */
	
	// Sutraction of the original data stream and the data obtain by the Kalman Filter
	float *difference = (float*) malloc(sizeof(float) * arraySize);
	getDifferenceCMSIS(input, output, difference, arraySize);
	
	// Convolution in C
	float *convolution = (float*) malloc(sizeof(float) * arraySize);
  GetConvolution(input, output, arraySize, convolution);
	
	// Convolution using CMSIS-DSP
	int maxBlockSize = 128;
	float32_t convolution_CMSIS[maxBlockSize * 2];
	getConvolutionCMSIS(input, output, convolution, arraySize);
	
	
	free(convolution);
	free(difference);
	free(output);
	return 0;
}




