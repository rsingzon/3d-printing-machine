/**
  ******************************************************************************************
  * @file    servo.h
  * @author  Jeffrey Tichelman
  * @date    April 8, 2015
  * @brief   This file contains include statements, definitions and fn prototypes to control motors
  ******************************************************************************************
	**/

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
#define BACK_MOTOR TIM12

#define ALL_MOTORS GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_14

#define MAX_X 4.0
#define MIN_X -4.0
#define MAX_Y 10.0
#define MIN_Y 5.0


void servo_init(void);
void resetPen(void);
void movePen(float x, float y);
void liftPen(void);
void lowerPen(void);
void drawSquare(void);
void drawRectangle(void);
void drawTriangle(void);
void moveUp(void);
void moveDown(void);
void moveLeft(void);
void moveRight(void);
void moveUpRight(void);
void moveDownRight(void);
void moveDownLeft(void);
void moveUpLeft(void);
void getAngles(double *leftAngle, double *rightAngle, float x, float y);
double radiansToDegrees(double radians);
int getPulse(int angle);
