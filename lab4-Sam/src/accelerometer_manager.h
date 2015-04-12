#ifndef ACCELEROMETER_MANAGER_H
#define ACCELEROMETER_MANAGER_H

#include "stm32f4xx.h"

void initializeAccelerometer();
void getAngles(float* allAxisAngles);
void fetchAllAxisAccelerations(int32_t* allAxisAcceleration);
void activateSingleTapDetectionInterrupt();
void activateButtonInterrupt();
void readAcceleration(int32_t* allAxisAcceleration);
void interrupt_configuration();

#endif