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
#include "kstate.h"
#include "accelerometer.h"

#define RECEIVER_FLAG 0x01
#define ACC_FLAG 0x01

#define MOVE_THRESHOLD 45
#define LIFT_THRESHOLD 10


// Global Variables
uint32_t receiverDelay;
uint32_t accelerometerDelay;
uint8_t receiver_enable = 1;
float roll, pitch;
uint8_t pen_lifted = 0;
float starting_point[2] = {0, 10.4};

// Helper method prototypes
void decode(uint8_t argument);

// Thread prototypes
void receiverThreadDef(void const *argument);
void accelerometerThreadDef(void const *argument);

// Thread declarations
osThreadDef(receiverThreadDef, osPriorityNormal, 1, 0);
osThreadId receiverThread;

osThreadDef(accelerometerThreadDef, osPriorityNormal, 1, 0);
osThreadId accelerometerThread;

// Mutex declarations
osMutexDef(Mutex_Motor);				
osMutexId motor_mutex;

// Timer declarations
void receiverCallback(void const *argument);
osTimerDef (receiverDef, receiverCallback);
osTimerId receiverTimer;


/**
*@brief Thread to read accelerometer values
*/
void accelerometerThreadDef(void const *argument){
	
	float angles[2];
	// Define kalman states for each accelerometer output
	kalman_state x_state = {0.05, 0.981, 0.0, 0.0, 0.0};
	kalman_state y_state = {0.05, 0.981, 0.0, 0.0, 0.0};
	kalman_state z_state = {0.05, 0.981, 0.0, 0.0, 0.0};
	
	float reference_pitch = 0;
	float reference_roll = 0;
	
	float pitch;
	float roll;
	
	int zero_angle_count = 0;
	
	
	while(1){
		osSignalWait(ACC_FLAG, osWaitForever ); 
		readAcc(angles, &x_state, &y_state, &z_state);
		
		roll = angles[0];
		pitch = angles[1];
		
		// Lift the pen if the one of the angles changes from zero
		if((abs(pitch) > LIFT_THRESHOLD || abs(roll) > LIFT_THRESHOLD) && pen_lifted == 0){
			osMutexWait(motor_mutex, osWaitForever);
			liftPen();
			osMutexRelease(motor_mutex);
			osDelay(500);
			pen_lifted = 1;
		}
		
		// Move the pen one direction of the angle 
		if(pen_lifted == 1){
			if(pitch < -MOVE_THRESHOLD){
				osMutexWait(motor_mutex, osWaitForever);
				moveDown();
				osMutexRelease(motor_mutex);
				osDelay(1000);
			}
			else if(pitch > MOVE_THRESHOLD){
				osMutexWait(motor_mutex, osWaitForever);
				moveUp();
				osMutexRelease(motor_mutex);
				osDelay(1000);
			}
			
			if(roll < -MOVE_THRESHOLD){
				osMutexWait(motor_mutex, osWaitForever);
				moveRight();
				osMutexRelease(motor_mutex);
				osDelay(1000);
			}
			else if(roll > MOVE_THRESHOLD){
				osMutexWait(motor_mutex, osWaitForever);
				moveLeft();
				osMutexRelease(motor_mutex);
				osDelay(1000);
			}
		
			// Lower the pen once the angle stabilizes at zero
			if(zero_angle_count > 100){
				osMutexWait(motor_mutex, osWaitForever);
				lowerPen();
				osMutexRelease(motor_mutex);
				zero_angle_count=0;
				osDelay(500);
				pen_lifted = 0;
			}
			else if(abs(pitch) < LIFT_THRESHOLD && abs(roll) < LIFT_THRESHOLD){
				zero_angle_count++;
			}	
			else{
				zero_angle_count=0;
			}
			
		}
				
		osSignalClear(accelerometerThread, ACC_FLAG);
	}
}


/**
	*@brief Thread to receive instructions from LCD board
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
		//printf("Status: %02x\n", statusByte);
	}
	
	uint8_t bytesAvailable;
	
	// Continuously read data from the buffer
	while(1){
		
		if(receiver_enable){
			// Check that the receiver is in the receiving state
			while((statusByte & 0xF0) == RECEIVING){
				
				osSignalWait(RECEIVER_FLAG, osWaitForever);
				statusByte = CC2500_Read(&bytesAvailable, 0x7B, 1);
				
				//printf("Bytes available: %d\n", bytesAvailable);
				
				readByte = NULL;
				
				// If data is available, print it
				if(bytesAvailable > 0){
					statusByte = CC2500_Read(&readByte, RX_FIFO_BYTE_ADDRESS, 1);
					printf("Data: %02x\n", readByte);
				}
				
				decode(readByte);
							
				statusByte = CC2500_No_Op();
				
				osSignalClear(receiverThread, RECEIVER_FLAG);
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

/**
  * @brief  Decides what to do with received communication packet
	* @param  argument : value read from receiver
  * @retval None
  */
