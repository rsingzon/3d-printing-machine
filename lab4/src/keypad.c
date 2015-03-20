
#include "keypad.h"

GPIO_InitTypeDef GPIO_InitStructureCol;
GPIO_InitTypeDef GPIO_InitStructureE;


// This method gets the input from the user and returns a float
// with the last 3 user entered digits, once enter(D) is pressed
float read_from_user(){
	float value = 0.0;
	float current;
	int character_number=0;
	while(1){
		char c = get_debounced_value();
		while(char_to_float(c)<0) {c = get_debounced_value();}
		current = char_to_float(c);
		if(current==10.0)
			break;
		else if(character_number==0){
			value += current;
			character_number++;
		}
		else if(character_number==1){
			value *=10.0;
			value += current;
			character_number++;
		}
		else if(character_number==2){
			value *=10.0;
			value += current;
			character_number++;
		}
		else{
			int temp = (int) value;
			temp = temp % 100;
			value = (float) temp;
			value *=10.0;
			value+=current;			
		}
	}
	return value;
}

//This method returns the integer of the button if 1-4 were pressed, -1 otherwise
int get_button_pressed(){
	char c = get_debounced_value();
	printf("%c\n", c);
	return char_to_int(c);
}

// This method uses a counter to debounce rising and falling edges of a raw button press
char get_debounced_value(){
	int db_counter=0;
	char c = get_raw_value();
	while(1){
		if(get_raw_value()==c){
			if(db_counter>8000){
				break;
			}
			else{
				db_counter++;
			}
		}
		else{
			c = get_raw_value();
			db_counter = 0;
		}
	}
	db_counter=0;
	while(db_counter<1000){
		if(!buttonPressed()){
			db_counter++;
		}
		else{
			db_counter=0;
		}
	}
	return c;
}

// Polls row and columns to resolve which button is pressed
char get_raw_value(){
	int foundColumn=0;
	int column, row;
	GPIO_WriteBit(GPIOE, ROWS, Bit_RESET);
	while(!foundColumn){
		
		if(!GPIO_ReadInputDataBit(GPIOE, COL1)){
			column = 1;
			foundColumn=1;
		}
		else if(!GPIO_ReadInputDataBit(GPIOE, COL2)){
			column = 2;
			foundColumn = 1;
		}
		else if(!GPIO_ReadInputDataBit(GPIOE, COL3)){
			column = 3;
			foundColumn = 1;
		}
		else if(!GPIO_ReadInputDataBit(GPIOE, COL4)){
			column = 4;
			foundColumn = 1;
		}
	}
	flip_GPIO();
	GPIO_WriteBit(GPIOE, COLUMNS, Bit_RESET);
	if(!GPIO_ReadInputDataBit(GPIOE, ROW1)){
		row=1;
	}
	else if(!GPIO_ReadInputDataBit(GPIOE, ROW2)){
		row=2;
	}
	else if(!GPIO_ReadInputDataBit(GPIOE, ROW3)){
		row=3;
	}
	else if(!GPIO_ReadInputDataBit(GPIOE, ROW4)){
		row=4;
	}
	reset_GPIO();
	return getValue(column, row);
}

// Resets Column/Row GPIO's to initial configuration
void reset_GPIO(){
		// Set Column pins of keypad
		GPIO_InitStructureCol.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_6;
		GPIO_InitStructureCol.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructureCol.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureCol.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureCol.GPIO_PuPd = GPIO_PuPd_UP;
		
		GPIO_Init(GPIOE, &GPIO_InitStructureCol);
		
		// Set Row pins of keypad
		GPIO_InitStructureE.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
		GPIO_InitStructureE.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructureE.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureE.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureE.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureE.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOE, &GPIO_InitStructureE);
}

// Flip column to output and rows to input
void flip_GPIO(){
			// Set Column pins of keypad
		GPIO_InitStructureCol.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_6;
		GPIO_InitStructureCol.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructureCol.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureCol.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureCol.GPIO_PuPd = GPIO_PuPd_UP;
		
		GPIO_Init(GPIOE, &GPIO_InitStructureCol);
		
		// Set Row pins of keypad
		GPIO_InitStructureE.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
		GPIO_InitStructureE.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructureE.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureE.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureE.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureE.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOE, &GPIO_InitStructureE);
}

// Maps row, column combinations to character corresponding to that key
int getValue(int column, int row){
	switch(column){
		case 1:
			switch(row){
				case 1:
					return '1';
				case 2:
					return '4';
				case 3:
					return '7';
				case 4:
					return '*';
			}
		case 2:
			switch(row){
				case 1:
					return '2';
				case 2:
					return '5';
				case 3:
					return '8';
				case 4:
					return '0';
			}
		case 3:
			switch(row){
				case 1:
					return '3';
				case 2:
					return '6';
				case 3:
					return '9';
				case 4:
					return '#';
			}
		case 4:
			switch(row){
				case 1:
					return 'A';
				case 2:
					return 'B';
				case 3:
					return 'C';
				case 4:
					return 'D';
			}	
	}
}

float char_to_float(char c){
	switch(c){
		case '0':
			return 0.0;
		case '1':
			return 1.0;
		case '2':
			return 2.0;
		case '3':
			return 3.0;
		case '4':
			return 4.0;
		case '5':
			return 5.0;
		case '6':
			return 6.0;
		case '7':
			return 7.0;
		case '8':
			return 8.0;
		case '9':
			return 9.0;
		case 'D':
			return 10.0;
		default:
			return -1.0;
	}
}

//this method maps keys 1-4 to corresponding integers, -1 for other keys
int char_to_int(char c){
		switch(c){
			case '1':
				return 1;
			case '2':
				return 2;
			case '3':
				return 3;
			case '4':
				return 4;
			default:
				return -1;
		}
	
}

int buttonPressed(){
	if(!GPIO_ReadInputDataBit(GPIOE, COL1)){
		return 1;
	}
	else if(!GPIO_ReadInputDataBit(GPIOE, COL2)){
		return 1;
	}
	else if(!GPIO_ReadInputDataBit(GPIOE, COL3)){
		return 1;
	}
	else if(!GPIO_ReadInputDataBit(GPIOE, COL4)){
		return 1;
	}
	return 0;
}