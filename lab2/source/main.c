#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"

GPIO_InitTypeDef GPIO_InitStructure;
void delay(float time);

int main(){
	
	// Start clock on GPIOD
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 

	/* Configure the LEDs on pins 12-15
	 * 12 = green/left
	 * 13 = orange/top
	 * 14 = red/right
	 * 15 = blue/bottom
	 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	while (1)
	{
		GPIO_SetBits(GPIOD,GPIO_Pin_12); // Turn on green
		delay(0xFFFFFF);
		GPIO_SetBits(GPIOD,GPIO_Pin_13); // Turn on orange
		delay(0xFFFFFF);
		GPIO_SetBits(GPIOD,GPIO_Pin_14); // Turn on red
		delay(0xFFFFFF);
		GPIO_SetBits(GPIOD,GPIO_Pin_15); // Turn on blue
		delay(0xFFFFFF);
		
		// Turn off all LEDs
		GPIO_ResetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
		delay(0xFFFFFF);
	}
	
	return 0;
}

void delay(float time)
{
	// Do nothing
	while(time--){}
}