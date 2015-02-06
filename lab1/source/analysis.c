#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int i;

void getDifferences(float* input1, float* input2, float* output, int Length)
{	
	for(i=0; i<Length; i++)
	{
		output[i]=input1[i]-input2[i];
	}
	
}

void getAvgStdDev(float* differences, float* average, float* std, int Length)
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

void getCorrelation(float* inputArray, float* filteredArray, int Length, float* correlation)
{
	int i, j, k;
	for(i=0; i<Length; i++)
	{
		correlation[i]=0;
		
		for(j=0; j<Length; j++)
		{
			k=Length - (i-j) -1;
			if((k==0)||((k>0)&&(k<Length)))
				correlation[i] = correlation[i] + inputArray[j]*filteredArray[k];
		}
	}
}

void getConvolution(float* inputArray, float* filteredArray, int Length, float* convolution)
{
	int i;
	for (i = 0; i < 2*Length-1; i++) {
    size_t kmin, kmax, k;

    convolution[i] = 0;

    kmin = (i >= Length - 1) ? i - (Length - 1) : 0;
    kmax = (i < Length - 1) ? i : Length - 1;

    for (k = kmin; k <= kmax; k++) {
      convolution[i] += inputArray[k] * filteredArray[i - k];
    }
  }
}