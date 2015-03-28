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

 /* PA3 = Chip select
	* PA5 = SCK	
	* PA6 = MISO
	* PA7 = MOSI
	*/

#define CC2500_GPIO_PORT			GPIOA
#define CC2500_GPIO_CLK				RCC_AHB1Periph_GPIOA
#define CC2500_SPI_CLK				RCC_APB2Periph_SPI1
#define CC2500_AF_SPI					GPIO_AF_SPI1

#define CC2500_SCI_SCK				GPIO_Pin_5
#define CC2500_SCI_MISO				GPIO_Pin_6
#define CC2500_SCI_MOSI				GPIO_Pin_7
#define CC2500_SCI_CS					GPIO_Pin_3

#define CC2500_SCK_SOURCE			GPIO_PinSource5
#define CC2500_MISO_SOURCE		GPIO_PinSource6
#define CC2500_MOSI_SOURCE		GPIO_PinSource7



#endif