#ifndef KEYPAD_MANAGER_H
#define KEYPAD_MANAGER_H

#include "stm32f4xx.h"

void clear_key_array();
void initialize_keypad();
int decode_key_pressed(int* output);
void scanning_algorithm();
int check_for_mode_change();

#endif