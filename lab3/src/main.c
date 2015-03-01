#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"

#include "init.h"

int main(){
	
	initAccelerometer();
	
	float rawAccValues[3]; 				// Raw values
	float angles[2];							// Roll and pitch in degrees
	
	while(1){
		LIS3DSH_ReadACC(rawAccValues);
		printf("X: %f\n", rawAccValues[0]);
		printf("Y: %f\n", rawAccValues[1]);
		printf("Z: %f\n\n", rawAccValues[2]);
		
		toAngles(rawAccValues, angles);
		printf("Roll: %f\n", angles[0]);
		printf("Pitch: %f\n\n", angles[1]);
	}
	
	
	return 0;
}


