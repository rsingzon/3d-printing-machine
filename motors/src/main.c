#include <stdio.h>

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
//static volatile uint_fast16_t ticks; //volatile because it changes outside of normal program flow and want to re-read. 

// Define OS flags
#define PWM_FLAG 0x01

// Define thread IDs
osThreadId motorThread;
osThreadId counterThread;

// Thread definition prototypes
void motorThreadDef(void const *argument);
void counterThreadDef(void const *argument);

// Define thread priorities
osThreadDef(motorThreadDef, osPriorityAboveNormal, 1, 768);
osThreadDef(counterThreadDef, osPriorityAboveNormal, 1, 768);

// Global variables 
int speed;

TIM_OCInitTypeDef  TIM_OCInitStructure;


int main (void) {
  //initlilization all component
    
 /*   servo_motor_init();
    wireless_Receiver_Thread_Create();

  osDelay(osWaitForever);
	*/
	
	GPIO_InitTypeDef GPIO_InitStructure;
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		
	/* GPIOD clock enable*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	/* Set GPIO to output*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	/* High speed */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	/* Output type: push pull */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	/* Pull up */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
		

		TIM_TimeBaseInitTypeDef TIMInitStructure;
		//4000 * (168Mhz)^-1 * period = 0.02 ie create event at a rate of 50 Hz
		// prescaler = 4000, period = 840
    TIMInitStructure.TIM_Prescaler = 42;
    TIMInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIMInitStructure.TIM_Period = 500;
    TIMInitStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseInit(TIM3, &TIMInitStructure);
		TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, ENABLE);	
		

	// Set up NVIC channel to handle interrupts from TIM3
		NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = TIM3_IRQn;									// Sets NVIC channel to TIM3
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;				// Sets interrupt priority to the lowest
    nvicStructure.NVIC_IRQChannelSubPriority = 1;								// Sets interrupt sub priority to the lowest
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;									// Enable this channel
    NVIC_Init(&nvicStructure);	


/*
	// The fade method uses the clock to count the period
	// Core clock = 168MHz
	int period = 16840;
	int dutyCycle = 2000;
	int count = 0;
	
	
	// Fade in LEDs
	while(1){
		while (count < dutyCycle) {
			GPIO_SetBits(GPIOD,GPIO_Pin_1); 
			count++;
		}
		while (count < period){
			GPIO_ResetBits(GPIOD,GPIO_Pin_1);
			count++;
		}
		count = 0;
	}
	
	*/
	

	// Initialize threads
	motorThread = osThreadCreate(osThread(motorThreadDef), NULL);
	counterThread = osThreadCreate(osThread(counterThreadDef), NULL);
	
	osKernelStart(); 	
	
}

/**
*@brief Thread to control the speed of the motors
*/
void motorThreadDef(void const *argument){
		
	uint16_t motorPin = GPIO_Pin_1 | GPIO_Pin_12;
	int pwm_time = 0;
	
	float angle;
	float fraction;
	speed = 3;
	
	while(1){
		
		osSignalWait(PWM_FLAG, osWaitForever);
		pwm_time++;
	
		if (pwm_time > 40) pwm_time=0;
		if (pwm_time < speed)
      GPIO_SetBits(GPIOD,motorPin); 
		else
      GPIO_ResetBits(GPIOD,motorPin);

		osSignalClear(motorThread, PWM_FLAG);
	
	}
}

/**
*@brief Thread to control the speed of the motors
*/
void counterThreadDef(void const *argument){
		
	int i;
	/*
	while(1){
			for(i = 0; i < 10000; i++);
			
			if(speed == 1){
				speed = 2;
			}
			else{
				speed = 1;
			}
	}
	*/
}


/*
int main(){
	uint16_t adc_value;
	float unfiltered_val;
	float filtered_val;
	kalman_state k_state;
	ticks = 0;
	SysTick_Config(20 * SystemCoreClock / 1000);
	//Initialize peripherals 
	initialize_LEDs();
	configureADC();
	while(1){
		
		while(!ticks);
		ticks = 0; //decrement the ticks
		adc_value = getADCValue();
		unfiltered_val = convert_to_celcius(adc_value);
		k_state = Kalmanfilter_C(&k_state, unfiltered_val);
		control_LEDs(k_state.x);
		printf("%f \n", k_state.x);	
	}
	
	return 0;
}

void SysTick_Handler() {
	ticks = 1;
} 

*/

/**
*@brief Interupt handler for TIM3, generates interrupts with configured frequency
*@retval None
*/
void TIM3_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);			// Clears incoming interrupt bit
	osSignalSet(motorThread, PWM_FLAG);
}
