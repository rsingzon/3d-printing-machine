#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "keypad_manager.h"

const int STAR = -1;
const int EMPTY = -2;
const int NO_KEY = -3;
const int A_KEY = -4;
const int DEBOUNCE = 5000000;
static int key_array[4];
static int is_key_pressed;

void initialize_keypad(){
	
	//enable for GPIOs in keypad
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	//initialize array holding keys
	clear_key_array();
	
}

void clear_key_array(){
	int i;
	for (i=0; i<4; i++){
		key_array[i] = EMPTY;
	}

}


//algorithm that sets rows to outputs and columns to inputs in order
//to detect column pressed and then reverses the process to detect the
//row pressed
void scanning_algorithm(){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	int col1,col2,col3,col4;
	int row1,row2,row3,row4;
	int col1_pressed, col2_pressed, col3_pressed, col4_pressed;
	int row1_pressed, row2_pressed, row3_pressed, row4_pressed;
	static int l_1_pressed, l_2_pressed,l_3_pressed, l_4_pressed,l_5_pressed, l_6_pressed,l_7_pressed, l_8_pressed,l_9_pressed,l_0_pressed,l_ent_pressed,l_a_pressed=0;
  static int key_pressed = 0;
	row1_pressed = 0;
	row2_pressed = 0;
	row3_pressed = 0;
	row4_pressed = 0;
	col1_pressed = 0;
	col2_pressed = 0;
	col3_pressed = 0;
	col4_pressed = 0;
	
	int key = NO_KEY;
	int i = 0;
	
	//if (key_pressed ==0){
	//Sets columns to outputs
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOA, GPIO_Pin_15);
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	GPIO_ResetBits(GPIOC, GPIO_Pin_8);
	GPIO_ResetBits(GPIOC, GPIO_Pin_9);

	//Sets rows to inputs
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//read rows


	while (1) {
		row3 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0);
		row1 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1);
		row4 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2);
		row2 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3);		
			
		 /* if (row1_pressed || row2_pressed || row3_pressed || row4_pressed){
			break;
			} */
			
			if (!row1){
				row1_pressed = 1;
				key_pressed = 1;
				break;
			}
			if (!row2) {
				row2_pressed = 1;
				key_pressed = 1;
				break;
			}
			if (!row3) {
				row3_pressed = 1;
				key_pressed = 1;
				break;
			}
			if (!row4) {
				row4_pressed = 1;
				key_pressed = 1;
				break;
			}
			else {
			key_pressed = 0;
			key_pressed =0;
			l_1_pressed =0;
			l_2_pressed =0;
			l_3_pressed =0;
			l_4_pressed =0;
			l_5_pressed =0;
			l_6_pressed =0;		
			l_7_pressed =0;
			l_8_pressed =0;
			l_9_pressed =0;
			l_0_pressed =0;
			l_a_pressed =0;
			l_ent_pressed =0;
			}
		}
	

		//columns as inputs
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
		GPIO_Init(GPIOC, &GPIO_InitStructure);
			
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		//rows as outputs set to 0
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		GPIO_ResetBits(GPIOD, GPIO_Pin_0);
		GPIO_ResetBits(GPIOD, GPIO_Pin_1);
		GPIO_ResetBits(GPIOD, GPIO_Pin_2);
		GPIO_ResetBits(GPIOD, GPIO_Pin_3);
	//read columns
	while (1) {
		col3 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8);
		col4 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9);
		col2 = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8);
		col1 = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15);
		
		/*if (col1_pressed || col2_pressed || col3_pressed || col4_pressed){
			break;
		}*/
		
		if (!col1){
				col1_pressed = 1;	
				break;
		}
		if (!col2) {
				col2_pressed = 1;	
				break;
		}
		if (!col3) {
				col3_pressed = 1;
				break;
		}
		if (!col4) {
				col4_pressed = 1;
				break;
		}
	}
  
	/*if (!row1_pressed && !col1_pressed){
		l_1_pressed = 0;
	}*/
	if (key_pressed == 1){
	if (row1_pressed && col1_pressed && l_1_pressed ==0) {
		key = 1;
		printf("%s \n", " 1 Pressed");
		l_1_pressed =1;
		l_2_pressed =0;
		l_3_pressed =0;
		l_4_pressed =0;
		l_5_pressed =0;
		l_6_pressed =0;		
		l_7_pressed =0;
		l_8_pressed =0;
		l_9_pressed =0;
		l_0_pressed =0;
		l_a_pressed =0;
		l_ent_pressed =0;
	}
	else if (row1_pressed && col2_pressed && l_2_pressed ==0){
		key = 2;
		printf("%s \n", "2 Pressed");
		l_1_pressed =0;
		l_2_pressed =1;
		l_3_pressed =0;
		l_4_pressed =0;
		l_5_pressed =0;
		l_6_pressed =0;		
		l_7_pressed =0;
		l_8_pressed =0;
		l_9_pressed =0;
		l_0_pressed =0;
		l_a_pressed =0;
		l_ent_pressed =0;
	}
	else if (row1_pressed && col3_pressed && l_3_pressed ==0) {
		key = 3;
		printf("%s \n", "3 Pressed");
		l_1_pressed =0;
		l_2_pressed =0;
		l_3_pressed =1;
		l_4_pressed =0;
		l_5_pressed =0;
		l_6_pressed =0;		
		l_7_pressed =0;
		l_8_pressed =0;
		l_9_pressed =0;
		l_0_pressed =0;
		l_a_pressed =0;
		l_ent_pressed =0;
	}
		else if (row1_pressed && col4_pressed && l_a_pressed ==0) {
		key = A_KEY;
		printf("%s \n", "A Pressed");
		l_1_pressed =0;
		l_2_pressed =0;
		l_3_pressed =0;
		l_4_pressed =0;
		l_5_pressed =0;
		l_6_pressed =0;		
		l_7_pressed =0;
		l_8_pressed =0;
		l_9_pressed =0;
		l_0_pressed =0;
		l_a_pressed =1;
		l_ent_pressed =0;
	}
	else if (row2_pressed && col1_pressed && l_4_pressed ==0) {
		key = 4;
		printf("%s \n", "4 Pressed");
		l_1_pressed =0;
		l_2_pressed =0;
		l_3_pressed =0;
		l_4_pressed =1;
		l_5_pressed =0;
		l_6_pressed =0;		
		l_7_pressed =0;
		l_8_pressed =0;
		l_9_pressed =0;
		l_0_pressed =0;
		l_a_pressed =0;
		l_ent_pressed =0;
	}
		else if (row2_pressed && col2_pressed && l_5_pressed ==0){
		key = 5;
		printf("%s \n", "5 Pressed");
		l_1_pressed =0;
		l_2_pressed =0;
		l_3_pressed =0;
		l_4_pressed =0;
		l_5_pressed =1;
		l_6_pressed =0;		
		l_7_pressed =0;
		l_8_pressed =0;
		l_9_pressed =0;
		l_0_pressed =0;
			l_a_pressed =0;
		l_ent_pressed =0;

	}
	else if (row2_pressed && col3_pressed && l_6_pressed ==0) {
		key = 6;
		printf("%s \n", "6 Pressed");
		l_1_pressed =0;
		l_2_pressed =0;
		l_3_pressed =0;
		l_4_pressed =0;
		l_5_pressed =0;
		l_6_pressed =1;		
		l_7_pressed =0;
		l_8_pressed =0;
		l_9_pressed =0;
		l_0_pressed =0;
		l_a_pressed =0;
		l_ent_pressed =0;

	}
	else if (row3_pressed && col1_pressed && l_7_pressed ==0) {
		key = 7;
		printf("%s \n", "7 Pressed");
		l_1_pressed =0;
		l_2_pressed =0;
		l_3_pressed =0;
		l_4_pressed =0;
		l_5_pressed =0;
		l_6_pressed =0;		
		l_7_pressed =1;
		l_8_pressed =0;
		l_9_pressed =0;
		l_0_pressed =0;
		l_a_pressed =0;
		l_ent_pressed =0;

	}
	else if (row3_pressed && col2_pressed && l_8_pressed ==0){
		key = 8;
		printf("%s \n", "8 Pressed");
		l_1_pressed =0;
		l_2_pressed =0;
		l_3_pressed =0;
		l_4_pressed =0;
		l_5_pressed =0;
		l_6_pressed =0;		
		l_7_pressed =0;
		l_8_pressed =1;
		l_9_pressed =0;
		l_0_pressed =0;
		l_a_pressed =0;
		l_ent_pressed =0;
	}
	else if (row3_pressed && col3_pressed && l_9_pressed ==0) {
		key = 9;
		printf("%s \n", "9 Pressed");
		l_1_pressed =0;
		l_2_pressed =0;
		l_3_pressed =0;
		l_4_pressed =0;
		l_5_pressed =0;
		l_6_pressed =0;		
		l_7_pressed =0;
		l_8_pressed =0;
		l_9_pressed =1;
		l_0_pressed =0;
		l_a_pressed =0;
		l_ent_pressed =0;
	}
	else if (row4_pressed && col1_pressed && l_ent_pressed ==0) {
		key = STAR;
		printf("%s \n", "ENTER Pressed");
		l_1_pressed =0;
		l_2_pressed =0;
		l_3_pressed =0;
		l_4_pressed =0;
		l_5_pressed =0;
		l_6_pressed =0;		
		l_7_pressed =0;
		l_8_pressed =0;
		l_9_pressed =0;
		l_0_pressed =0;
		l_a_pressed =0;
		l_ent_pressed =1;
	}
	else if (row4_pressed && col2_pressed && l_0_pressed ==0) {
		key = 0;
		printf("%s \n", "0 Pressed");
		l_1_pressed =0;
		l_2_pressed =0;
		l_3_pressed =0;
		l_4_pressed =0;
		l_5_pressed =0;
		l_6_pressed =0;		
		l_7_pressed =0;
		l_8_pressed =0;
		l_9_pressed =0;
		l_0_pressed =1;
		l_a_pressed =0;
		l_ent_pressed =0;
		}
	}
	
	if (key != NO_KEY){
		for (i =0; i < 4; i++){
			if (key_array[i] == EMPTY){
				key_array[i] = key;
				break;
			}
		}
	}
	
	//wait method using software counting
	//int n = 0;
	for (i = 0; i<=DEBOUNCE; i++){
		//if (n == 5000000)
			//break;
		//else 
			//n++;	
		}
}

//decodes the key pressed based on the sequence of numbers stored in the key_array buffer
int decode_key_pressed(int* output){
	int enter_index;
	int value_entered = 0;
	for (enter_index = 0; enter_index < 4; enter_index++){
		if (key_array[enter_index] == STAR){
			value_entered = 1;
			break;
		}
	}
	if (value_entered ==1){
		if (enter_index == 1) {
			*output = key_array[0];
		}
		else if (enter_index ==2){
			*output = 10* key_array[0] + key_array[1];
		}
		else if (enter_index == 3){
			*output = 100 * key_array[0] + 10 * key_array[1] + key_array[2];
		}
	}
	return value_entered;
}

int check_for_mode_change(){
	GPIO_InitTypeDef GPIO_InitStructure;
	//Set columns as inputs
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//Set row as outputs
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//set output to 0
	GPIO_ResetBits(GPIOD, GPIO_Pin_1);
	//read column
	int col4 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9);
	//reverse ie set column as output
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//Set row as input
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//set column to 0
	GPIO_ResetBits(GPIOC, GPIO_Pin_9);
	int row1 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1);
	if (row1 == 0 && col4 == 0){
		return 1;
	}
	return 0;

}

