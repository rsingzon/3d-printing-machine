/**
  * @file    gyroscope.c
  * @author  Ryan Singzon
  * @date    April 11, 2015
  * @brief   Provides functions to read gyroscope
	*/

#include "gyroscope.h"

/**
 * Initializes the gyroscope
 */
void initGyroscope(void){

	// Initialize gyroscope settings
	L3GD20_InitTypeDef gyro_init;
	
	gyro_init.Power_Mode = L3GD20_MODE_ACTIVE;
	gyro_init.Output_DataRate = L3GD20_OUTPUT_DATARATE_1;
	gyro_init.Axes_Enable = L3GD20_AXES_ENABLE;
	gyro_init.Band_Width = L3GD20_BANDWIDTH_1;
	gyro_init.BlockData_Update = L3GD20_BlockDataUpdate_Single;
	gyro_init.Endianness = L3GD20_BLE_LSB;
	gyro_init.Full_Scale = L3GD20_FULLSCALE_250;
	
	L3GD20_Init(&gyro_init);
	
	// Enable interrupts for gyroscope



//	// Enable the clock for the external interrupt line (EXTI)
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//	
//	// Select the pin from which interrupts should be read
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);	
//	
//	// Initialization struct
//	EXTI_InitTypeDef interrupt_init;

//	interrupt_init.EXTI_Line = EXTI_Line0; 							// Use EXTI Line 0 
//  interrupt_init.EXTI_Mode = EXTI_Mode_Interrupt;     // Set the EXTI mode to interrupt
//  interrupt_init.EXTI_Trigger = EXTI_Trigger_Rising; 	// Set the trigger to rising edge
//  interrupt_init.EXTI_LineCmd = ENABLE;     					// Enable the EXTI line    

//	// Enable interrupts
//	EXTI_Init(&interrupt_init);
//	
//	//Enable the NVIC 
//	NVIC_InitTypeDef NVIC_init; 
//	
//	NVIC_init.NVIC_IRQChannel = EXTI0_IRQn; 					//Use EXTI Line 0
//	NVIC_init.NVIC_IRQChannelPreemptionPriority = 0; 	//Set preemption priority
//	NVIC_init.NVIC_IRQChannelSubPriority = 1; 				//Set sub prioirity
//	NVIC_init.NVIC_IRQChannelCmd = ENABLE; 						//Enable NVIC
//	
//	NVIC_Init(&NVIC_init); 														//Configure the NVIC for use with EXTI
}

/**
 * @brief Reads the gyroscope and returns a struct containing the rotation values for the three axes
 */
void getRotation(L3GD20_Rotation *rotations){
	
	// Read the high and low portions of each of the outputs
	uint8_t buffer[6];
	
	L3GD20_Read(&buffer[0], L3GD20_OUT_X_L_ADDR, 1);
	L3GD20_Read(&buffer[1], L3GD20_OUT_X_H_ADDR, 1);
	L3GD20_Read(&buffer[2], L3GD20_OUT_Y_L_ADDR, 1);
	L3GD20_Read(&buffer[3], L3GD20_OUT_Y_H_ADDR, 1);
	L3GD20_Read(&buffer[4], L3GD20_OUT_Z_L_ADDR, 1);
	L3GD20_Read(&buffer[5], L3GD20_OUT_Z_H_ADDR, 1);
	
	// Combine the high and low halves
	uint16_t out_x = (int32_t)(buffer[0] | buffer[1] << 8);
	uint16_t out_y = (int32_t)(buffer[2] | buffer[3] << 8);
	uint16_t out_z = (int32_t)(buffer[4] | buffer[5] << 8);
	
	rotations->X = out_x;
	rotations->Y = out_y;
	rotations->Z = out_z;
}