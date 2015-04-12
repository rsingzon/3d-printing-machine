#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "Temperature.h"

//Converts voltage to celcius
float convert_to_celcius(uint16_t vs) {
	float temp = ((float)vs/4095.0)*3;
	return (temp-V25)/AVG_SLOPE+25;
	
}

//Display Temperature 

float display_temperature(float temperature){
	

}