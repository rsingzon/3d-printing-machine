#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "Tim_Interrupt.h"

//configures the hardware timer
void TIM3_Configuration(void){
	
		NVIC_InitTypeDef nvicStructure;
	
    nvicStructure.NVIC_IRQChannel = TIM3_IRQn; //select TIM3 as interrupt request channel
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 1; //lower priotity than accelerometer
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
	
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
		
    NVIC_Init(&nvicStructure);
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseInitTypeDef TIMInitStructure;
		//4000 * (168Mhz)^-1 * period = 0.02 ie create event at a rate of 50 Hz
		// prescaler = 4000, period = 840
    TIMInitStructure.TIM_Prescaler = 4000;
    TIMInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIMInitStructure.TIM_Period = 840;
    TIMInitStructure.TIM_ClockDivision = 0;
	
    TIM_TimeBaseInit(TIM3, &TIMInitStructure);
	
	  /* TIM Interrupts enable */
    TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);
	
    TIM_Cmd(TIM3, ENABLE);
	

	
}