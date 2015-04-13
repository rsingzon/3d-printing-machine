/**
  ******************************************************************************************
  * @file    keypad.h
  * @author  Jeffrey Tichelman
  * @date    April 9, 2015
  * @brief   This file contains include statements, definitions and fn prototypes to handle keypad
  ******************************************************************************************
	**/

#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"


#define COL1 GPIO_Pin_8
#define COL2 GPIO_Pin_11
#define COL3 GPIO_Pin_12
#define COL4 GPIO_Pin_13
#define COLUMNS GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13

#define GPIO_COL GPIOC


#define ROW1 GPIO_Pin_3
#define ROW2 GPIO_Pin_4
#define ROW3 GPIO_Pin_5
#define ROW4 GPIO_Pin_6
#define ROWS GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6

#define GPIO_ROW GPIOE


void initKeypad(void);
char get_button_pressed(void);
int getColumn(void);
int getRow(void);
void reset_GPIO(void);
void flip_GPIO(void);
char getValue(int column, int row);
float char_to_float(char c);
int buttonPressed(void);
