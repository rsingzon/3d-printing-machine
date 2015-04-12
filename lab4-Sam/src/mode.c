#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "keypad_manager.h"
#include "mode.h"

uint8_t get_mode(uint8_t mode){
	//clear the array that stores keys pressed
	clear_key_array();
	//scan for key pressed
	//scanning_algorithm();
	if (check_for_mode_change()){
		return !mode;
	}
	else {
		return mode;
	}
}