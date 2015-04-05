#include "servo.h"


int servo_init(){
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);						// TIM2 clock enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);						// TIM3 clock enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);						// TIM4 clock enable
	
	/* GPIOB clock enable*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	/* GPIO Configuration: TIM12 CH2 (PB3) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	/* Assign GPIO pin to TIM 2 alternate function */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_TIM12);
	
	/* GPIO Configuration: TIM3 CH1 (PB4) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	/* Assign GPIO pin to TIM 3 alternate function */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
	
	/* GPIO Configuration: TIM4 CH1 (PB6) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	/* Assign GPIO pin to TIM 2 alternate function */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
	
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
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	
	TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);												// Initialize TIM2 timer
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);												// Initialize TIM3 timer
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);												// Initialize TIM4 timer
	
	/* idenpendent PWM mode 1*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	/* output compare state enabled, ebable the comparasion between counter and puls value*/
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	/* Output polarity high, when counter < puls value, set the output polarity high*/
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	/* Set CCR1 to 0 degree at initial*/
	//*** TO BE CHANGED*****
	TIM_OCInitStructure.TIM_Pulse = NINETY_DEGREE_PULSE;

	TIM_OC1Init(TIM12, &TIM_OCInitStructure);															// Enable PWM on TIM2
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);															// Enable PWM on TIM3
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);															// Enable PWM on TIM4
	
	
	/* Enable the prereload of CCR1 register, which controls the duty circly of PWM */
	TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);
	/* Enable the prereload of TIM2_ARR register, which defermines the frequency of PWM */
	TIM_ARRPreloadConfig(TIM12, ENABLE);
	/* TIM2 enable counter */
	TIM_Cmd(TIM12, ENABLE);
	
	/* Enable the prereload of CCR1 register, which controls the duty circly of PWM */
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	/* Enable the prereload of TIM2_ARR register, which defermines the frequency of PWM */
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	/* TIM2 enable counter */
	TIM_Cmd(TIM3, ENABLE);
	
	/* Enable the prereload of CCR1 register, which controls the duty circly of PWM */
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	/* Enable the prereload of TIM2_ARR register, which defermines the frequency of PWM */
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	/* TIM2 enable counter */
	TIM_Cmd(TIM4, ENABLE);
	
	//*****Making this 480 turns the angle to 90 degree position, 180  ---> 0 degree, 780 ---> 180 degree
	TIM_SetCompare1(TIM12, 780);
	
	int pulse45 = getPulse(45);
	int pulse135 = getPulse(135);
	TIM_SetCompare1(LEFT_MOTOR, pulse45);
	TIM_SetCompare1(TIM4, pulse135);
	
	int i;
	while(i<1000){i++;}
	
	return 0;
}

int getPulse(int angle){
	return 180 + angle*(600/180);
}