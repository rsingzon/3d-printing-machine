#ifndef	 ADC_manager_H
#define ADC_manager_H

#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

void configureADC(void);
uint16_t getADCValue(void);

extern uint16_t ADC_output;
extern uint32_t temperatureInCelsius;
#endif
