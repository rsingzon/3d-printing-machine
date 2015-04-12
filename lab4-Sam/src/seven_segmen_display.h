#ifndef SEVEN_SEGMEN_DISPLAY_H
#define SEVEN_SEGMEN_DISPLAY_H

#include "stm32f4xx.h"

void initialize_seven_segment_display();
void display_measured_angle(float angle, int segment);
void display_user_angle(uint32_t angle, int segment);
void display_up();
void display_down();
void turn_OFF_Display();

#endif