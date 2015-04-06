/*!
 * @file cc2500.c
 * @brief CC2500 wireless transceiver functions
 * @author Ryan Singzon 260397455
 * @author Jeffrey Tichelman 260446802
 * @date April 5, 2015
 */
 
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_conf.h"
#include "cc2500.h"
#include "spi.h"
 
 /**
  * @brief  Starts transmission on the CC2500
  * @retval Status
  */
uint8_t CC2500_Start_Transmit(void){
	
	uint8_t status = CC2500_Write(0,TRANSMIT_COMMAND,0); 
	return status;
}

 /**
  * @brief  Starts receiving on the CC2500
  * @retval Status
  */
uint8_t CC2500_Start_Receive(void){
	
	uint8_t status = CC2500_Write(0,RECEIVE_COMMAND,0); 
	return status;
}

 /**
  * @brief  Reset CC2500
  * @retval Status
  */
uint8_t CC2500_Reset(void){
	
	uint8_t status = CC2500_Write(0,RESET_COMMAND,0); 
	return status;
}

 /**
  * @brief  Strobes the No op command
  * @retval Status
  */
uint8_t CC2500_No_Op(void){
	
	uint8_t status = CC2500_Write(0,NO_OP_COMMAND,0); 
	return status;
}

 /**
  * @brief  Sets the channel of the transceiver
	* @param  Channel
  * @retval Status
  */
uint8_t CC2500_Set_Channel(uint8_t *channel){
	
	uint8_t status = CC2500_Write(channel, CHANNR_ADDRESS, 1);
	return status;
}


 /**
  * @brief  Initializes the registers of the CC2500
  * @retval Status
  */
