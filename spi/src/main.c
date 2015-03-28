/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include <stdio.h>

#include "spi.h"

void init_SPI1(void){
	GPIO_InitTypeDef GPIO_InitStruct_A;
	SPI_InitTypeDef SPI_InitStruct;
	
	// enable clock for used IO pins
	RCC_AHB1PeriphClockCmd(CC2500_GPIO_CLK, ENABLE);

	// enable peripheral clock
	RCC_APB2PeriphClockCmd(CC2500_SPI_CLK, ENABLE);
	
	/* configure pins used by SPI1
	* PA3 = Chip select
	* PA5 = SCK	
	* PA6 = MISO
	* PA7 = MOSI
	*/
	GPIO_InitStruct_A.GPIO_Pin = CC2500_SCI_SCK | CC2500_SCI_MISO | CC2500_SCI_MOSI;
	GPIO_InitStruct_A.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct_A.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct_A.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct_A.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStruct_A);
	
	// connect SPI1 pins to SPI alternate function
	GPIO_PinAFConfig(GPIOA, CC2500_SCK_SOURCE, CC2500_AF_SPI);
	GPIO_PinAFConfig(GPIOA, CC2500_MISO_SOURCE, CC2500_AF_SPI);
	GPIO_PinAFConfig(GPIOA, CC2500_MOSI_SOURCE, CC2500_AF_SPI);
	
	// Configure the chip select pin
	GPIO_InitStruct_A.GPIO_Pin = CC2500_SCI_CS;
	GPIO_InitStruct_A.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct_A.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct_A.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct_A.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct_A);
	
	// Set chip select high
	GPIOA->BSRRL |= CC2500_SCI_CS; 
	
	
	/* configure SPI1 in Mode 0
	* CPOL = 0 --> clock is low when idle
	* CPHA = 0 --> data is sampled at the first edge
	*/
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // set to full duplex mode, seperate MOSI and MISO lines
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master; // transmit in master mode, NSS pin has to be always high
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // one packet of data is 8 bits wide
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low; // clock is low when idle
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge; // data sampled at first edge
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set; // set the NSS management to internal and pull internal NSS high
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; // SPI frequency is APB2 frequency / 4
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// data is transmitted MSB first
	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_Cmd(SPI1, ENABLE); // enable SPI1
}
/* This funtion is used to transmit and receive data
* with SPI1
* data --> data to be transmitted
* returns received value
*/
uint8_t SPI1_send(uint8_t data){
	SPI1->DR = data; // write data to be transmitted to the SPI data register
	while( !(SPI1->SR & SPI_I2S_FLAG_TXE) ); // wait until transmit complete
	while( !(SPI1->SR & SPI_I2S_FLAG_RXNE) ); // wait until receive complete
	while( SPI1->SR & SPI_I2S_FLAG_BSY ); // wait until SPI is not busy anymore
	return SPI1->DR; // return received data from SPI data register
}

/*
 * main: initialize and start the system
 */
int main (void) {
	uint8_t received_val = 0;
	init_SPI1();
	while(1){
		GPIOA->BSRRH |= CC2500_SCI_CS; // set PA3 (CS) low
		SPI1_send(0xAA); // transmit data
		received_val = SPI1_send(0x00); // transmit dummy byte and receive data
		GPIOE->BSRRL |= CC2500_SCI_CS; // set PA3 (CS) high
	}
}
/*
// Read a register on the CC2500 chip
char cc2500_read_reg(char i) {

  char cc2500_status_byte = uart_spi_byte(CC2500_READ | i, SPI_START_TRANS);
  return uart_spi_byte(0x00, SPI_END_TRANS);
}

char uart_spi_byte(char b, int trans) {
  char data = 0x00;
  //only uart1 at the moment
  if (uart_port & USE_UART1){
    if(trans&SPI_START_TRANS) {
      P5OUT &= ~0x01;
    }
    uart_put_byte(b);
    while ((IFG2 & URXIFG1) == 0);
    data = RXBUF1;

    timer_wait(2);

    if(trans & SPI_END_TRANS) {
      P5OUT |= 0x01;
    }
  }

  return data;
}
*/