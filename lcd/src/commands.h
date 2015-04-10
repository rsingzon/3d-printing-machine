/**
  ******************************************************************************************
  * @file    commands.h
  * @author  Ryan Singzon
  * @date    April 6, 2015
  * @brief   This file contains functions to send commands to to the motor controller board
  ******************************************************************************************
	**/
	
#ifndef __COMMANDS_H
#define __COMMANDS_H

#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "../../receiver/src/spi.h"
#include "../../receiver/src/cc2500.h"

// Define shapes and directions
#define SQUARE						(uint8_t) 0x00
#define RECTANGLE					(uint8_t) 0x01
#define TRIANGLE					(uint8_t) 0x02

#define SHAPE_MODE				(uint8_t) 0x00
#define FREE_DRAW_MODE		(uint8_t) 0x01
#define RESET_MODE				(uint8_t) 0x02

#define UP								(uint8_t) 0x50
#define DOWN							(uint8_t) 0x51
#define LEFT							(uint8_t) 0x52
#define RIGHT							(uint8_t) 0x53

#define UP_LEFT						(uint8_t) 0x61
#define UP_RIGHT					(uint8_t) 0x62
#define DOWN_LEFT					(uint8_t) 0x63
#define DOWN_RIGHT				(uint8_t) 0x64

#define DIRECTION_BUFFER_SIZE	64

// Define commands to send to the controller board
#define SQUARE_COMMAND		(uint8_t) 0x40
#define RECTANGLE_COMMAND	(uint8_t)	0x41
#define TRIANGLE_COMMAND	(uint8_t)	0x42

#define UP_COMMAND				(uint8_t)	0x50
#define DOWN_COMMAND			(uint8_t)	0x51
#define LEFT_COMMAND			(uint8_t)	0x52
#define RIGHT_COMMAND			(uint8_t)	0x53

#define RESET_POSITION_COMMAND	(uint8_t)	0x99

// Function prototypes
uint8_t sendShape(uint8_t shape);
uint8_t sendFreeDraw(uint8_t direction);
uint8_t resetPosition(void);

#endif