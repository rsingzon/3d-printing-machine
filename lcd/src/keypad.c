/*!
 * @file servo.c
 * @brief Functions to initialize and control keypad
 * @author Jeffrey Tichelman
 * @date April 9, 2015
 */


#include "keypad.h"

// Init structures for keypad interrupt
GPIO_InitTypeDef GPIO_InitStructureKeypadCol;	
GPIO_InitTypeDef GPIO_InitStructureKeypadRow;


/**
  * @brief  Initializes GPIO pins for keypad and configures interrupt on column pins
  * @retval None
  */
void initKeypad(){
	
		//enable clock for GPIOE
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		
		//enable clock for SYSCFG
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
		// Set PIN1 on keypad
		GPIO_InitStructureKeypadCol.GPIO_Pin = COLUMNS;
		GPIO_InitStructureKeypadCol.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructureKeypadCol.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureKeypadCol.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureKeypadCol.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIO_COL, &GPIO_InitStructureKeypadCol);
		
		// Set Row pins of keypad
		GPIO_InitStructureKeypadRow.GPIO_Pin = ROWS;
		GPIO_InitStructureKeypadRow.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructureKeypadRow.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureKeypadRow.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureKeypadRow.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIO_ROW, &GPIO_InitStructureKeypadRow);
		

		//configure interrupts for keypad columns
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource8);
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource11);
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource12);
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
		
		
		EXTI_InitTypeDef kbinterrupt_init;
		kbinterrupt_init.EXTI_Line = EXTI_Line8 | EXTI_Line11 | EXTI_Line12 | EXTI_Line13; 						// Use pin 8 and 11-13
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
		
		NVIC_init.NVIC_IRQChannel = EXTI9_5_IRQn;
		NVIC_Init(&NVIC_init); 														
		
		GPIO_WriteBit(GPIO_ROW, ROWS, Bit_RESET);
}

/**
  * @brief  Resolves the button currently being pressed, if there is one
  * @retval Character read if valid, 'e' otherwise
  */
char get_button_pressed(){
	if(getColumn()==-1){
		return 'e';
	}
	int col = getColumn();
	flip_GPIO();
	
	if(getRow()==-1){
		reset_GPIO();
		return 'e';
	}
	
	int row = getRow();
	
	int dbCounter=0;
	while(dbCounter<1000){dbCounter++;}
	if(getRow()!=row){
		reset_GPIO();
		return 'e';
	}
	dbCounter = 0;
	while(dbCounter<1000){
		if(!buttonPressed())
			dbCounter++;
	}
	reset_GPIO();
	
	char c = getValue(col, row);
	
	return c;
	
}

/**
  * @brief  Determines which column current button press is in
  * @retval Column number if one is pressed, -1 otherwise
  */
int getColumn(){
	if(!GPIO_ReadInputDataBit(GPIO_COL, COL1)){
			return 1;
	}
	if(!GPIO_ReadInputDataBit(GPIO_COL, COL2)){
			return 2;
	}
	if(!GPIO_ReadInputDataBit(GPIO_COL, COL3)){
		return 3;
	}
	if(!GPIO_ReadInputDataBit(GPIO_COL, COL4)){
		return 4;
	}
	else{
		return -1;
	}
}

/**
  * @brief  Determines which row current button press is in
  * @retval Row number if one is pressed, -1 otherwise
  */
int getRow(){
	if(!GPIO_ReadInputDataBit(GPIO_ROW, ROW1)){
			return 1;
	}
	if(!GPIO_ReadInputDataBit(GPIO_ROW, ROW2)){
			return 2;
	}
	if(!GPIO_ReadInputDataBit(GPIO_ROW, ROW3)){
		return 3;
	}
	if(!GPIO_ReadInputDataBit(GPIO_ROW, ROW4)){
		return 4;
	}
	else{
		return -1;
	}
	
}

/**
	* @brief  Resets GPIO to initial config: columns input and rows output
  * @retval None
  */
void reset_GPIO(){
		// Set PIN1 on keypad
		GPIO_InitStructureKeypadCol.GPIO_Pin = COLUMNS;
		GPIO_InitStructureKeypadCol.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructureKeypadCol.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureKeypadCol.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureKeypadCol.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIO_COL, &GPIO_InitStructureKeypadCol);
		
		// Set Row pins of keypad
		GPIO_InitStructureKeypadRow.GPIO_Pin = ROWS;
		GPIO_InitStructureKeypadRow.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructureKeypadRow.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureKeypadRow.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureKeypadRow.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIO_ROW, &GPIO_InitStructureKeypadRow);
	
		GPIO_WriteBit(GPIO_ROW, ROWS, Bit_RESET);
}

/**
	* @brief  Flips GPIO config: rows inpu and columns output
  * @retval None
  */
void flip_GPIO(){
		GPIO_InitStructureKeypadCol.GPIO_Pin = COLUMNS;
		GPIO_InitStructureKeypadCol.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructureKeypadCol.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureKeypadCol.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureKeypadCol.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIO_COL, &GPIO_InitStructureKeypadCol);
		
		// Set Row pins of keypad
		GPIO_InitStructureKeypadRow.GPIO_Pin = ROWS;
		GPIO_InitStructureKeypadRow.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructureKeypadRow.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureKeypadRow.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureKeypadRow.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIO_ROW, &GPIO_InitStructureKeypadRow);
	
		GPIO_WriteBit(GPIO_COL, COLUMNS, Bit_RESET);
}

/**
  * @brief  Maps column, row index pairs to corresponding button character
	* @param  column : column index of button pressed
	* @param  row : row index of column pressed
  * @retval Character of button at position (row, column)
  */
char getValue(int column, int row){
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
	return 'e';
}

/**
  * @brief  Maps numeric characters to corresponding float value and D(enter) to 10.0
	* @param  c : character to convert
  * @retval float representation if c is valid, -1.0 otherwise
  */
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

/**
  * @brief 	Determines if a button is currently being pressed
  * @retval 1 if button is being pressed, 0 otherwise
  */
int buttonPressed(){
	if(!GPIO_ReadInputDataBit(GPIO_ROW, ROW1)){
		return 1;
	}
	else if(!GPIO_ReadInputDataBit(GPIO_ROW, ROW2)){
		return 1;
	}
	else if(!GPIO_ReadInputDataBit(GPIO_ROW, ROW3)){
		return 1;
	}
	else if(!GPIO_ReadInputDataBit(GPIO_ROW, ROW4)){
		return 1;
	}
	return 0;
}
