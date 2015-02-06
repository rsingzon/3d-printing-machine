/**
 * ECSE 426 - Microprocessor Systems
 * Singzon, Ryan
 * Tichelman, Jeffrey
 */

#include <stdio.h>
#include <stdlib.h>
#include "arm_math.h"

//typedef signed long int32_t;

// Get the difference between the input array and the filtered array
void getDifferenceCMSIS(float* inputArray, float* filteredArray, float32_t *difference, uint32_t arraySize)
{
	arm_sub_f32(inputArray, filteredArray, difference, arraySize);	
}

void getAvgStDevCMSIS(float *differences, float *average, float *stdDev, int length)
{
	// Get the average of the differences
	arm_mean_f32 (differences, length, average);
	
	// Get the standard deviation of of the differences
	arm_std_f32 (differences, length, stdDev);
}

// Get the correlation between the two arrays
void getCorrelationCMSIS(float *inputArray, float *filteredArray, float* correlation, int length)
{
	arm_correlate_f32(inputArray, length, filteredArray, length, correlation);
}

void getConvolutionCMSIS(float *inputArray, float *filteredArray, float *convolution, int length)
{
		arm_conv_f32(inputArray, length, filteredArray, length, convolution);
	
//	int convolutionLength = length * 2 - 1;
//	float32_t Ak[length];
//	float32_t Bk[length];
//	arm_status status;
//	
//  arm_cfft_radix4_instance_f32 cfft_instance;
//  arm_cfft_radix4_instance_f32 *cfft_instance_ptr = (arm_cfft_radix4_instance_f32*) &cfft_instance;
//  
//  convolutionLength = length*2 - 1;
//  
//	// Fill input with zeros
//  arm_fill_f32(0.0,  Ak, length);
//  arm_fill_f32(0.0,  Bk, length);
//  
//	/* Copy the input values to the fft input buffers */
//  arm_copy_f32(inputArray,  Ak, length/2);
//  arm_copy_f32(filteredArray,  Bk, length/2);
//  
//	/* Initialize the CFFT function to compute 64 point fft */
//  status = arm_cfft_radix4_init_f32(cfft_instance_ptr, 64, 0, 1);
//  
//	/* Transform input a[n] from time domain to frequency domain A[k] */
//  arm_cfft_radix4_f32(cfft_instance_ptr, Ak);
//  
//	/* Transform input b[n] from time domain to frequency domain B[k] */
//  arm_cfft_radix4_f32(cfft_instance_ptr, Bk);
//  
//	/* Complex Multiplication of the two input buffers in frequency domain */
//  arm_cmplx_mult_cmplx_f32(Ak, Bk, convolution, length/2);
//  
//	/* Initialize the CIFFT function to compute 64 point ifft */
//  status = arm_cfft_radix4_init_f32(cfft_instance_ptr, 64, 1, 1);
//  
//	/* Transform the multiplication output from frequency domain to time domain,
//     that gives the convolved output  */
//  arm_cfft_radix4_f32(cfft_instance_ptr, convolution);
}