#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "stm32f4xx.h"         
#include "stm32f4xx_conf.h"
#include "osObjects.h" 

#define NINETY_DEGREE_PULSE 480
#define ZERO_DEGREE_PULSE 180
#define ONE_EIGHTY_DEGREE_PULSE 780


#define SERVO_GPIO GPIOB
#define LEFT_MOTOR TIM3
#define RIGHT_MOTOR TIM4

#define ALL_MOTORS GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_14



int servo_init(void);
void movePen(float x, float y);
void drawSquare(void);
void drawRectangle(void);
void drawTriangle(void);
void getAngles(double *leftAngle, double *rightAngle, float x, float y);
double radiansToDegrees(double radians);
int getPulse(int angle);