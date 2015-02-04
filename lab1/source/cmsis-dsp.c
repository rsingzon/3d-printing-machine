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