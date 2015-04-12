/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include <stdio.h>
#include "ADC_manager.h"
#include "LED_manager.h"
#include "Temperature.h"
#include "KalmanFilter.h"
#include "Tim_Interrupt.h"
#include "cmsis_os.h"
#include "display_controller.h"
#include "seven_segmen_display.h"
#include "keypad_manager.h"
#include "accelerometer_manager.h"
#include "mode.h"

//define signals to be used 
#define SIGNAL_ACQUIRE_TEMPERATURE_DATA 0x01
#define SIGNAL_DISPLAY 0x02
#define SIGNAL_ACCELERATION_DATA_RDY 0x03
#define SIGNAL_PWM 0x04

// ID for thread
osThreadId	Blinky_thread;
osThreadId temperature_detection_thread_id;
osThreadId display_thread_id;
osThreadId accelerometer_detection_thread_id;
osTimerId id1; //id for timer
osThreadId PWM_thread_id;//Temp

//mutex for writing to shared variables
osMutexDef (MutexTemperature);
osMutexDef (MutexPitch);
osMutexDef (MutexAlarm);


static volatile uint16_t TIM3_interrupt;
static volatile uint16_t display_on_seven_segment;
uint8_t is_temperature_exceeded = 0;
float filtered_temperature;
float filtered_pitch;
float filtered_roll;
int angle_entered;
int LED;
uint16_t mode = 0; //mode 0 is temperature display mode 1 is accelerometer display

void Timer1_Callback (void const *arg); 
void PWM_Callback(void const *arg);
osTimerDef (Timer1, Timer1_Callback); 
osTimerDef (PWM_Timer, PWM_Callback); 

void Blinky_GPIO_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
}

void Blinky(void const *argument){
	while(1){
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		printf("hello world\n");
		osDelay(250);
	}
}

/*Temperature detection thread, every time the SIGNAL_ACQUIRE_TEMPERATURE_DATA
is given by the TIM timer (at a rate of 50 Hz), this thread executes by acquiring
data from the ADC, converting it to degrees and filtering it. 
Mutex MutexTemperature is used to protect writing to shared variable filtered_temperature. 

*/
void temperature_detection_thread (void const *argument){
	uint16_t adc_value;
	float unfiltered_temperature;
	//initialize the Kalman state
	kalman_state k_state;
	k_state.q = 0.15;
	k_state.r = 8.26664;
	osEvent evt;
	osMutexId mutex_id; //mutex used for the critical section: ie writing to shared variable
	osMutexId alarm_id; //mutex for alarm flag
	osStatus status;
	mutex_id = osMutexCreate (osMutex (MutexTemperature));
	alarm_id = osMutexCreate (osMutex (MutexAlarm));
	while(1){
		
		evt = osSignalWait(SIGNAL_ACQUIRE_TEMPERATURE_DATA, osWaitForever); //waiting for TIM interrupt
		osSignalClear(temperature_detection_thread_id, SIGNAL_ACQUIRE_TEMPERATURE_DATA);
		if (evt.status == osEventSignal){
			adc_value = getADCValue();
			unfiltered_temperature = convert_to_celcius(adc_value);
			k_state = Kalmanfilter_C(&k_state, unfiltered_temperature);
			//mutual exclusion for writing to temperature variable
			osMutexWait (mutex_id, osWaitForever);
			filtered_temperature = k_state.x;
			osMutexRelease(mutex_id); //end of critical section
			//if temperature exceeds threshold then make this variable 1
			osMutexWait (alarm_id, osWaitForever);
			if (filtered_temperature > 50){
				is_temperature_exceeded = 1;
			}
			else {
				is_temperature_exceeded = 0;
			}
			osMutexRelease(alarm_id);
		}
	}
	
}

void PWM_thread (void const *argument){
	osEvent evt;
	while(1){
		
		evt = osSignalWait(SIGNAL_PWM, 5); //waiting for TIM interrupt
		osSignalClear(PWM_thread_id, SIGNAL_PWM);
		
		changeLeds(filtered_pitch, LED);
		
	}
	
}

/*Accelerometer detection happens here. The thread waits for the
SIGNAL_ACCELERATION_DATA_RDY set by IRQ handler every time acceleration
data is ready. Then fetches pitch and roll

*/
void accelerometer_detection_thread (void const *argument){
	kalman_state k_state_pitch;
	k_state_pitch.q = 85;
	k_state_pitch.r = 698;
	kalman_state k_state_roll;
	k_state_roll.q = 85;
	k_state_roll.r = 698;
	float allAxisAngles[2] = {0};
	osMutexId mutex_id; //mutex used for the critical section: ie writing to shared variable
	osStatus status;
	mutex_id = osMutexCreate (osMutex (MutexPitch));
	while(1){
		osSignalWait(SIGNAL_ACCELERATION_DATA_RDY, 10);
		osSignalClear(accelerometer_detection_thread_id,SIGNAL_ACCELERATION_DATA_RDY);
		getAngles(allAxisAngles);
		Kalmanfilter_C(&k_state_pitch, allAxisAngles[0]);
		Kalmanfilter_C(&k_state_roll, allAxisAngles[1]);
		osMutexWait (mutex_id, osWaitForever); //Critical section
		filtered_pitch = k_state_pitch.x;
		osMutexRelease(mutex_id);
		filtered_roll = k_state_roll.x;
	}
}

