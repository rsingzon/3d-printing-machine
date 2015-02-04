#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int i;

void ComputeDifferences(float* input1, float* input2, float* output, int Length)
{	
	for(i=0; i<Length; i++)
	{
		output[i]=input1[i]-input2[i];
	}
	
}

void ComputeAvgStDev(float* differences, float* average, float* std, int Length)
{
	float avg, stdev;
	
	/* Initialize average and stdev */
	avg = 0.0;
	stdev=0.0;
	
	/* Iterate through the array and compute sum, and square sum */
	for(i=0; i<Length; i++)
	{
		avg += differences[i];
		differences[i] = differences[i]*differences[i];
		stdev += differences[i];
	}
	
	/* Compute and store the average */
	avg = avg / Length;
	memcpy(average, &avg, sizeof(float));
	
	/*Compute and store the standard deviation */
	stdev = stdev / Length;
	stdev = sqrt((double)stdev);
	memcpy(std, &stdev, sizeof(float));
}

float GetCorrelation(float* inputArray, float* filteredArray, int Length)
{
	float numerator, denominator;
	
	numerator=0.0;
	denominator=0.0;
	
	/* Compute numerator = inputArray*filteredArray and denominator = sum(inputArray^2) */
	for (i=0; i<Length; i++)
	{
		numerator += inputArray[i]*filteredArray[i];
		denominator += inputArray[i]*inputArray[i];
	}
	
	/* Return the correlation */
	return (numerator / denominator);
}

float* GetConvolution(float* inputArray, float* filteredArray, int Length)
{
	int j=0;
	
	float *convolution = (float*)malloc((2*Length)-1);
	
	for (i=0; i<Length; i++)
	{
		convolution[i]=0.0;
		for (j=0; j<=i; j++)
		{
			convolution[i] += inputArray[i-j]*filteredArray[i];
		}
	}
	
	return convolution;
}