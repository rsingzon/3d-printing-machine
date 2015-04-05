#include <stdio.h>
//#include "arm_math.h"

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"

#include "servo.h"

#define LEFT_MOTOR_FLAG 0x01
#define RIGHT_MOTOR_FLAG 0x01

uint32_t leftMotorDelay;
uint32_t rightMotorDelay;

//static volatile uint_fast16_t ticks; //volatile because it changes outside of normal program flow and want to re-read. 

// Timer definitions
void leftMotorCallback(void const *argument);
osTimerDef (leftMotorDef, leftMotorCallback);
osTimerId leftMotorTimer;

void rightMotorCallback(void const *argument);
osTimerDef (rightMotorDef, rightMotorCallback);
osTimerId rightMotorTimer;

// Thread definitions
osThreadId leftMotorThread;
void leftMotorThreadDef(void const *argument);
osThreadDef(leftMotorThreadDef, osPriorityAboveNormal, 1, 1024);

osThreadId rightMotorThread;
void rightMotorThreadDef(void const *argument);
osThreadDef(rightMotorThreadDef, osPriorityAboveNormal, 1, 1024);

// Left motor thread
static int left_angle=0;
void leftMotorThreadDef(void const *argument){
	while(1){
		osSignalWait(LEFT_MOTOR_FLAG, osWaitForever);
		
		if(left_angle<135){
			TIM_SetCompare1(LEFT_MOTOR, getPulse(45)+left_angle);
			left_angle++;
		}
		
		osSignalClear(leftMotorThread, LEFT_MOTOR_FLAG);
	}
}

// Right motor thread
static int right_angle=0;
void rightMotorThreadDef(void const *argument){
	while(1){
		osSignalWait(RIGHT_MOTOR_FLAG, osWaitForever);
		
		if(right_angle<45){
			TIM_SetCompare1(RIGHT_MOTOR, getPulse(135)+right_angle);
			right_angle++;
		}
		
		
		
		osSignalClear(rightMotorThread, RIGHT_MOTOR_FLAG);
	}
}

int main (void) {
	
	osKernelInitialize ();                    // initialize CMSIS-RTOS
	
	servo_init();
	
	leftMotorThread = osThreadCreate(osThread(leftMotorThreadDef), NULL);
	rightMotorThread = osThreadCreate(osThread(rightMotorThreadDef), NULL);
	
	uint32_t leftMotorTimerType = 1;
	leftMotorTimer = osTimerCreate (osTimer(leftMotorDef), osTimerPeriodic, &leftMotorTimerType);
	
	leftMotorDelay = 10;
	osTimerStart (leftMotorTimer, leftMotorDelay); 
	
	uint32_t rightMotorTimerType = 1;
	rightMotorTimer = osTimerCreate (osTimer(rightMotorDef), osTimerPeriodic, &rightMotorTimerType);
	
	rightMotorDelay = 12;
	osTimerStart (rightMotorTimer, rightMotorDelay); 
	
	
}

void leftMotorCallback(void const *argument){
	osSignalSet(leftMotorThread, LEFT_MOTOR_FLAG);
}

void rightMotorCallback(void const *argument){
	osSignalSet(rightMotorThread, RIGHT_MOTOR_FLAG);
}