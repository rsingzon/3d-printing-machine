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

#define LEFT_MOTOR_FLAG 0x01
#define RIGHT_MOTOR_FLAG 0x01

uint32_t leftMotorDelay;
uint32_t rightMotorDelay;

// Thread prototypes
void receiverThreadDef(void const *argument);
void leftMotorThreadDef(void const *argument);
void rightMotorThreadDef(void const *argument);

// Thread declarations
osThreadDef(receiverThreadDef, osPriorityNormal, 1, 0);
osThreadDef(leftMotorThreadDef, osPriorityNormal, 1, 0);
osThreadDef(rightMotorThreadDef, osPriorityNormal, 1, 0);


osThreadId receiverThread;
osThreadId leftMotorThread;
osThreadId rightMotorThread;

// Timer declarations
void leftMotorCallback(void const *argument);
osTimerDef (leftMotorDef, leftMotorCallback);
osTimerId leftMotorTimer;

void rightMotorCallback(void const *argument);
osTimerDef (rightMotorDef, rightMotorCallback);
osTimerId rightMotorTimer;


/*
 * @Brief Thread to receive instructions from the LCD board
 */
void receiverThreadDef(void const *argument){
	// Enable transmission on the CC2500
	init_SPI1();
	uint8_t statusByte;
	uint8_t readByte;
	uint8_t numBytes = 1;
	uint8_t channel = 5;
	
	statusByte = CC2500_Reset();	
	
	// Initialize register values
	CC2500_Init_Registers();

	// Set the channel
	statusByte = CC2500_Set_Channel(&channel);
	
	CC2500_Read_Registers();
		
	// Wait for the receiver to enter receive mode
	statusByte = CC2500_Start_Receive();
	while((statusByte & 0xF0) != RECEIVING){
		statusByte = CC2500_No_Op();
		//printf("Status: %02x\n", statusByte);
	}
	
	uint8_t bytesAvailable;
	
	// Continuously read data from the buffer
	while(1){
		
		// Check that the receiver is in the receiving state
		while((statusByte & 0xF0) == RECEIVING){
			statusByte = CC2500_Read(&bytesAvailable, RX_BYTES, 2);
			
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

// Left motor thread
static int left_angle;
void leftMotorThreadDef(void const *argument){
	while(1){
		osSignalWait(LEFT_MOTOR_FLAG, osWaitForever);
		
		TIM_SetCompare1(LEFT_MOTOR, left_angle);
		
		osSignalClear(leftMotorThread, LEFT_MOTOR_FLAG);
	}
}

// Right motor thread
static int right_angle;
void rightMotorThreadDef(void const *argument){
	while(1){
		osSignalWait(RIGHT_MOTOR_FLAG, osWaitForever);
		
		
		TIM_SetCompare1(RIGHT_MOTOR, right_angle);
		
		
		osSignalClear(rightMotorThread, RIGHT_MOTOR_FLAG);
	}
}


/*
 * main: initialize and start the system
 */
int main (void) {
//	osKernelInitialize();
//	
//	servo_init();
//	
//	
//	leftMotorThread = osThreadCreate(osThread(leftMotorThreadDef), NULL);
//	rightMotorThread = osThreadCreate(osThread(rightMotorThreadDef), NULL);
//	
//	uint32_t leftMotorTimerType = 1;
//	leftMotorTimer = osTimerCreate (osTimer(leftMotorDef), osTimerPeriodic, &leftMotorTimerType);
//	
//	leftMotorDelay = 10;
//	osTimerStart (leftMotorTimer, leftMotorDelay); 
//	
//	uint32_t rightMotorTimerType = 1;
//	rightMotorTimer = osTimerCreate (osTimer(rightMotorDef), osTimerPeriodic, &rightMotorTimerType);
//	
//	rightMotorDelay = 10;
//	osTimerStart (rightMotorTimer, rightMotorDelay); 
//	
//	receiverThread = osThreadCreate(osThread(receiverThreadDef), NULL);
//	
//	osKernelStart();


	osKernelInitialize();
	servo_init();
	
	osDelay(2000);
	
	movePen(3.0, 10.4);
	
	osDelay(2000);
	
	movePen(3.0, 7.4);
	
	osDelay(2000);
	
	movePen(-3.0, 7.4);
	
	osDelay(2000);
	
	movePen(-3.0, 10.4);
	
	osDelay(2000);
	
	movePen(0, 10.4);
}

void leftMotorCallback(void const *argument){
	osSignalSet(leftMotorThread, LEFT_MOTOR_FLAG);
}

void rightMotorCallback(void const *argument){
	osSignalSet(rightMotorThread, RIGHT_MOTOR_FLAG);
}