uint8_t CC2500_Init_Registers(void){
	
	uint8_t regValueArray[NUM_REGISTERS_TO_INIT];

	regValueArray[0] = VAL_CC2500_IOCFG2;
	regValueArray[1] = VAL_CC2500_IOCFG0;
	regValueArray[2] = VAL_CC2500_FIFOTHR;
	regValueArray[3] = VAL_CC2500_PKTLEN;
	regValueArray[4] = VAL_CC2500_PKTCTRL1;
	regValueArray[5] = VAL_CC2500_PKTCTRL0;
	regValueArray[6] = VAL_CC2500_ADDR;
	regValueArray[7] = VAL_CC2500_CHANNR;
	regValueArray[8] = VAL_CC2500_FSCTRL1;
	regValueArray[9] = VAL_CC2500_FSCTRL0;
	regValueArray[10] = VAL_CC2500_FREQ2;
	regValueArray[11] = VAL_CC2500_FREQ1;
	regValueArray[12] = VAL_CC2500_FREQ0;
	regValueArray[13] = VAL_CC2500_MDMCFG4;
	regValueArray[14] = VAL_CC2500_MDMCFG3;
	regValueArray[15] = VAL_CC2500_MDMCFG2;
	regValueArray[16] = VAL_CC2500_MDMCFG1;
	regValueArray[17] = VAL_CC2500_MDMCFG0;
	regValueArray[18] = VAL_CC2500_DEVIATN;
	regValueArray[19] = VAL_CC2500_MCSM1;
	regValueArray[20] = VAL_CC2500_MCSM0;
	regValueArray[21] = VAL_CC2500_FOCCFG;
	regValueArray[22] = VAL_CC2500_BSCFG;
	regValueArray[23] = VAL_CC2500_AGCTRL2;
	regValueArray[24] = VAL_CC2500_AGCTRL1;
	regValueArray[25] = VAL_CC2500_AGCTRL0;
	
	regValueArray[26] = VAL_CC2500_FREND1;
	regValueArray[27] = VAL_CC2500_FREND0;
	regValueArray[28] = VAL_CC2500_FSCAL3;
	regValueArray[29] = VAL_CC2500_FSCAL2;
	regValueArray[30] = VAL_CC2500_FSCAL1;
	regValueArray[31] = VAL_CC2500_FSCAL0;
	
	
	regValueArray[32] = VAL_CC2500_FSTEST;
	
	regValueArray[33] = VAL_CC2500_TEST2;
	regValueArray[34] = VAL_CC2500_TEST1;
	regValueArray[35] = VAL_CC2500_TEST0;
	
	CC2500_Write(&regValueArray[0], IOCFG2_ADDRESS, 1);
	CC2500_Write(&regValueArray[1], IOCFG0_ADDRESS, 1);
	CC2500_Write(&regValueArray[2], FIFOTHR_ADDRESS, 1);
	CC2500_Write(&regValueArray[3], PKTLEN_ADDRESS, 1);
	CC2500_Write(&regValueArray[4], PKTCTRL1_ADDRESS, 1);
	CC2500_Write(&regValueArray[5], PKTCTRL0_ADDRESS, 1);
	CC2500_Write(&regValueArray[6], ADDR_ADDRESS, 1);
	CC2500_Write(&regValueArray[7], CHANNR_ADDRESS, 1);
	CC2500_Write(&regValueArray[8], FSCTRL1_ADDRESS, 1);
	CC2500_Write(&regValueArray[9], FSCTRL0_ADDRESS, 1);
	CC2500_Write(&regValueArray[10], FREQ2_ADDRESS, 1);
	CC2500_Write(&regValueArray[11], FREQ1_ADDRESS, 1);
	CC2500_Write(&regValueArray[12], FREQ0_ADDRESS, 1);
	CC2500_Write(&regValueArray[13], MDMCFG4_ADDRESS, 1);
	CC2500_Write(&regValueArray[14], MDMCFG3_ADDRESS, 1);
	CC2500_Write(&regValueArray[15], MDMCFG2_ADDRESS, 1);
	CC2500_Write(&regValueArray[16], MDMCFG1_ADDRESS, 1);
	CC2500_Write(&regValueArray[17], MDMCFG0_ADDRESS, 1);
	CC2500_Write(&regValueArray[18], DEVIATN_ADDRESS, 1);
	CC2500_Write(&regValueArray[19], MCSM1_ADDRESS, 1);
	CC2500_Write(&regValueArray[20], MCSM0_ADDRESS, 1);
	CC2500_Write(&regValueArray[21], FOCCFG_ADDRESS, 1);
	CC2500_Write(&regValueArray[22], BSCFG_ADDRESS, 1);
	CC2500_Write(&regValueArray[23], AGCCTRL2_ADDRESS, 1);
	CC2500_Write(&regValueArray[24], AGCCTRL1_ADDRESS, 1);
	CC2500_Write(&regValueArray[25], AGCCTRL0_ADDRESS, 1);
	CC2500_Write(&regValueArray[26], FREND1_ADDRESS, 1);
	CC2500_Write(&regValueArray[27], FREND0_ADDRESS, 1);
	CC2500_Write(&regValueArray[28], FSCAL3_ADDRESS, 1);
	CC2500_Write(&regValueArray[29], FSCAL2_ADDRESS, 1);
	CC2500_Write(&regValueArray[30], FSCAL1_ADDRESS, 1);
	CC2500_Write(&regValueArray[31], FSCAL0_ADDRESS, 1);
	
	CC2500_Write(&regValueArray[32], FSTEST_ADDRESS, 1);
	CC2500_Write(&regValueArray[33], TEST2_ADDRESS, 1);
	CC2500_Write(&regValueArray[34], TEST1_ADDRESS, 1);
	CC2500_Write(&regValueArray[35], TEST0_ADDRESS, 1);
		
	return 0;
}

void CC2500_Read_Registers(void){
	
	int numBytes = 1;
	uint8_t state;
	uint8_t regAddr = 0x00;
	
	int i; 
	
	// Read all register values
	for(i = 0; i < NUM_REGISTERS_TO_INIT; i++){
		CC2500_Read(&state, regAddr, numBytes);
		printf("Address: %x Value: %x\n", regAddr, state);
		
		regAddr++;
	}	
}