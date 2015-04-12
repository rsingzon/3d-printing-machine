#include <stdio.h>
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "lis302dl.h"
#include "math.h"
#include "accelerometer_manager.h"

float radianToDegree(double radian);

const double PI = 3.1415926535;
const float AxOffset = 28.0;
const float AyOffset = 23.6;
const float AzOffset = 18.28;

void initializeAccelerometer() {
	// Sets up all the parameters in order to initialize the accelerometer.
	LIS302DL_InitTypeDef accInitParameters;
	// Activates the accelerometer.
	accInitParameters.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE;
	// Set data rate to 100 Hz as per spec
	accInitParameters.Output_DataRate = LIS302DL_DATARATE_100;
	// Sets the axis to be activated. All the axis are activated (X, Y and Z) since we want to be able to have the Roll and Pitch of the board.
	accInitParameters.Axes_Enable = LIS302DL_Z_ENABLE;
	// The Full Scale range was set to 2_3 so we can achieve a better precision on the readings that we are doing.
	// The scale is going to be +/-2g at this mode (we don't need to have a larger scale for this lab experiment).
	// Go to the Accelerometer Application Notes at page 18 for more details about that.
	accInitParameters.Full_Scale = LIS302DL_FULLSCALE_2_3;
	// We set the SelfTest to NORMAL, because we don't want it to test itself.
	accInitParameters.Self_Test = LIS302DL_SELFTEST_NORMAL;

	// Initializes the LIS302DL accelerometer with the specified parameters in accInitParameters.
	LIS302DL_Init(&accInitParameters);
}

void interrupt_configuration(){
	
	GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
	
	uint8_t ctr = 0x04; //value to write to interrupt control register of MEMS
	
	LIS302DL_Write(&ctr, LIS302DL_CTRL_REG3_ADDR, 1); //configure interrupt active high, push pull, Int2 disabled and Int1 data ready.
	


	// Enables the clock for GPIOE since this GPIO is connected to the EXTI, and we need to use that since the 
	// the accelerometer is an external peripheral.
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
	// GPIO pin is set to in so that it can get the interrupt.
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	// We are using Pin 0 to get the interrupt (which is GPIOE).
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	// Selects the MEMS_INT1 as the EXTI line
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);

	EXTI_InitTypeDef EXTI_InitStructure;

	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;						// Triggers interrupt instead of event
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;							// Specifies EXTI0 channel
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	// Sets highest priority for IRQ channel
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					// Sets hightest priority in channel
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								// Enables the channel
	NVIC_Init(&NVIC_InitStructure);
	EXTI_GenerateSWInterrupt(EXTI_Line0);
	
}

void getAngles(float* allAxisAngles) {	

	double Ax,Ay,Az;
	int allAxisAcceleration[3]; //acceleration buffer
	fetchAllAxisAccelerations(allAxisAcceleration); //fetch acceleration values
	Ax = allAxisAcceleration[0];
	Ay = allAxisAcceleration[1];
	Az = allAxisAcceleration[2];
	// Compute the 3-axis tilt angles. Those formulas can be found in the Application note at page 14.
	// Those values will be in radians.
	double pitchInRad = atan(Ax / sqrt(Ay*(Ay) + Az*(Az)));
	double rollInRad = atan(Ay / sqrt(Ax*(Ax) + Az*(Az)));
	
	// Converts the readings from radian to degree.
	float pitch = radianToDegree(pitchInRad);
	float roll = radianToDegree(rollInRad);
	
	// Stores the computed tilt angles (in degree) in the array passed in the signature.
	allAxisAngles[0] = pitch;
	allAxisAngles[1] = roll;
}

/**
 * Fetches the accelerations from the 3 axis and store the retrieved values in the filter's buffer.
*/
void fetchAllAxisAccelerations(int32_t* allAxisAcceleration) {
	// Reads the acceleration from the accelerometer on all axis.
	LIS302DL_ReadACC(allAxisAcceleration);
	
	
	// Stores the accelerations in different variables.
	allAxisAcceleration[0] = (double)allAxisAcceleration[0] + AxOffset;
	allAxisAcceleration[1] = (double)allAxisAcceleration[1] + AyOffset;
	allAxisAcceleration[2] = (double)allAxisAcceleration[2] + AzOffset;
	

}

/**
 * Reads the acceleration from the 3-axis onboard accelerometer.
 *
 * @param The array that will contain the acceleration for each axis.
*/
void readAcceleration(int32_t* allAxisAcceleration) {
	LIS302DL_ReadACC(allAxisAcceleration);
}

float radianToDegree(double radian) {
	return (radian*360)/(2*PI);
}