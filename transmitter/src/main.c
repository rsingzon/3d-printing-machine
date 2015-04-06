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

	statusByte = CC2500_Reset();	
	
	// Initialize register values
	CC2500_Init_Registers();
	CC2500_Read_Registers();

	uint8_t readByte;
	uint8_t numBytes = 1;
		
	statusByte = CC2500_Start_Transmit();
	
	while((statusByte & 0xF0) != TRANSMITTING){
		statusByte = CC2500_No_Op();
		printf("Status: %02x\n", statusByte);
	}
	
	uint8_t bytesAvailable;
	
	// Continuously read data from the buffer
	while(1){
		
		// Check that the receiver is in the receiving state
		while((statusByte & 0xF0) == 0x10){
			statusByte = CC2500_Read(&bytesAvailable, BYTES_AVAILABLE_REG, 2);
			
			// If data is available, print it
			if(bytesAvailable > 0){
				statusByte = CC2500_Read(&readByte, 0xBF, 1);
				printf("Data: %02x\n", readByte);
			}
			
			statusByte = CC2500_No_Op();
		}	
		
		// Put the receiver back in receiving state
		statusByte = CC2500_Start_Receive();
		
		// Wait for the receiver to enter the correct state
		while((statusByte & 0xF0) != 0x10){
			statusByte = CC2500_No_Op();
			//printf("Status: %02x\n", statusByte);
		}
		
	}
}
