/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                 		 	// Device header
#include "stm32f4xx_conf.h"
#include <stdio.h>

#include "../../receiver/src/spi.h"
#include "../../receiver/src/cc2500.h"

/*
 * main: initialize and start the system
 */
int main (void) {
	// Enable transmission on the CC2500
	init_SPI1();
	uint8_t statusByte;
	uint8_t readByte;
	uint8_t numBytes = 1;
	uint8_t channel = 5;

	statusByte = CC2500_Reset();	
	
	// Initialize register values
	CC2500_Init_Registers();
	CC2500_Read_Registers();	
	
	// Set the channel on which to transmit
	statusByte = CC2500_Set_Channel(&channel);

	// Wait for the transceiver to enter transmitting mode
	statusByte = CC2500_Start_Transmit();
	
	while((statusByte & 0xF0) != TRANSMITTING){
		statusByte = CC2500_No_Op();
		printf("Status: %02x\n", statusByte);
	}
	
	uint8_t bytesAvailable;
	uint8_t message = 0x7D;
	
	// Continuously write data from to the buffer
	while(1){
		
		// Check that the transmitter is in the transmitting state
		while((statusByte & 0xF0) == TRANSMITTING){
						
			// If data is available, transmit
			statusByte = CC2500_Read(&bytesAvailable, BYTES_AVAILABLE_REG, 2);
			
			if(bytesAvailable < 5){
				statusByte = CC2500_Write(&message, TX_FIFO_BYTE_ADDRESS , 1);
				printf("Data sent: %02x\n", message);
			}
			
			statusByte = CC2500_No_Op();
		}	
		
		// Put the receiver back in receiving state
		statusByte = CC2500_Start_Transmit();
		
		// Wait for the receiver to enter the correct state
		while((statusByte & 0xF0) != TRANSMITTING){
			statusByte = CC2500_No_Op();
		}
	}
}
