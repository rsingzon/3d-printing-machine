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

#define RADIANS_TO_DEGREES 57.29577951308233

// Function prototypes
void initAccelerometer();
void initAccelerometerInterrupt();
void toAngles(float *accValues, float *angles);

#endif