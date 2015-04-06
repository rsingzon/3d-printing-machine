/*----------------------------------------------------------------------------
 * ECSE 426 - Microprocessor Systems
 * @author Ryan Singzon
 * @date April 6, 2015
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                 		 	// Device header
#include "stm32f4xx_conf.h"
#include <stdio.h>

#include "../../receiver/src/spi.h"
#include "../../receiver/src/cc2500.h"

// Define flags
#define TRANSMITTER_FLAG 0x01

// Thread IDs
osThreadId transmitterThread;

// Define threads 
void transmitterThreadDef(void const *argument);

// Define thread priorities
osThreadDef(transmitterThreadDef, osPriorityAboveNormal, 1, 768);

// Timer defintions
void transmitterTimeout(void const *argument);
osTimerDef (transmitterTimerDef, transmitterTimeout);
osTimerId transmitterTimer;

/*
 * @Brief Thread to transmit instructions to the motor controller board
 */
void transmitterThreadDef(void const *argument){
	// Enable SPI1
	init_SPI1();
	
	// Variable definitions
	uint8_t statusByte;
	uint8_t readByte;
	uint8_t numBytes = 1;
	uint8_t channel = 5;

	// Reset chip and initialize registers
	statusByte = CC2500_Reset();	
	CC2500_Init_Registers();
	
	// Set the channel on which to transmit
	statusByte = CC2500_Set_Channel(&channel);
	
	// Print out the register values to ensure correctness
	CC2500_Read_Registers();	

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
		
		// Wait until the timer timesout before executing the thread
		osSignalWait(TRANSMITTER_FLAG, osWaitForever);
		
		// Check that the transmitter is in the transmitting state
		while((statusByte & 0xF0) == TRANSMITTING){
						
			// If the FIFO has space available available, transmit
			statusByte = CC2500_Read(&bytesAvailable, TX_BYTES, 2);
			
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
		

		osSignalClear(transmitterThread, TRANSMITTER_FLAG);
	}
}

/*
 * main: initialize and start the system
 */
int main (void) {
	
	osKernelInitialize();                    // initialize CMSIS-RTOS
	
	// Create a thread for the transmitter
	transmitterThread = osThreadCreate(osThread(transmitterThreadDef), NULL);
	
	// Create a timer for the transmitter
	uint32_t transmitterTimerType = 1;
	transmitterTimer = osTimerCreate (osTimer(transmitterTimerDef), osTimerPeriodic, &transmitterTimerType);	
	
	// Start timer
	// Set period to 100ms
	uint32_t transmitterDelay = 1000;
	osTimerStart (transmitterTimer, transmitterDelay); 
	
	// Start thread execution
	osKernelStart();
}


/**
* @brief Timeout function for the transmitter
* @retval None
*/
void transmitterTimeout(void const *argument){
	osSignalSet(transmitterThread, TRANSMITTER_FLAG);
}