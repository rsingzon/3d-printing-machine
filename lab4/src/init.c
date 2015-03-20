/**
  * @file    init.c
  * @author  Ryan Singzon, Jeffrey Tichelman
  * @date    February 28, 2015
  * @brief   Initializes peripheral devices
	*/

#include "init.h"

void initAccelerometer() 
{
	
	// Configure accelerometer parameters
	LIS3DSH_InitTypeDef accelerometer_init;
	
	accelerometer_init.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_100;				/* Power down or /active mode with output data rate 3.125 / 6.25 / 12.5 / 25 / 50 / 100 / 400 / 800 / 1600 HZ */
	accelerometer_init.Axes_Enable = LIS3DSH_X_ENABLE | LIS3DSH_Y_ENABLE | LIS3DSH_Z_ENABLE;		/* Enable all axes */
  accelerometer_init.Continous_Update = LIS3DSH_ContinousUpdate_Enabled;			/* Block or update Low/High registers of data until all data is read */
	accelerometer_init.AA_Filter_BW = LIS3DSH_AA_BW_50;													/* Choose anti-aliasing filter BW 800 / 400 / 200 / 50 Hz*/
  accelerometer_init.Full_Scale = LIS3DSH_FULLSCALE_2;       									/* Full scale 2 / 4 / 6 / 8 / 16 g */
  accelerometer_init.Self_Test = LIS3DSH_SELFTEST_NORMAL;    									/* Self test */
	
	// Initialize accelerometer
	LIS3DSH_Init(&accelerometer_init);
	
	// Enable interrupts for accelerometer
	LIS3DSH_DRYInterruptConfigTypeDef interrupt_init;
	
	interrupt_init.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;   /* Enable/Disable data ready interrupt */
  interrupt_init.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;      /* Interrupt Signal Active Low / Active High */
  interrupt_init.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED;            /* Interrupt type as latched or pulsed */ 
	
	// Initialize interrupts
	LIS3DSH_DataReadyInterruptConfig(&interrupt_init);
}

void initAccelerometerInterrupt(void)
{
	// Enable the clock for the external interrupt line (EXTI)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	// Select the pin from which interrupts should be read
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);	
	
	// Initialization struct
	EXTI_InitTypeDef interrupt_init;

	interrupt_init.EXTI_Line = EXTI_Line0; 							// Use EXTI Line 0 
  interrupt_init.EXTI_Mode = EXTI_Mode_Interrupt;     // Set the EXTI mode to interrupt
  interrupt_init.EXTI_Trigger = EXTI_Trigger_Rising; 	// Set the trigger to rising edge
  interrupt_init.EXTI_LineCmd = ENABLE;     					// Enable the EXTI line    

	// Enable interrupts
	EXTI_Init(&interrupt_init);
	
	//Enable the NVIC 
	NVIC_InitTypeDef NVIC_init; 
	
	NVIC_init.NVIC_IRQChannel = EXTI0_IRQn; 					//Use EXTI Line 0
	NVIC_init.NVIC_IRQChannelPreemptionPriority = 0; 	//Set preemption priority
	NVIC_init.NVIC_IRQChannelSubPriority = 1; 				//Set sub prioirity
	NVIC_init.NVIC_IRQChannelCmd = ENABLE; 						//Enable NVIC
	
	NVIC_Init(&NVIC_init); 														//Configure the NVIC for use with EXTI
}

void initIO(){
		
		GPIO_InitTypeDef GPIO_InitStructureB;
		GPIO_InitTypeDef GPIO_InitStructureC;
		GPIO_InitTypeDef GPIO_InitStructureE;
	
		// Init structures for keypad interrupt
		GPIO_InitTypeDef GPIO_InitStructureKeypadCol;	
	
		// Enable clock for GPIO busses
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
		// Set pins 0, 1, 4, 5, 7, 8, 11, 12, 13, 14, and 15 as output for GPIOB
		GPIO_InitStructureB.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_InitStructureB.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructureB.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureB.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureB.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOB, &GPIO_InitStructureB);
	
		// Set pin 1, 2 as outputs for GPIOC (DP, degree symbol)
		GPIO_InitStructureC.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4;
		GPIO_InitStructureC.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructureC.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureC.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureC.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOC, &GPIO_InitStructureC);
		
		// Set PIN1 on keypad
		GPIO_InitStructureKeypadCol.GPIO_Pin = GPIO_Pin_1;
		GPIO_InitStructureKeypadCol.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructureKeypadCol.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureKeypadCol.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureKeypadCol.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureKeypadCol.GPIO_PuPd = GPIO_PuPd_UP;
		
		GPIO_Init(GPIOA, &GPIO_InitStructureKeypadCol);
		GPIO_Init(GPIOD, &GPIO_InitStructureKeypadCol);
		GPIO_Init(GPIOB, &GPIO_InitStructureKeypadCol);
		GPIO_Init(GPIOE, &GPIO_InitStructureKeypadCol);
		
		// Set Row pins of keypad
		GPIO_InitStructureE.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
		GPIO_InitStructureE.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructureE.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureE.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructureE.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructureE.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOE, &GPIO_InitStructureE);
		
