#include "keypad.h"

void read_value(){
//	int haveDigit=0;
//	int row, column;
//	while(!haveDigit){
//		if(!GPIO_ReadInputDataBit(GPIOD, COL1)){
//			column =1;
//			haveDigit=1;
//		}
//		else if(!GPIO_ReadInputDataBit(GPIOD, COL2)){
//			column = 2;
//			haveDigit = 1;
//		}
//		else if(!GPIO_ReadInputDataBit(GPIOD, COL3)){
//			column = 3;
//			haveDigit=1;
//		}
//		else if(!GPIO_ReadInputDataBit(GPIOD, COL4)){
//			column = 4;
//			haveDigit=1;
//		}
//	}
//	printf("%d\n", column);
	int foundColumn=0;
	int column;
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
	printf("%d\n", column);
	
}

