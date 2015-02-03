#include <stdio.h>
#include <stdlib.h>
#include "arm_math.h"
#include "kstate.h"

//typedef signed long int32_t;

void update(kalman_state* kstate, float measurement, float* destination);
int Kalmanfilter_C(float* InputArray, float* OutputArray, kalman_state* kstate, int Length);


int main()
{
	float q = 0.5;
	float r = 0.5;
	
	printf("hi");
	
	kalman_state kstate = {q, r, 0.0, 0.0, 0.0};
	
	float input[] = {1.5, 3.2, 2.5, 3.4, 5.3};
	float output[] = {0.0, 0.0, 0.0, 0.0, 0.0};
	
	
	Kalmanfilter_C(input, output, &kstate, 5);
	
	return 0;
}




