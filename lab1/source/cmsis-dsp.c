/**
 * ECSE 426 - Microprocessor Systems
 * Singzon, Ryan
 * Tichelman, Jeffrey
 */

#include <stdio.h>
#include <stdlib.h>
#include "arm_math.h"

//typedef signed long int32_t;

int getDifferenceCMSIS(float* inputArray, float* filteredArray, float32_t *difference, uint32_t arraySize)
{
	
	arm_sub_f32(inputArray, filteredArray, difference, arraySize);
	
	return 0;
	
}


int getConvolutionCMSIS(float *inputArray, float *filteredArray, float *convolution, int length)
{
	
	int maxBlockSize = 128;
	int convolutionLength = length * 2 - 1;
	float32_t Ak[maxBlockSize];
	float32_t Bk[maxBlockSize];
	arm_status status;
	
	/* CFFT Structure instance */
  arm_cfft_radix4_instance_f32 cfft_instance;
  /* CFFT Structure instance pointer */
  arm_cfft_radix4_instance_f32 *cfft_instance_ptr =
      (arm_cfft_radix4_instance_f32*) &cfft_instance;
  
	/* output length of convolution */
  convolutionLength = length*2 - 1;
  
	/* Initialise the fft input buffers with all zeros */
  arm_fill_f32(0.0,  Ak, maxBlockSize);
  arm_fill_f32(0.0,  Bk, maxBlockSize);
  
	/* Copy the input values to the fft input buffers */
  arm_copy_f32(inputArray,  Ak, maxBlockSize/2);
  arm_copy_f32(filteredArray,  Bk, maxBlockSize/2);
  
	/* Initialize the CFFT function to compute 64 point fft */
  status = arm_cfft_radix4_init_f32(cfft_instance_ptr, 64, 0, 1);
  
	/* Transform input a[n] from time domain to frequency domain A[k] */
  arm_cfft_radix4_f32(cfft_instance_ptr, Ak);
  
	/* Transform input b[n] from time domain to frequency domain B[k] */
  arm_cfft_radix4_f32(cfft_instance_ptr, Bk);
  
	/* Complex Multiplication of the two input buffers in frequency domain */
  arm_cmplx_mult_cmplx_f32(Ak, Bk, convolution, maxBlockSize/2);
  
	/* Initialize the CIFFT function to compute 64 point ifft */
  status = arm_cfft_radix4_init_f32(cfft_instance_ptr, 64, 1, 1);
  
	/* Transform the multiplication output from frequency domain to time domain,
     that gives the convolved output  */
  arm_cfft_radix4_f32(cfft_instance_ptr, convolution);
}