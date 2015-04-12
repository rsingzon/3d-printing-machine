#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "keypad_manager.h"
#include "seven_segmen_display.h"
#include "math.h"
/*

Pin Assignment: 

1- First transistor - PE8
2- Second transistor - PE9
6 - Third transistor - PE10


   A
 F   B
   G 
 E   C 
   D 

A 14 PC2
B 16 PE7
C 13 PA1
D 3  PA2
E 5  PC4
F 11 PA3
G 15 PC1
DP 7 PC5

*/
char nthdigit(int x, int n);
void display_controller(int number_to_display, int segment); //local function decoding numbers or characters to display segments

//Initializes GPIOS used for seven segment display
void initialize_seven_segment_display(){
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	//initialize GPIOA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//initialize GPIOC
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//initialize GPIOE
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	//temp set all to 1
	GPIO_SetBits(GPIOA, GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
	GPIO_SetBits(GPIOC, GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_5);
	GPIO_SetBits(GPIOE, GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
	

}

//Function used to display the angle a user entered, integer between 0 to 180
void display_user_angle(uint32_t angle, int segment){
	uint32_t digits_to_display[3];
	digits_to_display[0] = angle/100;
	digits_to_display[1] = (angle/10)%10;
	digits_to_display[2] = angle%10;
	
	if (segment == 1) {
		display_controller(digits_to_display[0], 1);
	}
	else if (segment == 2) {
		display_controller(digits_to_display[1], 2);
	}
	else if (segment == 2) {
		display_controller(digits_to_display[2], 3);
	}
}
//Function used to display a float measured angle according to spec format including decimal point
void display_measured_angle(float angle, int segment){
	uint32_t digits_to_display[4];
	uint32_t temp_angle; //used to store the angle in integer format
	if (angle >= 100) {
		temp_angle = (unsigned int)angle;
		digits_to_display[0] = 1;
		digits_to_display[1] = (temp_angle-100)/10; //get second digit
		digits_to_display[2] = (temp_angle-100) % 10; //get third digit
		
		if (segment == 1) {
			display_controller(digits_to_display[0], segment);
		}
		else if (segment == 2) {
			display_controller(digits_to_display[1], segment);
		}
		else if (segment == 3) {
			display_controller(digits_to_display[2], segment);
		}
	}
	else if (10 <= angle < 100) {
		//multiply angle by 10 to make it in the format XXX.YYYY
		angle = angle *10;
		//change the angle to integer format
		temp_angle = (unsigned int)angle; //cast it to an integer to get digits left of decimal point
		digits_to_display[0] = temp_angle/100; //get third digit
		digits_to_display[1] = (temp_angle%100)/10; //get second digit
		digits_to_display[2] = (temp_angle%100)%10; //get third digit
		
		if (segment == 1) {
			display_controller(digits_to_display[0], segment);
		}
		else if (segment == 2) {
			display_controller(digits_to_display[1], segment);
			display_controller(10, segment); //display dot
		}
		else if (segment == 3) {
			display_controller(digits_to_display[2], segment);
		}
	}
	else if (0 <= angle <10) {
		//multiply angle by 100 to make it in the format XXX.YYYY
		angle = angle * 100;
		temp_angle = (int)angle; //cast it to an integer to get digits left of decimal point
		digits_to_display[0] = temp_angle/100; //get third digit
		digits_to_display[1] = (temp_angle%100)/10; //get second digit
		digits_to_display[2] = (temp_angle%100)%10; //get first digit
		if (segment == 1) {
			display_controller(digits_to_display[0], segment);
			display_controller(10, segment); //display dot
		}
		else if (segment == 2) {
			display_controller(digits_to_display[1], segment);
			
		}
		else if (segment == 3) {
			display_controller(digits_to_display[2], segment);
		}
	}
	
}
//Displays the word up
void display_up(){
	static uint8_t letter = 0;
	if (letter == 0){
		display_controller(11,1);
	}
	if (letter == 1) {
		display_controller(12,2);
	}
	letter = !letter;
}

//Displays the word dn
void display_down(){
	static uint8_t letter = 0;
	if (letter == 0){
		display_controller(13,1);
	}
	if (letter == 1) {
		display_controller(14,2);
	}
	letter = !letter;
}

void display_controller(int number_to_display, int segment){
	
		if (segment == 1){
		GPIO_WriteBit(GPIOE, GPIO_Pin_8, Bit_SET);
		GPIO_WriteBit(GPIOE, GPIO_Pin_9, Bit_RESET);
		GPIO_WriteBit(GPIOE, GPIO_Pin_10, Bit_RESET);	
	}
	else if (segment == 2) {
		GPIO_WriteBit(GPIOE, GPIO_Pin_8, Bit_RESET);
		GPIO_WriteBit(GPIOE, GPIO_Pin_9, Bit_SET);
		GPIO_WriteBit(GPIOE, GPIO_Pin_10, Bit_RESET);	
	}
	else if (segment == 3) {
		GPIO_WriteBit(GPIOE, GPIO_Pin_8, Bit_RESET);
		GPIO_WriteBit(GPIOE, GPIO_Pin_9, Bit_RESET);
		GPIO_WriteBit(GPIOE, GPIO_Pin_10, Bit_SET);	
	}
	
	if (number_to_display == 0){
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_SET); 
		GPIO_WriteBit(GPIOE, GPIO_Pin_7, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);	
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_SET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET);
		GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_SET);
	}
	
	else if (number_to_display == 1) {
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET); 
		GPIO_WriteBit(GPIOE, GPIO_Pin_7, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);	
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_RESET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET);
		GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_SET);
	}
	
	else if (number_to_display == 2) {
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_SET); 
		GPIO_WriteBit(GPIOE, GPIO_Pin_7, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);	
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_RESET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET);
		GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_SET);
	}
	
	else if (number_to_display == 3) {
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_SET); 
		GPIO_WriteBit(GPIOE, GPIO_Pin_7, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);	
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_RESET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET);
		GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_SET);
	}
	
	else if (number_to_display == 4) {
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET); 
		GPIO_WriteBit(GPIOE, GPIO_Pin_7, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);	
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_SET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET);
		GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_SET);
	}
	
	else if (number_to_display == 5) {
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_SET); 
		GPIO_WriteBit(GPIOE, GPIO_Pin_7, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);	
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_SET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET);
		GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_SET);
	}
	else if (number_to_display == 6) {
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET); 
		GPIO_WriteBit(GPIOE, GPIO_Pin_7, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);	
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_SET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET);
		GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_SET);
	}
	
	else if (number_to_display == 7) {
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_SET); 
		GPIO_WriteBit(GPIOE, GPIO_Pin_7, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);	
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_RESET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET);		
		GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_SET);
	}
	
	else if (number_to_display == 8) {
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_SET); 
		GPIO_WriteBit(GPIOE, GPIO_Pin_7, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);	
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_SET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET);
		GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_SET);
	}
	
	else if (number_to_display == 9) {
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_SET); 
		GPIO_WriteBit(GPIOE, GPIO_Pin_7, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);	
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_SET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET);	//dot
		GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_SET);
	}
	else if (number_to_display == 10){
		
		GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_SET);	
		GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_SET);
	}
	
	else if (number_to_display == 11){
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET); 
		GPIO_WriteBit(GPIOE, GPIO_Pin_7, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);	
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_SET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET);
		GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_RESET);
	}
	else if (number_to_display == 12){
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_SET); 
		GPIO_WriteBit(GPIOE, GPIO_Pin_7, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);	
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_SET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET);
		GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_RESET);
	}

	else if (number_to_display == 13){
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET); 
		GPIO_WriteBit(GPIOE, GPIO_Pin_7, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);	
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_RESET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET);
		GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_RESET);
	}
	else if (number_to_display == 14){
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_SET); 
		GPIO_WriteBit(GPIOE, GPIO_Pin_7, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);	
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_SET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET);
		GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_RESET);
	}
}

void turn_OFF_Display(){
		GPIO_WriteBit(GPIOE, GPIO_Pin_8, Bit_RESET);
		GPIO_WriteBit(GPIOE, GPIO_Pin_9, Bit_RESET);
		GPIO_WriteBit(GPIOE, GPIO_Pin_10, Bit_RESET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET); 
		GPIO_WriteBit(GPIOE, GPIO_Pin_7, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);	
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_RESET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);	
		GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET);
		GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_RESET);
}

char nthdigit(int x, int n){
    while (n--) {
        x /= 10;
    }
    return (x % 10) + '0';
}

