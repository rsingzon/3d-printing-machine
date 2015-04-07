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
#define TRIANGLE		(uint8_t) 0x00
#define SQUARE			(uint8_t) 0x01
#define RECTANGLE		(uint8_t) 0x02

#define FREE_DRAW_MODE	(uint8_t) 0x03

#define UP					(uint8_t) 0x00
#define DOWN				(uint8_t) 0x01
#define LEFT				(uint8_t) 0x02
#define RIGHT				(uint8_t) 0x03

// Function prototypes
uint8_t sendShape(uint8_t);

#endif