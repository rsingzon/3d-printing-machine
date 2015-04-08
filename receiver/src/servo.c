#include "servo.h"


int servo_init(){
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);						// TIM2 clock enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);						// TIM3 clock enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);						// TIM4 clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);						// GPIOB clock enable
	
	/* GPIO Configuration: TIM12 CH2 (PB3) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_TIM12);				// Assign to TIM12 alternate function
	
	/* GPIO Configuration: TIM3 CH1 (PB4) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);					// Assign to TIM3 alternate function
	
	/* GPIO Configuration: TIM4 CH1 (PB6) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);					// Assign to TIM3 alternate function
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	uint16_t prescaler, period;

/*
	*	AHB1 clock frequency = 42MHz
	* SystemCoreClock = 168MHz
	*	prescaler = ((168M/2)/315000)-1 = 265.67
	* period = 6300
	* Frequency = AHB1 clock / prescaler / period = 25 Hz
*/
	prescaler = (uint16_t) ((SystemCoreClock / 2) / 315000) - 1;
	period = 6300;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = period; //interrupt
	TIM_TimeBaseStructure.TIM_Prescaler = prescaler; // time clock
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	
	TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);												// Initialize TIM2 timer
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);													// Initialize TIM3 timer
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);													// Initialize TIM4 timer
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;												// Output Compare = Independent PWM Mode
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;						// Enable output state comparison
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;								// When counter < pulse value -> output high
	TIM_OCInitStructure.TIM_Pulse = NINETY_DEGREE_PULSE;										// Load ninety degree pulse into register

	TIM_OC1Init(TIM12, &TIM_OCInitStructure);															// Enable PWM on TIM2
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);															// Enable PWM on TIM3
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);															// Enable PWM on TIM4
	
	
	// Configure motor control timers
	TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);										// Preload OC1 register, which holds duty cycle
	TIM_ARRPreloadConfig(TIM12, ENABLE);																	// Preload ARR register, which holds PWM frequency
	TIM_Cmd(TIM12, ENABLE);																								// Enable timer
	
	// Same for TIM3
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	
	// Same for TIM4
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	TIM_Cmd(TIM4, ENABLE);

	// Initialize pen to (0, 10.4);
	movePen(0, 10.4);
	
	return 0;
}

// Moves pen to (x,y)
void movePen(float x, float y){
	double leftAngle, rightAngle;
	getAngles(&leftAngle, &rightAngle, x, y);
	printf("%lf, %lf\n", leftAngle, rightAngle);
	TIM_SetCompare1(RIGHT_MOTOR, getPulse(rightAngle));
	TIM_SetCompare1(LEFT_MOTOR, getPulse(leftAngle));
}

void drawSquare(){
	movePen(0.0, 8.4);
	
	osDelay(2000);
	
	movePen(-1.0, 8.4);
	
	osDelay(2000);
	
	movePen(-1.0, 6.4);
	
	osDelay(2000);
	
	movePen(1.0, 6.4);
	
	osDelay(2000);
	
	movePen(1.0, 8.4);
	
	osDelay(2000);
	
	movePen(0, 8.4);
}

void drawRectangle(){
	movePen(0.0, 8.4);
	
	osDelay(2000);
	
	movePen(-2.0, 8.4);
	
	osDelay(2000);
	
	movePen(-2.0, 7.0 );
	
	osDelay(2000);
	
	movePen(1.2, 7.2);
	
	osDelay(2000);
	
	movePen(1.5, 8.25);
	
	osDelay(2000);
	
	movePen(0, 8.4);
	
}

void drawTriangle(){
	movePen(0.0, 8.0);
	
	osDelay(2000);
	
	movePen(-2.0, 6.0);
	
	osDelay(2000);
	
	movePen(1.2, 6.0);
	
	osDelay(2000);
	
	movePen(0.0, 8.0);
}

void getAngles(double *leftAngle, double *rightAngle, float x, float y){
	double ab, ad, ac, dx, dy, h, cx1, cx2, cy1, cy2;
	
	ab = sqrt(pow(x+1.9,2) + pow(y, 2));
	
	// If ab>ac+bc or ab<|ac-bc| circles do not intersect
	if(ab>13.5 || ab<0){
		printf("Invalid point\n");
		return;
	}
	
	// Find Left Motor Angle
	ad = (pow(ab,2) + pow(6.5,2) - pow(7,2))/(2*ab);
	h = sqrt(pow(6.5,2)-pow(ad,2));
	dx = -1.9 + ad*(x+1.9)/ab;
	dy = 0 + ad*(y)/ab;
	cx1 = dx + h*(y)/ab;
	cx2 = dx - h*(y)/ab;
	cy1 = dy - h*(x+1.9)/ab;
	cy2 = dy + h*(x+1.9)/ab;
	
	// Choose c with lesser x value
	if(cx2<cx1){
		cx1=cx2;
		cy1=cy2;
	}
	
	// Compute result and store in leftAngle
	if(cx1 < -1.9){
		*leftAngle = radiansToDegrees(atan(cy1/(abs(cx1)-1.9)));
	}
	else if (cx1 < 0){
		*leftAngle = 180 - radiansToDegrees(atan(cy1/(1.9-cx1)));
	}
	else {
		*leftAngle = 180 - radiansToDegrees(atan(cy1/(cx1+1.9)));
	}
	
	// Find Right Motor Angle
	dx = 1.9 + ad*(x-1.9)/ab;
	cx1 = dx + h*(y)/ab;
	cx2 = dx - h*(y)/ab;
	cy1 = dy - h*(x+1.9)/ab;
	cy2 = dy + h*(x+1.9)/ab;
	
	// Choose c with greater x value
	if(cx2>cx1){
		cx1=cx2;
		cy1=cy2;
	}
	
	if(cx1>1.9){
		*rightAngle = 180 - radiansToDegrees(atan(cy1/(cx1-1.9)));
	}
	else if (cx1>0){
		*rightAngle = radiansToDegrees(atan(cy1/(1.9-cx1)));
	}
	else{
		*rightAngle = radiansToDegrees(atan(cy1/(abs(cx1)+1.9)));
	}
}

double radiansToDegrees(double radians){
	return radians * (180.0 / 3.14);
}

// Converts angle in degrees to duty cycle corresponding to that pulse width
int getPulse(int angle){
	return 180 + angle*(600/180);
}