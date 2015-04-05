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

// Configuration registers
#define RESET_COMMAND					0x30
#define CALIBRATE_COMMAND			0x33
#define RECEIVE_COMMAND				0x34
#define TRANSMIT_COMMAND			0x35
#define	IDLE_COMMAND					0x36
#define NO_OP_COMMAND					0x3D

#define FLUSH_RX_FIFO_COMMAND	0x3A

// Function prototypes
uint8_t CC2500_Init_Registers(void);
void CC2500_Read_Registers(void);
uint8_t CC2500_Start_Transmit(void);
uint8_t CC2500_Start_Receive(void);
uint8_t CC2500_Reset(void);
uint8_t CC2500_Get_Status(void);

uint8_t CC2500_Strobe(uint8_t command);
uint8_t CC2500_Burst(uint8_t command, uint16_t bytesToWrite);

#endif