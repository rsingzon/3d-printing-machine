/**
  ******************************************************************************
  * @file    spi.h
  * @author  Ryan Singzon
  * @date    March 28, 2015
  * @brief   This file contains definitions and prototypes for SPI configuration
  ******************************************************************************
	**/
	
#ifndef __SPI_H
#define __SPI_H

#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"

 /* PA3 = Chip select
	* PA5 = SCK	
	* PA6 = MISO
	* PA7 = MOSI
	*/
	
// Defines the maximum timeout value for waiting loops
#define CC2500_FLAG_TIMEOUT         ((uint32_t)0x1000)
	
// Flag to specify a read/write command
#define READWRITE_CMD              ((uint8_t)0x80) 
// Flag to specify a read/write command with multiple bytes
#define MULTIPLEBYTE_CMD           ((uint8_t)0x40)
// Dummy byte
#define DUMMY_BYTE                 ((uint8_t)0x00)

#define CC2500_GPIO_PORT			GPIOA
#define CC2500_GPIO_CLK				RCC_AHB1Periph_GPIOA
#define CC2500_SPI_CLK				RCC_APB2Periph_SPI1
#define CC2500_AF_SPI					GPIO_AF_SPI1

#define CC2500_SPI						SPI1
#define CC2500_SPI_SCK				GPIO_Pin_5
#define CC2500_SPI_MISO				GPIO_Pin_6
#define CC2500_SPI_MOSI				GPIO_Pin_7
#define CC2500_SPI_CS					GPIO_Pin_3

#define CC2500_SCK_SOURCE			GPIO_PinSource5
#define CC2500_MISO_SOURCE		GPIO_PinSource6
#define CC2500_MOSI_SOURCE		GPIO_PinSource7

// Sets the chip select pin to either high or low
#define CC2500_CS_LOW()       GPIO_ResetBits(CC2500_GPIO_PORT, CC2500_SPI_CS)
#define CC2500_CS_HIGH()      GPIO_SetBits(CC2500_GPIO_PORT, CC2500_SPI_CS)

// Function prototypes
static uint8_t CC2500_SendByte(uint8_t byte);
uint8_t CC2500_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);
uint8_t CC2500_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);

void init_SPI1(void);

#endif
