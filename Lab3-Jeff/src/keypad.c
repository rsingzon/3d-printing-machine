#include "keypad.h"

GPIO_InitTypeDef GPIO_InitStructureD;
GPIO_InitTypeDef GPIO_InitStructureE;



float read_from_user(){
	printf("%c\n", get_debounced_value());
}

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
char get_raw_value(){
	int foundColumn=0;
	int column, row;
	GPIO_WriteBit(GPIOE, ROWS, Bit_RESET);
	while(!foundColumn){
		if(!GPIO_ReadInputDataBit(GPIOD, COL1)){
			column = 1;
			foundColumn=1;
		}
		else if(!GPIO_ReadInputDataBit(GPIOD, COL2)){
			column = 2;
			foundColumn = 1;
		}
		else if(!GPIO_ReadInputDataBit(GPIOD, COL3)){
			column = 3;
			foundColumn = 1;
		}
		else if(!GPIO_ReadInputDataBit(GPIOD, COL4)){
			column = 4;
			foundColumn = 1;
		}
	}
	flip_GPIO();
	GPIO_WriteBit(GPIOD, COLUMNS, Bit_RESET);
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

void reset_GPIO(){
		// Set Column pins of keypad
		GPIO_InitStructureD.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
		GPIO_InitStructureD.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructureD.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureD.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureD.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureD.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOD, &GPIO_InitStructureD);
		
		
		// Set Row pins of keypad
		GPIO_InitStructureE.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
		GPIO_InitStructureE.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructureE.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureE.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureE.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureE.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOE, &GPIO_InitStructureE);
}

void flip_GPIO(){
			// Set Column pins of keypad
		GPIO_InitStructureD.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
		GPIO_InitStructureD.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructureD.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureD.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureD.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureD.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOD, &GPIO_InitStructureD);
		
		
		// Set Row pins of keypad
		GPIO_InitStructureE.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
		GPIO_InitStructureE.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructureE.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureE.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureE.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureE.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOE, &GPIO_InitStructureE);
}

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

int buttonPressed(){
	if(!GPIO_ReadInputDataBit(GPIOD, COL1)){
		return 1;
	}
	else if(!GPIO_ReadInputDataBit(GPIOD, COL2)){
		return 1;
	}
	else if(!GPIO_ReadInputDataBit(GPIOD, COL3)){
		return 1;
	}
	else if(!GPIO_ReadInputDataBit(GPIOD, COL4)){
		return 1;
	}
	return 0;
}