//		//configure interrupt for keypad
//		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1);	
//		
//		EXTI_InitTypeDef interrupt_init;
//		interrupt_init.EXTI_Line = EXTI_Line1; 							// Use EXTI Line 01
//		interrupt_init.EXTI_Mode = EXTI_Mode_Interrupt;     // Set the EXTI mode to interrupt
//		interrupt_init.EXTI_Trigger = EXTI_Trigger_Falling; 	// Set the trigger to rising edge
//		interrupt_init.EXTI_LineCmd = ENABLE;     					// Enable the EXTI line    
//		EXTI_Init(&interrupt_init);
//		
//		//Enable the NVIC 
//		NVIC_InitTypeDef NVIC_init; 
//		
//		NVIC_init.NVIC_IRQChannel = EXTI1_IRQn; 					//Use EXTI Line 0
//		NVIC_init.NVIC_IRQChannelPreemptionPriority = 0; 	//Set preemption priority
//		NVIC_init.NVIC_IRQChannelSubPriority = 1; 				//Set sub prioirity
//		NVIC_init.NVIC_IRQChannelCmd = ENABLE; 						//Enable NVIC
//		
//		NVIC_Init(&NVIC_init); 														//Configure the NVIC for use with EXTI
		
		// Enable LED GPIOs
		GPIO_InitTypeDef GPIO_InitStructure;
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		
}

/**
* @brief Initializes the ADC
* @details This function sets pins 12-15 as outputs on GPIOD
* @retval None
*/
void initADC(void)
{
	ADC_InitTypeDef adc_init_s;
	ADC_CommonInitTypeDef adc_common_init_s;
	
	// Start clock for the ADC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	
	// Configure common ADC settings
	adc_common_init_s.ADC_Mode = ADC_Mode_Independent;
	adc_common_init_s.ADC_Prescaler = ADC_Prescaler_Div2;
	adc_common_init_s.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	adc_common_init_s.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	
	// Initialize ADC
	ADC_CommonInit(&adc_common_init_s);
	
	// Configure other ADC settings
	adc_init_s.ADC_Resolution = ADC_Resolution_12b;
	adc_init_s.ADC_ScanConvMode = DISABLE;
	adc_init_s.ADC_ContinuousConvMode = DISABLE;
	adc_init_s.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	adc_init_s.ADC_DataAlign = ADC_DataAlign_Right;
	adc_init_s.ADC_NbrOfConversion = 1;
	
	// Initialize ADC
	ADC_Init(ADC1, &adc_init_s);
	
	// Enable module
	ADC_Cmd(ADC1, ENABLE); 
	
	// Set ADC channel and sample time
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_480Cycles);
	
	
	ADC_TempSensorVrefintCmd(ENABLE); //Enable the connections to the temp sensor
	ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_480Cycles); //Cofigure the ADC Channel
}

void initTimer(){
	
		// Set up TIM3
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseInitTypeDef timerInitStructure; 
    timerInitStructure.TIM_Prescaler = 100;											// Scales the clock period for counter
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		// to count up
    timerInitStructure.TIM_Period = 400;												// Sets period between update events
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;				// Specifies clock division, use standard
    timerInitStructure.TIM_RepetitionCounter = 0;								// For PWM, only relevant for TIM1 and TIM8
    TIM_TimeBaseInit(TIM3, &timerInitStructure);								// Send struct to TIM3
    TIM_Cmd(TIM3, ENABLE);																			// Enable TIM3
	
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);									// This allows timer to generate update events (interrupts)

		// Set up NVIC channel to handle interrupts from TIM3
		NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = TIM3_IRQn;									// Sets NVIC channel to TIM3
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;				// Sets interrupt priority to the lowest
    nvicStructure.NVIC_IRQChannelSubPriority = 1;								// Sets interrupt sub priority to the lowest
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;									// Enable this channel
    NVIC_Init(&nvicStructure);																	// Initialize NVIC

}

