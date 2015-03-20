#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#define COL GPIO_Pin_1

#define COL1 GPIO_Pin_12
#define COL2 GPIO_Pin_13
#define COL3 GPIO_Pin_14
#define COL4 GPIO_Pin_15
#define COLUMNS GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15


#define ROW1 GPIO_Pin_3
#define ROW2 GPIO_Pin_4
#define ROW3 GPIO_Pin_5
#define ROW4 GPIO_Pin_6
#define ROWS GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6


float read_from_user(void);
int get_button_pressed(void);
char get_debounced_value(void);
char get_raw_value(void);
void reset_GPIO(void);
void flip_GPIO(void);
int getValue(int column, int row);
float char_to_float(char c);
int char_to_int(char c);
int buttonPressed(void);