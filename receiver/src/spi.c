/*!
 * @file spi.c
 * @brief SPI drivers for the CC2500 wireless transceiver
 * @author Ryan Singzon 260397455
 * @author Jeffrey Tichelman 260446802
 * @date April 5, 2015
 */
 
#include "spi.h"

__IO uint32_t  CC2500Timeout = CC2500_FLAG_TIMEOUT; 

/**
* @brief Initializes the GPIOs to use SPI
* @retval None
*/
void init_SPI1(void){
	GPIO_InitTypeDef GPIO_InitStruct_A;
	SPI_InitTypeDef SPI_InitStruct;
	
	// Enable GPIO clock
	RCC_AHB1PeriphClockCmd(CC2500_GPIO_CLK, ENABLE);

	// Enable SPI clock
	RCC_APB2PeriphClockCmd(CC2500_SPI_CLK, ENABLE);
	
	// Connect SPI1 pins to SPI alternate function
	GPIO_PinAFConfig(GPIOA, CC2500_SCK_SOURCE, CC2500_AF_SPI);
	GPIO_PinAFConfig(GPIOA, CC2500_MISO_SOURCE, CC2500_AF_SPI);
	GPIO_PinAFConfig(GPIOA, CC2500_MOSI_SOURCE, CC2500_AF_SPI);
	
	/* configure pins used by SPI1
	* PA3 = Chip select
	* PA5 = SCK	
	* PA6 = MISO
	* PA7 = MOSI
	*/
	GPIO_InitStruct_A.GPIO_Pin = CC2500_SPI_SCK | CC2500_SPI_MISO | CC2500_SPI_MOSI;
	GPIO_InitStruct_A.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct_A.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct_A.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct_A.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStruct_A);	
	
	// Set chip select high
	// GPIOA->BSRRL |= CC2500_SCI_CS; 
	
	// Deinitialize previous SPI configurations
	SPI_I2S_DeInit(CC2500_SPI);
	
	// Configure SPI
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // Full duplex mode
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; 									// 8 bits per packet
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low; 												// clock is low when idle
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;											 	// Data sampled at first edge
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; // SPI frequency is APB2 frequency / 4
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;									// MSB transmitted first
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master; 											// Master mode
	SPI_Init(CC2500_SPI, &SPI_InitStruct);
	
	// Enable SPI
	SPI_Cmd(CC2500_SPI, ENABLE); 
	
		// Configure the chip select pin
	GPIO_InitStruct_A.GPIO_Pin = CC2500_SPI_CS;
	GPIO_InitStruct_A.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct_A.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct_A.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct_A.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct_A);
	
	// Set chip select to high
	GPIO_SetBits(CC2500_GPIO_PORT, CC2500_SPI_CS);
	/*
	// Enable interrupts on GPIO pins
  GPIO_InitStructure.GPIO_Pin = LIS3DSH_SPI_INT1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(LIS3DSH_SPI_INT1_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = LIS3DSH_SPI_INT2_PIN;
  GPIO_Init(LIS3DSH_SPI_INT2_GPIO_PORT, &GPIO_InitStructure);
	*/
}

static uint8_t CC2500_SendByte(uint8_t byte){
	// Continue waiting while the data register is not empty
	CC2500Timeout = CC2500_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(CC2500_SPI, SPI_I2S_FLAG_TXE) == RESET)
  {
    if((CC2500Timeout--) == 0) return 0;
  }
  
  /* Send a Byte through the SPI peripheral */
  SPI_I2S_SendData(CC2500_SPI, byte);
  
  /* Wait to receive a Byte */
  CC2500Timeout = CC2500_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(CC2500_SPI, SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((CC2500Timeout--) == 0) return 0;
  }
  
  /* Return the Byte read from the SPI bus */
  return (uint8_t)SPI_I2S_ReceiveData(CC2500_SPI);
}

/**
  * @brief  Reads a block of data from the CC2500
  * @param  pBuffer : pointer to the buffer that receives the data read from the CC2500
  * @param  ReadAddr : CC2500 internal address to read from
  * @param  NumByteToRead : number of bytes to read from the CC2500
  * @retval None
  */
uint8_t CC2500_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{  
  //if(NumByteToRead > 0x01 || ReadAddr >= 0x30)
	if(NumByteToRead > 0x01)
  {
    ReadAddr |= (uint8_t)(READWRITE_CMD | MULTIPLEBYTE_CMD);
  }
  else
  {
    ReadAddr |= (uint8_t)READWRITE_CMD;
  }
	
  /* Set chip select Low at the start of the transmission */
  CC2500_CS_LOW();
  
	CC2500Timeout = CC2500_FLAG_TIMEOUT;
	while (GPIO_ReadInputDataBit(CC2500_GPIO_PORT, CC2500_SPI_MISO)==1){
    if((CC2500Timeout--) == 0) return 0;
  }
	
  /* Send the Address of the indexed register */
  uint8_t statusByte = CC2500_SendByte(ReadAddr);
	
  
  /* Receive the data that will be read from the device (MSB First) */
  while(NumByteToRead > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock to CC2500 (Slave device) */
    *pBuffer = CC2500_SendByte(DUMMY_BYTE);
    NumByteToRead--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  CC2500_CS_HIGH();
	return statusByte;
}

/**
  * @brief  Writes one byte to the CC2500
  * @param  pBuffer : pointer to the buffer  containing the data to be written to the CC2500
  * @param  WriteAddr : CC2500's internal address to write to.
  * @param  NumByteToWrite: Number of bytes to write.
  * @retval None
  */
uint8_t CC2500_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
	uint8_t statusByte;
	
  /* Configure the MS bit: 
       - When 0, the address will remain unchanged in multiple read/write commands.
       - When 1, the address will be auto incremented in multiple read/write commands.
  */
  if(NumByteToWrite > 0x01)
  {
    WriteAddr |= (uint8_t)MULTIPLEBYTE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  CC2500_CS_LOW();
	CC2500Timeout = CC2500_FLAG_TIMEOUT;
	while (GPIO_ReadInputDataBit(CC2500_GPIO_PORT, CC2500_SPI_MISO)==1){
    if((CC2500Timeout--) == 0) return 0xFF;
  }
  
  /* Send the Address of the indexed register */
  statusByte = CC2500_SendByte(WriteAddr);
  /* Send the data that will be written into the device (MSB First) */
  while(NumByteToWrite >= 0x01)
  {
    statusByte = CC2500_SendByte(*pBuffer);
    NumByteToWrite--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  CC2500_CS_HIGH();
	
	return statusByte;
}