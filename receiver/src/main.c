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
#include "servo.h"
#include "../../lcd/src/commands.h"

#define RECEIVER_FLAG 0x01

uint32_t receiverDelay;
uint32_t leftMotorDelay;
uint32_t rightMotorDelay;

uint8_t receiver_enable = 1;

void decode(uint8_t argument);

// Thread prototypes
void receiverThreadDef(void const *argument);

// Thread declarations
osThreadDef(receiverThreadDef, osPriorityNormal, 1, 0);
osThreadId receiverThread;

// Timer declarations
void receiverCallback(void const *argument);
osTimerDef (receiverDef, receiverCallback);
osTimerId receiverTimer;


/*
 * @Brief Thread to receive instructions from the LCD board
 */
void receiverThreadDef(void const *argument){
	// Enable transmission on the CC2500
	init_SPI1();
	uint8_t statusByte;
	uint8_t readByte;
	uint8_t readCommand[8];
	uint8_t numBytes = 1;
	uint8_t channel = CHANNEL;
	
	statusByte = CC2500_Reset();	
	
	// Initialize register values and set channel
	CC2500_Init_Registers();
	statusByte = CC2500_Set_Channel(&channel);
	//CC2500_Read_Registers();
		
	// Wait for the receiver to enter receive mode
	statusByte = CC2500_Start_Receive();
	while((statusByte & 0xF0) != RECEIVING){
		statusByte = CC2500_No_Op();
		printf("Status: %02x\n", statusByte);
	}
	
	uint8_t bytesAvailable;
	
	// Continuously read data from the buffer
	while(1){
		
		if(receiver_enable){
			// Check that the receiver is in the receiving state
			while((statusByte & 0xF0) == RECEIVING){
				
				osSignalWait(RECEIVER_FLAG, osWaitForever);
				statusByte = CC2500_Read(&bytesAvailable, 0x7B, 1);
				
				printf("Bytes available: %d\n", bytesAvailable);
				
				// If data is available, print it
				if(bytesAvailable > 0){
					statusByte = CC2500_Read(&readByte, RX_FIFO_BYTE_ADDRESS, 1);
					printf("Data: %02x\n", readByte);
					
					//statusByte = CC2500_Read(readCommand, RX_FIFO_BURST_ADDRESS, 8);
					//printf("Data: %d %d %d %d %d %d %d %d\n", readCommand[0],readCommand[1],readCommand[2],readCommand[3],readCommand[4],readCommand[5],readCommand[6],readCommand[7]);
				}
				
				decode(readByte);
							
				statusByte = CC2500_No_Op();
				
				osSignalWait(RECEIVER_FLAG, osWaitForever);
			}	
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

void decode(uint8_t argument){
	switch(argument){
		case SQUARE_COMMAND:
			receiver_enable = 0;
			drawSquare();
			osDelay(1000);
			receiver_enable = 1;
			break;
		
		case RECTANGLE_COMMAND:
			receiver_enable = 0;
			drawRectangle();
			osDelay(1000);
			receiver_enable = 1;
			break;
		
		case TRIANGLE_COMMAND:
			receiver_enable = 0;
			drawTriangle();
			osDelay(1000);
			receiver_enable = 1;
			break;
		
		default:
			break;
	}
}


/*
 * main: initialize and start the system
 */

int main (void) {
	osKernelInitialize();
	
	servo_init();

	uint32_t receiverTimerType = 1;
	receiverTimer = osTimerCreate (osTimer(receiverDef), osTimerPeriodic, &receiverTimerType);
	
	receiverDelay = 100;
	osTimerStart (receiverTimer, receiverDelay);	
	
	receiverThread = osThreadCreate(osThread(receiverThreadDef), NULL);
	
	osKernelStart();


}

void receiverCallback(void const *argument){
	osSignalSet(receiverThread, RECEIVER_FLAG);
}
