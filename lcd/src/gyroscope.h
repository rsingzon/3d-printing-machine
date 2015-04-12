/**
  * @file    gyroscope.h
  * @author  Ryan Singzon
  * @date    April 11, 2015
  * @brief   Header file for gyroscope control code
	*/


#ifndef __GYROSCOPE_H
#define __GYROSCOPE_H

#include "stm32f4xx_conf.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_l3gd20.h"

// Struct containing rotation values
typedef struct {
    uint16_t X;
    uint16_t Y;
    uint16_t Z;
} L3GD20_Rotation;

void initGyroscope(void);
void getRotation(L3GD20_Rotation *rotations);

#endif