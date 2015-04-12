/*!
 * @file servo.c
 * @brief Functions to initialize and control motors that move pen
 * @author Jeffrey Tichelman
 * @date April 8, 2015
 */

#include "servo.h"

static float currentX, currentY;

/**
  * @brief  Initializes timers and GPIO pins to control motors
  * @retval None
  */
void servo_init(){
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
	liftPen();
	osDelay(500);
	movePen(0, 10.4);
	lowerPen();
}

/**
  * @brief  Moves pen to position (x,y)
	* @param  x : x coordinate of destination
	* @param  y : y coordinate of destination
  * @retval None
  */
void movePen(float x, float y){
	double leftAngle, rightAngle;
	
	// Get motor angles for desired (x,y) position
	getAngles(&leftAngle, &rightAngle, x, y);
	
	// Get pulse associated with angle and send to motor
	TIM_SetCompare1(RIGHT_MOTOR, getPulse(rightAngle));
	TIM_SetCompare1(LEFT_MOTOR, getPulse(leftAngle));
	
	// Update internal state
	currentX= x;
	currentY = y;
	
}

/**
  * @brief  Lifts pen from page by moving back motor to 90 degrees
  * @retval None
  */
void liftPen(){
	TIM_SetCompare1(BACK_MOTOR, NINETY_DEGREE_PULSE);
}

/**
  * @brief  Lowers pen to page by moving back motor to 0 degrees
  * @retval None
  */
void lowerPen(){
	TIM_SetCompare1(BACK_MOTOR, ZERO_DEGREE_PULSE);
}

/**
  * @brief  Draws a square with corners at (-1.0, 8.4), (-1.0, 6.4), (1.0, 6.4), (1.0, 8.4) respectively
  * @retval None
  */
void drawSquare(){
	liftPen();
	osDelay(500);
	movePen(0.0, 8.4);
	osDelay(500);
	lowerPen();
	
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

/**
  * @brief  Draws a rectangle with corners at (-2.0, 8.4), (-2.0, 7.0 ), (1.5, 7.0), (1.5, 8.5) respectively
  * @retval None
  */
void drawRectangle(){
	liftPen();
	osDelay(500);
	movePen(0.0, 8.4);
	osDelay(500);
	lowerPen();
	
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

/**
  * @brief  Draws a triangle with corners at (0.0, 8.4), (-2.0, 6.0), and (1.5, 6.0) respectively
  * @retval None
  */
void drawTriangle(){
	liftPen();
	osDelay(500);
	movePen(0.0, 8.4);
	osDelay(500);
	lowerPen();
	
	osDelay(2000);
	
	movePen(-2.0, 6.0);
	
	osDelay(2000);
	
	movePen(1.2, 7.0);
	
	osDelay(2000);
	
	movePen(0.0, 8.0);
}

/**
  * @brief  Moves pen up 1 cm from current position
  * @retval None
  */
void moveUp(void){
	movePen(currentX, currentY+1.0);
}

/**
  * @brief  Moves pen down 1 cm from current position
  * @retval None
  */
void moveDown(void){
	movePen(currentX, currentY-1.0);
}

/**
  * @brief  Moves pen left 1 cm from current position
  * @retval None
  */
void moveLeft(void){
	movePen(currentX-1.0, currentY);
}

/**
  * @brief  Moves pen right 1 cm from current position
  * @retval None
  */
void moveRight(void){
	movePen(currentX+1.0, currentY);
}

/**
  * @brief  Moves pen up 1 cm and right 1 cm from current position
  * @retval None
  */
void moveUpRight(void){
	movePen(currentX+1.0, currentY+1.0);
}

/**
  * @brief  Moves pen down 1 cm and right 1 cm from current position
  * @retval None
  */
void moveDownRight(void){
	movePen(currentX+1.0, currentY-1.0);
}

/**
  * @brief  Moves pen down 1 cm and left 1 cm from current position
  * @retval None
  */
void moveDownLeft(void){
	movePen(currentX-1.0, currentY-1.0);
}

/**
  * @brief  Moves pen up 1 cm and left 1 cm from current position
  * @retval None
  */
void moveUpLeft(void){
	movePen(currentX-1.0, currentY+1.0);
}

/**
  * @brief  Gets motor angles required to have pen at position(x,y)
	* @param  leftAngle : pointer to location to store calculated left motor angle
	* @param  rightAngle : pointer to location to store calculated right motor angle
	* @param  x : x coordinate of destination
	* @param  y : y coordinate of destination
  * @retval None
  */
void getAngles(double *leftAngle, double *rightAngle, float x, float y){
	double ab, ad, dx, dy, h, cx1, cx2, cy1, cy2;
	
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

/**
  * @brief  Converts an angle expressed in radians to the corresponding angle in degrees
	* @param  radians : angle value expressed in terms of radians
  * @retval Double representing the same angle in degrees
  */
double radiansToDegrees(double radians){
	return radians * (180.0 / 3.14);
}

/**
  * @brief  Calculates the duty cycle pulse to send motor to certain angle
	* @param  angle : angle toturn motor to
  * @retval integer with corresponding duty cycle
	* Uses linear map with 180 corresponding to 0 degree angle and 780 to 180 degree, respectively
  */
int getPulse(int angle){
	return 180 + angle*(600/180);
}
