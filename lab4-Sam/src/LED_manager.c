#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "LED_manager.h"

enum {
	TOP,
	RIGHT,
	LEFT,
	BOTTOM
}led_location; 

static void Delay();//dummy loop that adds delay

#define MAX_POSITIVE_DEVIATION 2
#define MAX_NEGATIVE_DEVIATION -2
#define ALARM_TEMPERATURE 50


float current_baseline = 0; 
int led_state = TOP;
//uint32_t dutyCycle; //percent of time the LEDs should stay on
//uint32_t tickCounter = 0; //tells us which time-slice of the period we are on
const uint32_t PERIOD = 18; //Each period has 100 time-slices 


//Configures LEDs
void initialize_LEDs(){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	//Enable the clock 
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15; //Configure the pins for LEDs
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //set the mode to out because it's an output
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //set the type to push pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //dont limit the slew rate
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}
//controls LEDs given a filtered temperature
void control_LEDs(float temperature) {
	static int is_first_time = 1; //this value is set to one for first time initializion then set to 0
	static int was_in_pulse = 0; //if system was previously in pulse mode or not
	//perform first time initialization by setting temperature to current baseline
	if (is_first_time) {
		current_baseline = temperature;
		is_first_time = 0;
	}
	
	//perform pulse if temperature exceeds threshold
	if (temperature > ALARM_TEMPERATURE){
		startPulse();
		//changeLeds();
		current_baseline = temperature;
		turn_LEDs_off();
		was_in_pulse = 1;
	}
	
	//rotate ccwise if the temperature deviated by more than -2
	else if (temperature - current_baseline < MAX_NEGATIVE_DEVIATION){
		turn_LEDs_off();
		rotate_counterclockwise();
		turn_on_LED();
		current_baseline = temperature;
		
	}
	//rotate cwise if temperature deviated by more than 2
	else if (temperature - current_baseline > MAX_POSITIVE_DEVIATION) {
		turn_LEDs_off();
		rotate_clockwise();
		turn_on_LED();
		current_baseline = temperature;
		
	}
	//if it was previously in pulse mode restore the LED that was on before entering in pulse mode
	else if (temperature < ALARM_TEMPERATURE && was_in_pulse ==1) {
		was_in_pulse = 0;
		turn_on_LED();
	}
}

//initializes the tick counter and duty Cycle to 0
void startPulse(){

}

/**
 * Generates pulse by PWM
*/
void changeLeds(float angle, int LED){
		static int direction = 0; //stores the direction of counting up = 0 down = 1
		static uint32_t tickCounter = 1; //tick increments or decretements every 2 ms
		static uint32_t dutyCycle = 0; //duty cycle initialized to 0 
		dutyCycle = (uint32_t)angle/10; //duty cycle is according to angle eg if angle is 10 duty cycle is 1. This achieves intervals of 0-18 for dutycyle 1. 18-36 dutycyle =2 etc
		//if tick counter is less than duty cycle keep LED on otherwise off for PWM effect
		if (tickCounter <= dutyCycle) {
			ledsOn(LED);
		}
		else {
			turn_LEDs_off();
		}
		//if tickCounter is greater than designated period, here 18 because max duty cycle is 180/10 then switch direction
		if (tickCounter > PERIOD) {
			direction = !direction;
		}
		//if tickCounter reaches minimum then switch direction to upwards
		if (tickCounter == 0) {
			direction = !direction;
		}
		//if direction is up count up else count down
		if (direction == 0){
			tickCounter++;
		}
		else {
			tickCounter--;
		}
		
}
		




/**
 * switch on all leds
*/
void ledsOn(int LED){
	if (LED == 1){
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
	}
	else if (LED ==2){
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
	}
	else if (LED == 3){
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
	}
	else if (LED == 4){
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
	}
}

//turn clockwise according to led_state
void rotate_clockwise(){
	
	turn_LEDs_off();
	
	switch(led_state){
		case TOP: 
			led_state = RIGHT;
			return;
		case RIGHT:
			led_state = BOTTOM;
			return;
		case BOTTOM:
			led_state = LEFT;
			return;
		case LEFT:
			led_state =  TOP;
			return;
	}
}

//turn ccwise according to led_state
void rotate_counterclockwise(){
	
	turn_LEDs_off();
	switch(led_state){
		case TOP: 
			led_state = LEFT;
			return;
		case LEFT:
			led_state = BOTTOM;
			return;
		case BOTTOM:
			led_state = RIGHT;
			return;
		case RIGHT:
			led_state = TOP;
			return;
	}
}

//turn all leds off
void turn_LEDs_off(){
	GPIO_ResetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
}

//turn on specific led according to led state
void turn_on_LED(){
	
	switch(led_state){
		
		case TOP:
			GPIO_SetBits(GPIOD, GPIO_Pin_13);
			return;
		case RIGHT:
			GPIO_SetBits(GPIOD, GPIO_Pin_14);
			return;
		case BOTTOM:
			GPIO_SetBits(GPIOD, GPIO_Pin_15);
			return;
		case LEFT:
			GPIO_SetBits(GPIOD, GPIO_Pin_12);
			return;
		
	}
}

//dummy loop to add delay between lighting up 
static void Delay() {
	int i = 0;
	
		while(i < 5000) {
			i++;
		}
}
