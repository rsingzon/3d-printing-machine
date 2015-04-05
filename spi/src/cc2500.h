/**
  ******************************************************************************
  * @file    cc2500.h
  * @author  Jeffrey Tichelman, Ryan Singzon
  * @date    April 5, 2015
  * @brief   This file contains definitions for the CC2500 wireless transceiver
  ******************************************************************************
	**/
	
#ifndef __CC2500_H
#define __CC2500_H

#include "registers.h"

uint8_t CC2500_Start_Transmit(void);
uint8_t CC2500_Start_Receive(void);
uint8_t CC2500_Reset(void);

#define TRANSMIT_COMMAND			0x35
#define RECEIVE_COMMAND				0x34
#define RESET_COMMAND					0x30

// Function prototypes
uint8_t CC2500_Init_Registers(void);
void CC2500_Read_Registers(void);

#endif