/*Thread used for user display. Every 1ms set by osTimer we do multiplexing for 
7 segment display. 
SIGNAL_DISPLAY is set by the osTimer 

*/
void display_thread (void const *argument){
	osEvent evt;
	int count = 0;
	
	while(1){
		
		osSignalWait(SIGNAL_DISPLAY, 1);
		osSignalClear(display_thread_id, SIGNAL_DISPLAY);
		//changeLeds(filtered_pitch, LED);
		mode = get_mode(mode);
		if (mode == 0){
			control_display(filtered_temperature, filtered_temperature, is_temperature_exceeded);
		}
		else if (mode == 1){
			control_display(filtered_pitch +90.0, angle_entered, is_temperature_exceeded); //TEMP THIS SHOULD BE THE USER ANGLE
		}
		count ++;
		//every 100 times this thread gets here when the temperature is higher then alarm, it turns off the display for 200 ms
		//to provide the flickering effect
		if (is_temperature_exceeded && count > 500){
			turn_OFF_Display();
			osTimerStop(id1);
			osDelay(200);
			osTimerStart(id1,1UL);
			count = 0;
		}

		
 }
	
}

osThreadDef(Blinky, osPriorityNormal, 1, 0);
osThreadDef(temperature_detection_thread, osPriorityNormal, 1, 0);
osThreadDef(display_thread, osPriorityNormal, 1, 0);
osThreadDef(accelerometer_detection_thread, osPriorityNormal, 1, 0);
osThreadDef(PWM_thread, osPriorityNormal, 1,0);

/*
 * main: initialize and start the system
 */
int main (void) {
  osKernelInitialize ();                    // initialize CMSIS-RTOS
	
	
  // initialize peripherals here
	Blinky_GPIO_Init(); 
	TIM3_Configuration();
	initialize_LEDs();
	configureADC();
	initialize_keypad();
	initializeAccelerometer();
	interrupt_configuration();
	initialize_seven_segment_display();
	//TIM3_interrupt = 1;
	//display_on_seven_segment= 0;
	//Get user input
	int is_angle_entered = 0;
	while(1) {
		scanning_algorithm();
		is_angle_entered = decode_key_pressed(&angle_entered);
		if (is_angle_entered) {
			break;
		}

	}
	is_angle_entered=0;
	clear_key_array();
	//Get LED input
	while(1) {
		
		scanning_algorithm();
		is_angle_entered = decode_key_pressed(&LED);
		if (is_angle_entered) {
			break;
		}

	}
  // create 'thread' functions that start executing,
  // example: tid_name = osThreadCreate (osThread(name), NULL);
	//Blinky_thread = osThreadCreate(osThread(Blinky), NULL);
	temperature_detection_thread_id = osThreadCreate(osThread(temperature_detection_thread), NULL);
	display_thread_id = osThreadCreate(osThread(display_thread), NULL);
	accelerometer_detection_thread_id = osThreadCreate(osThread(accelerometer_detection_thread),NULL);
	PWM_thread_id = osThreadCreate(osThread(PWM_thread), NULL);
	if (accelerometer_detection_thread_id!=NULL){
		printf("ACCELEROMETER THREAD CONFIGURED");
	}
	
	//create timer
	int exec1 = 1;
	int exec2 = 2;
	id1 = osTimerCreate (osTimer(Timer1), osTimerPeriodic, &exec1);
	osTimerId id2 = osTimerCreate(osTimer(PWM_Timer),osTimerPeriodic, &exec2);
	if (id1 != NULL) {
	// Periodic timer created
		printf("%s \n", " Timer exists!!");
	}
	
	osTimerStart (id1, 1UL); //timer is 1 ms or 1000 Hz for the 7 segment display
	osTimerStart(id2,2UL); //1 ms for every LED change
	osKernelStart ();                         // start thread execution 




	while(1){
		
		
		osDelay(osWaitForever);
		
	}
	
	return 0;
}


//Callback function called every 1 ms setting SIGNAL_DISPLAY for the display function
void Timer1_Callback (void const *arg) { // timer callback function

	osSignalSet(display_thread_id, SIGNAL_DISPLAY);
} 

void PWM_Callback (void const *arg) { // timer callback function

	osSignalSet(PWM_thread_id, SIGNAL_DISPLAY);
} 

//Handler for acceleration data ready
void EXTI0_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line0) != RESET){
		EXTI_ClearITPendingBit(EXTI_Line0);
		//accelerometer_interrupt = 1;
		static int first_time = 1;
		if (first_time == 1) {
			
			first_time =0;
		}
		else{
			osSignalSet(accelerometer_detection_thread_id,SIGNAL_ACCELERATION_DATA_RDY);
		}
	}
	

}


//handler for hardware timer
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
		
		//TIM3_interrupt = 1;
		//put a signal, and then on thread wait for this signal.
		osSignalSet(temperature_detection_thread_id, SIGNAL_ACQUIRE_TEMPERATURE_DATA);
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  }
}


