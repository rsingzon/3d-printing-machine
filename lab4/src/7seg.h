#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#define select_1 GPIO_Pin_0
#define select_2 GPIO_Pin_3
#define select_3 GPIO_Pin_4
#define select_4 GPIO_Pin_5

#define all_select GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5

#define seg_a GPIO_Pin_7
#define seg_b GPIO_Pin_8
#define seg_c GPIO_Pin_11
#define seg_d GPIO_Pin_12
#define seg_e GPIO_Pin_13
#define seg_f GPIO_Pin_14
#define seg_g GPIO_Pin_15

#define seg_dp GPIO_Pin_3

void displayValue(float value, int digit, int flash);
void displaySegments(int number);