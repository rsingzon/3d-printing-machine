#ifndef DISPLAY_CONTROLLER_H
#define DISPLAY_CONTROLLER_H

#include "stm32f4xx.h"

void control_display(float measured_angle, uint32_t user_angle, uint8_t flicker);

#endif