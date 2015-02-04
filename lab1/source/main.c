/**
 * ECSE 426 - Microprocessor Systems
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "arm_math.h"
#include "kstate.h"

//typedef signed long int32_t;

void update(kalman_state* kstate, float measurement, float* destination);
int Kalmanfilter_C(float* InputArray, float* OutputArray, kalman_state* kstate, int Length);


int main()
{
	float q = 0.1;
	float r = 0.1;
		
	kalman_state kstate = {q, r, 0.0, 0.0, 0.0};
	
	float input[] = {1.5, 3.2, 2.5, 3.4, 5.3};
	float output[] = {0.0, 0.0, 0.0, 0.0, 0.0};
	
	// Call assembly implementation
	
	// Call C implementation 
	Kalmanfilter_C(input, output, &kstate, 5);
	
	/**
	 * Part III - CMSIS-DSP
	 */
	
	// Sutraction of the original data stream and the data obtain by the Kalman Filter
	
	
	return 0;
}




