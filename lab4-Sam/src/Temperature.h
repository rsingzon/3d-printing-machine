#include "stm32f4xx_conf.h"
#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#define AVG_SLOPE 0.0025
#define V25 0.76

float display_temperature(float temperature);
float convert_to_celcius(uint16_t vs);
#endif