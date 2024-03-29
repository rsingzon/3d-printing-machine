
#include "keypad.h"

// Init structures for keypad interrupt
GPIO_InitTypeDef GPIO_InitStructureKeypadCol;	
GPIO_InitTypeDef GPIO_InitStructureKeypadRow;


void initKeypad(){
	
		//enable clock for GPIOE
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		
		//enable clock for SYSCFG
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
		// Set PIN1 on keypad
		GPIO_InitStructureKeypadCol.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_InitStructureKeypadCol.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructureKeypadCol.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureKeypadCol.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureKeypadCol.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOE, &GPIO_InitStructureKeypadCol);
		
		// Set Row pins of keypad
		GPIO_InitStructureKeypadRow.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
		GPIO_InitStructureKeypadRow.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructureKeypadRow.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureKeypadRow.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureKeypadRow.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOE, &GPIO_InitStructureKeypadRow);
		

		//configure interrupts for keypad columns
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource12);
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource13);
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource14);
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource15);
		
		
		EXTI_InitTypeDef kbinterrupt_init;
		kbinterrupt_init.EXTI_Line = EXTI_Line12 | EXTI_Line13 | EXTI_Line14 | EXTI_Line15; 						// Use pins 12-15
		kbinterrupt_init.EXTI_Mode = EXTI_Mode_Interrupt;     // Set the EXTI mode to interrupt
		kbinterrupt_init.EXTI_Trigger = EXTI_Trigger_Falling; 	// Set the trigger to rising edge
		kbinterrupt_init.EXTI_LineCmd = ENABLE;     					// Enable the EXTI line    
		EXTI_Init(&kbinterrupt_init);
		
		//Enable the NVIC 
		NVIC_InitTypeDef NVIC_init; 
		
		NVIC_init.NVIC_IRQChannel = EXTI15_10_IRQn; 					//Use EXTI Line 12-15
		NVIC_init.NVIC_IRQChannelPreemptionPriority = 0; 	//Set preemption priority
		NVIC_init.NVIC_IRQChannelSubPriority = 1; 				//Set sub prioirity
		NVIC_init.NVIC_IRQChannelCmd = ENABLE; 						//Enable NVIC
		NVIC_Init(&NVIC_init); 														
		
		GPIO_WriteBit(GPIOE, ROWS, Bit_RESET);
}

//This method returns the integer of the button if 1-4 were pressed, -1 otherwise
int get_button_pressed(){
	if(getColumn()==-1){
		return 0;
	}
	int col = getColumn();
	flip_GPIO();
	
	if(getRow()==-1){
		reset_GPIO();
		return 0;
	}
	
	int row = getRow();
	
	int dbCounter=0;
	while(dbCounter<500){dbCounter++;}
	if(getRow()!=row){
		reset_GPIO();
		return 0;
	}
	reset_GPIO();
	
	char c = getValue(col, row);
	
	return char_to_int(c);
	
}


int getColumn(){
	if(!GPIO_ReadInputDataBit(GPIOE, COL1)){
			return 1;
	}
	if(!GPIO_ReadInputDataBit(GPIOE, COL2)){
			return 2;
	}
	if(!GPIO_ReadInputDataBit(GPIOE, COL3)){
		return 3;
	}
	if(!GPIO_ReadInputDataBit(GPIOE, COL4)){
		return 4;
	}
	else{
		return -1;
	}
}

int getRow(){
	if(!GPIO_ReadInputDataBit(GPIOE, ROW1)){
			return 1;
	}
	if(!GPIO_ReadInputDataBit(GPIOE, ROW2)){
			return 2;
	}
	if(!GPIO_ReadInputDataBit(GPIOE, ROW3)){
		return 3;
	}
	if(!GPIO_ReadInputDataBit(GPIOE, ROW4)){
		return 4;
	}
	else{
		return -1;
	}
	
}

// Resets Column/Row GPIO's to initial configuration
void reset_GPIO(){
		// Set PIN1 on keypad
		GPIO_InitStructureKeypadCol.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_InitStructureKeypadCol.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructureKeypadCol.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureKeypadCol.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureKeypadCol.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOE, &GPIO_InitStructureKeypadCol);
		
		// Set Row pins of keypad
		GPIO_InitStructureKeypadRow.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
		GPIO_InitStructureKeypadRow.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructureKeypadRow.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureKeypadRow.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureKeypadRow.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOE, &GPIO_InitStructureKeypadRow);
	
		GPIO_WriteBit(GPIOE, ROWS, Bit_RESET);
}

// Flip column to output and rows to input
void flip_GPIO(){
		GPIO_InitStructureKeypadCol.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_InitStructureKeypadCol.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructureKeypadCol.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureKeypadCol.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureKeypadCol.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOE, &GPIO_InitStructureKeypadCol);
		
		// Set Row pins of keypad
		GPIO_InitStructureKeypadRow.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
		GPIO_InitStructureKeypadRow.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructureKeypadRow.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureKeypadRow.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureKeypadRow.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOE, &GPIO_InitStructureKeypadRow);
	
		GPIO_WriteBit(GPIOE, COLUMNS, Bit_RESET);
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
			case '4':
				return 1;
			case '5':
				return 2;
			case '6':
				return 3;
			case '7':
				return 4;
			case 'D':
				return 5;
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