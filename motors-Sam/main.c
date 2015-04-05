#include <stdio.h>
//#include "arm_math.h"
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "ADC_manager.h"
#include "LED_manager.h"
#include "Temperature.h"
#include "KalmanFilter.h"
//static volatile uint_fast16_t ticks; //volatile because it changes outside of normal program flow and want to re-read. 


 TIM_OCInitTypeDef  TIM_OCInitStructure;
 #define NIGHTY_DEGREE_PULSE 480
int main (void) {
	
	GPIO_InitTypeDef GPIO_InitStructure;
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/* GPIOB clock enable*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	/* GPIOD Configuration: TIM3 CH1 (PB4) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	/* GPIO Alternate function Mode */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	/* High speed */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	/* Output type: push pull */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	/* Pull up */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Connect TIM3 pins to AF2 */  
	/* !!!!!!! Connect to PB4 !!!!!!! */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	uint16_t prescaler, period;

	/*
	 * TIM3 counter clock frequency = 315000
	 * Prescaler = (TIM3CLK / TIM3 counter clock frequency) - 1
	 * sysctemcorecolock freq = 168 MHz
	 * TIM3 clock freq = 84 MHz
	 * with TIM_period = 2100, the PWM output frequency is
	 * TIM4 counter clock frequency / TIM_period = 315000 / 6300 = 50Hz
	 */
	prescaler = (uint16_t) ((SystemCoreClock / 2) / 315000) - 1;
	period = 6300;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = period; //interrupt
	TIM_TimeBaseStructure.TIM_Prescaler = prescaler; // time clock
	/* The ration between clock frequency and the sampleing frequency, not used */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	/* upcounting counter mode, counter counts from 0 to ARR and restarts from 0 */
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	/* idenpendent PWM mode 1*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	/* output compare state enabled, ebable the comparasion between counter and puls value*/
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	/* Output polarity high, when counter < puls value, set the output polarity high*/
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	/* Set CCR1 to 0 degree at initial*/
	//*** TO BE CHANGED*****
	TIM_OCInitStructure.TIM_Pulse = NIGHTY_DEGREE_PULSE;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	
	/* Enable the prereload of CCR1 register, which controls the duty circly of PWM */
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	/* Enable the prereload of TIM2_ARR register, which defermines the frequency of PWM */
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	/* TIM2 enable counter */
	TIM_Cmd(TIM3, ENABLE);
	
	//*****Making this 480 turns the angle to 90 degree position, 180  ---> 0 degree, 480 ---> 180 degree
	TIM_SetCompare1(TIM3, 780);


}
