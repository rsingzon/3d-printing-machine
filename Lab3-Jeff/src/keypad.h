#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#define COL1 GPIO_Pin_0
#define COL2 GPIO_Pin_1
#define COL3 GPIO_Pin_2
#define COL4 GPIO_Pin_3
#define COLUMNS GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3

#define ROW1 GPIO_Pin_3
#define ROW2 GPIO_Pin_4
#define ROW3 GPIO_Pin_5
#define ROW4 GPIO_Pin_6
#define ROWS GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6

void read_value();