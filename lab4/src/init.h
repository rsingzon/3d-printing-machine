/**
  * @file    init.h
  * @author  Ryan Singzon, Jeffrey Tichelman
  * @date    February 28, 2015
  * @brief   Header file for initialization code
	*/

#ifndef __INIT_H
#define __INIT_H

#include <math.h>
#include "lis3dsh.h"

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

// Function prototypes
void initAccelerometer();
void initAccelerometerInterrupt();
void initIO();
void initADC();
void initTimer();

#endif