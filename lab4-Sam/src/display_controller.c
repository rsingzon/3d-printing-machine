#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "seven_segmen_display.h"
#include "cmsis_os.h"

const float OFFSET = 2.5;

//This function controls the display of the 7 segment display
//based on the user angle and measured angle relationship
//1. If measured angle is greater than target display dn
//2. If measured angle is smaller than target display up
//3. If measured angle is within range of OFFSET display measured angle
void control_display(float measured_angle, uint32_t user_angle, uint8_t flicker){
	static uint8_t segment = 1;
	static uint8_t was_in_range = 0;
	if (measured_angle > user_angle + OFFSET) {
		/*
		if (was_in_range) {
			segment = 1;
			was_in_range = 0;
			
		}*/
		//display_measured_angle(measured_angle, segment);
		
		display_down();
	}
	else if (measured_angle < user_angle - OFFSET) {
		/*
		if (was_in_range) {
			segment = 1;
			was_in_range = 0;
		
		}*/
		//display_measured_angle(measured_angle, segment);
	
		display_up();
	}
	else if (measured_angle < user_angle + OFFSET || measured_angle > user_angle - OFFSET) {
		/*
		if (!was_in_range) {
			segment = 1;
			was_in_range = 1;
		}*/
		//display_user_angle(user_angle, segment);
		display_measured_angle(measured_angle, segment);
	
	}
	segment++;
	if (segment == 4) {
		segment = 1;
		if (flicker == 1){
			//osWait(10);
		}
	}
}