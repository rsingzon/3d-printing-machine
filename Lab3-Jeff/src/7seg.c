#include "7seg.h"

void displayValue(float value, int digit) {
	uint32_t digitOn;
	int i, decimalPlace, intValue, displValue;
	
	// Find where decimal place should go based on value
	if(value<10){
		decimalPlace=1;
		value*=100;
	}
	else if (value<100){
		decimalPlace =2;
		value*=10;
	}
	// Don't display a decimal place
	else{
		decimalPlace=-1;
	}
	
	
	intValue = value;
	
	if (digit==1){
		digitOn = select_1;
	}
	else if (digit==2){
		digitOn = select_2;
	}
	else{
		digitOn = select_3;
	}
	
//	else {
//		digitOn = select_4;
//	}
	
// Select appropriate digit of value
	for(i=3; i>digit;i--){
		intValue = intValue / 10;
	}
	intValue = intValue % 10;
	
	displaySegments(intValue);
	
	// Display decimalPlace if on correct digit
	if(digit==decimalPlace){
		GPIO_WriteBit(GPIOC, seg_dp, Bit_SET);
	}
	else{
		GPIO_WriteBit(GPIOC, seg_dp, Bit_RESET);
	}
	
	// Turn off all digits
	GPIO_WriteBit(GPIOB, all_select, Bit_SET);
	
	// Turn on selected digit
	GPIO_WriteBit(GPIOB, digitOn, Bit_RESET);
}

void displaySegments(int number){
	switch (number) {
        case 0:
            GPIO_WriteBit (GPIOB, seg_a, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_b, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_c, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_d, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_e, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_f, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_g, Bit_RESET);
            break;
         
        case 1:
            GPIO_WriteBit (GPIOB, seg_a, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_b, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_c, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_d, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_e, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_f, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_g, Bit_RESET);
            break;
         
        case 2:
            GPIO_WriteBit (GPIOB, seg_a, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_b, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_c, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_d, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_e, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_f, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_g, Bit_SET);
            break;
         
        case 3:
            GPIO_WriteBit (GPIOB, seg_a, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_b, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_c, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_d, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_e, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_f, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_g, Bit_SET);
            break;
         
        case 4:
            GPIO_WriteBit (GPIOB, seg_a, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_b, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_c, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_d, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_e, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_f, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_g, Bit_SET);
            break;
         
        case 5:
            GPIO_WriteBit (GPIOB, seg_a, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_b, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_c, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_d, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_e, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_f, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_g, Bit_SET);
            break;
         
        case 6:
            GPIO_WriteBit (GPIOB, seg_a, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_b, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_c, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_d, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_e, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_f, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_g, Bit_SET);
            break;
         
        case 7:
            GPIO_WriteBit (GPIOB, seg_a, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_b, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_c, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_d, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_e, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_f, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_g, Bit_RESET);		
						break;
         
        case 8:
            GPIO_WriteBit (GPIOB, seg_a, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_b, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_c, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_d, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_e, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_f, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_g, Bit_SET);
            break;
         
        case 9:
            GPIO_WriteBit (GPIOB, seg_a, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_b, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_c, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_d, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_e, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_f, Bit_SET);
            GPIO_WriteBit (GPIOB, seg_g, Bit_SET);
            break;
         
        case -1:    //Turn off all segments
            GPIO_WriteBit (GPIOB, seg_a, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_b, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_c, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_d, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_e, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_f, Bit_RESET);
            GPIO_WriteBit (GPIOB, seg_g, Bit_RESET);
            break;
    }
}