void decode(uint8_t argument){
	switch(argument){
		case SQUARE_COMMAND:
			osMutexWait(motor_mutex, osWaitForever);
			receiver_enable = 0;
			drawSquare();
			osMutexRelease(motor_mutex);
			osDelay(1000);
			receiver_enable = 1;
			break;
		
		case RECTANGLE_COMMAND:
			osMutexWait(motor_mutex, osWaitForever);
			receiver_enable = 0;
			drawRectangle();
			osMutexRelease(motor_mutex);
			osDelay(1000);
			receiver_enable = 1;
			break;
		
		case TRIANGLE_COMMAND:
			osMutexWait(motor_mutex, osWaitForever);
			receiver_enable = 0;
			drawTriangle();
			osMutexRelease(motor_mutex);
			osDelay(1000);
			receiver_enable = 1;
			break;
		
		case UP_COMMAND:
			osMutexWait(motor_mutex, osWaitForever);
			receiver_enable = 0;
			moveUp();
			osMutexRelease(motor_mutex);
			osDelay(1000);
			receiver_enable = 1;
			break;
		
		case DOWN_COMMAND:
			osMutexWait(motor_mutex, osWaitForever);
			receiver_enable = 0;
			moveDown();
			osMutexRelease(motor_mutex);
			osDelay(1000);
			receiver_enable = 1;
			break;
		
		case RIGHT_COMMAND:
			osMutexWait(motor_mutex, osWaitForever);
			receiver_enable = 0;
			moveRight();
			osMutexRelease(motor_mutex);
			osDelay(1000);
			receiver_enable = 1;
			break;
		
		case LEFT_COMMAND:
			osMutexWait(motor_mutex, osWaitForever);
			receiver_enable = 0;
			moveLeft();
			osMutexRelease(motor_mutex);
			osDelay(1000);
			receiver_enable = 1;
			break;
		
		case UP_LEFT:
			osMutexWait(motor_mutex, osWaitForever);
			receiver_enable = 0;
			moveUpLeft();
			osMutexRelease(motor_mutex);
			osDelay(1000);
			receiver_enable = 1;
			break;
		
		case UP_RIGHT:
			osMutexWait(motor_mutex, osWaitForever);
			receiver_enable = 0;
			moveUpRight();
			osMutexRelease(motor_mutex);
			osDelay(1000);
			receiver_enable = 1;
			break;
		
		case DOWN_LEFT:
			osMutexWait(motor_mutex, osWaitForever);
			receiver_enable = 0;
			moveDownLeft();
			osMutexRelease(motor_mutex);
			osDelay(1000);
			receiver_enable = 1;
			break;
		
		case DOWN_RIGHT:
			osMutexWait(motor_mutex, osWaitForever);
			receiver_enable = 0;
			moveDownRight();
			osMutexRelease(motor_mutex);
			osDelay(1000);
			receiver_enable = 1;
			break;
		
		case RESET_POSITION_COMMAND:
			osMutexWait(motor_mutex, osWaitForever);
			receiver_enable = 0;
			resetPen();
			osMutexRelease(motor_mutex);
			osDelay(1000);
			receiver_enable = 1;
			break;
		
		default:
			break;
	}
}

/*
 * main: initialize and start the system, in particular timers and threads
 */



int main (void) {
	osKernelInitialize();
	
	servo_init();
	
	initAccelerometer();
	initAccelerometerInterrupt();

	uint32_t receiverTimerType = 1;
	receiverTimer = osTimerCreate (osTimer(receiverDef), osTimerPeriodic, &receiverTimerType);
	
	receiverDelay = 100;
	osTimerStart (receiverTimer, receiverDelay);		
	
	motor_mutex = osMutexCreate(osMutex (Mutex_Motor));
	
	receiverThread = osThreadCreate(osThread(receiverThreadDef), NULL);
	accelerometerThread = osThreadCreate(osThread(accelerometerThreadDef), NULL);
	
	osKernelStart();
}

/**
*@brief Callback function for the reciever timer, signifies that microP should check receiver for new data
*@retval None
*/
void receiverCallback(void const *argument){
	osSignalSet(receiverThread, RECEIVER_FLAG);
}

/**
*@brief Interupt handler for EXTI0.  Informs uP that a sample is ready
*@retval None
*/
void EXTI0_IRQHandler(void)
{
	osSignalSet(accelerometerThread, ACC_FLAG);			// Set a signal for the accelerometer thread
	EXTI_ClearITPendingBit(EXTI_Line0); //Clear the EXTI0 interupt flag
}
