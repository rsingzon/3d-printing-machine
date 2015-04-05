#include <stdlib.h>
#include <stdio.h>
#include "stm32f4xx.h"         
#include "stm32f4xx_conf.h"

#define NINETY_DEGREE_PULSE 480
#define ZERO_DEGREE_PULSE 180
#define ONE_EIGHTY_DEGREE_PULSE 780


#define SERVO_GPIO GPIOB
#define LEFT_MOTOR TIM3
#define RIGHT_MOTOR TIM4

#define ALL_MOTORS GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_14



int servo_init(void);
int getPulse(int angle);