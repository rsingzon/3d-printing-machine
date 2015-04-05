/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                 		 	// Device header
#include "stm32f4xx_conf.h"
#include <stdio.h>

#include "spi.h"
#include "cc2500.h"

/*
 * main: initialize and start the system
 */
int main (void) {
	init_SPI1();
	uint8_t statusByte;
//		// Enable transmission on the CC2500
	statusByte = CC2500_Reset();	
	
	// Initialize register values
	CC2500_Init_Registers();
	CC2500_Read_Registers();
	
	
	uint8_t readByte;
	uint8_t numBytes = 1;
		

	//statusByte = CC2500_Reset();
	
	//statusByte = CC2500_Write(0, FLUSH_RX_FIFO_COMMAND, 0);
	
	uint8_t buffer[64];
	
	statusByte = CC2500_Start_Receive();
	
	while((statusByte & 0xF0) != 0x10){
		statusByte = CC2500_No_Op();
		printf("%d\n", statusByte);
	}
	
	uint8_t bytesAvailable;
	while((statusByte & 0xF0) == 0x10){
		statusByte = CC2500_Read(&bytesAvailable, 0x3B, 2);
		
		if(bytesAvailable > 0){
			statusByte = CC2500_Read(&readByte, 0xBF, 1);
			printf("Data: %x\n", readByte);
		}
		
		int k;
		statusByte = CC2500_No_Op();
	}
	
	printf("exited\n");
	/*
	statusByte = CC2500_Write(0,FLUSH_RX_FIFO_COMMAND,0); 
	
	statusByte = CC2500_Start_Receive();
	
	uint8_t bytesAvailable;
	statusByte = CC2500_Read(&bytesAvailable, 0x3B, 2);
	statusByte = CC2500_Read(buffer, 0xBF, 64);
	statusByte = CC2500_Read(&readByte, 0xBF, 1);
	statusByte = CC2500_Read(&readByte, NO_OP_COMMAND, 1);
	statusByte = CC2500_Read(&readByte, NO_OP_COMMAND, 1);
	statusByte = CC2500_Read(&readByte, NO_OP_COMMAND, 1);
	statusByte = CC2500_Read(&readByte, NO_OP_COMMAND, 1);
	statusByte = CC2500_Read(&readByte, NO_OP_COMMAND, 1);
	
	statusByte = CC2500_Start_Receive();
	statusByte = CC2500_Read(&readByte, 0xBF, 1);
	statusByte = CC2500_Read(&readByte, NO_OP_COMMAND, 1);
	statusByte = CC2500_Read(&readByte, NO_OP_COMMAND, 1);
	statusByte = CC2500_Read(&readByte, NO_OP_COMMAND, 1);
	statusByte = CC2500_Read(&readByte, NO_OP_COMMAND, 1);
	
	statusByte = CC2500_Start_Receive();
	statusByte = CC2500_Read(&readByte, 0xBF, 1);
	statusByte = CC2500_Read(&readByte, NO_OP_COMMAND, 1);
	statusByte = CC2500_Read(&readByte, NO_OP_COMMAND, 1);
	statusByte = CC2500_Read(&readByte, NO_OP_COMMAND, 1);
	statusByte = CC2500_Read(&readByte, NO_OP_COMMAND, 1);
	*/
	
	/*
		
	// Read 
	statusByte = CC2500_Read(&readByte, 0xFB, 2);
	uint8_t bytesToRead = 0x7F & readByte;
	
	uint8_t receivedByte;
	uint8_t receivedString[64];
	
	statusByte = CC2500_Read(receivedString, RX_FIFO_BYTE_ADDRESS, bytesToRead);
	CC2500_Read(&readByte, 0xFB, 2);
	
	printf("Received String: %s\n", receivedString);
	*/
	/*
		while(bytesToRead > 0){
			statusByte = CC2500_Read(&receivedByte, RX_FIFO_BYTE_ADDRESS, 1);
			bytesToRead--;
			
			CC2500_Read(&readByte, 0xFB, 2);
		}
	*/
	
	// The number of byte available to read is contained in the last
	// four bits of the status byte
	// Mask the status byte
}
