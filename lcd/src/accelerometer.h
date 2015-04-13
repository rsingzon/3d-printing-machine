/**
  * @file    accelerometer.h
  * @author  Ryan Singzon, Jeffrey Tichelman
  * @date    March 11, 2015
  * @brief   Header file for accelerometer control code
	*/

#include <math.h>
#include "kstate.h"
#include "lis3dsh.h"

#ifndef __ACCELEROMETER_H
#define __ACCELEROMETER_H

#define RADIANS_TO_DEGREES 57.29577951308233

#define X_OFFSET 20.605
#define Y_OFFSET 16.45576
#define Z_OFFSET 13.91

// Function prototypes
void initAccelerometer(void);
void initAccelerometerInterrupt(void);
void readAcc(float *filteredCorrectedValues, kalman_state *x_state, kalman_state *y_state, kalman_state *z_state);
void adjustAccValues(float* rawValues, float *adjustedValues);
void toAngles(float *accValues, float *angles);

#endif
