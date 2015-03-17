#include <stdio.h>
#include <stdlib.h>
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


float read_from_user();
char get_debounced_value();
char get_raw_value();
void reset_GPIO();
void flip_GPIO();
int getValue(int column, int row);
float char_to_float(char c);
int buttonPressed();