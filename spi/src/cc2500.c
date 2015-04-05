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
  * @brief  Initializes the registers of the CC2500
  * @retval Status
  */
uint8_t CC2500_Init_Registers(void){
	
	uint8_t regValueArray[NUM_REGISTERS_TO_INIT];
	
	regValueArray[IOCFG2_ADDRESS] = VAL_CC2500_IOCFG2;
	regValueArray[IOCFG0_ADDRESS] = VAL_CC2500_IOCFG0;
	regValueArray[FIFOTHR_ADDRESS] = VAL_CC2500_FIFOTHR;
	regValueArray[PKTLEN_ADDRESS] = VAL_CC2500_PKTLEN;
	regValueArray[PKTCTRL1_ADDRESS] = VAL_CC2500_PKTCTRL1;
	regValueArray[PKTCTRL0_ADDRESS] = VAL_CC2500_PKTCTRL0;
	regValueArray[ADDR_ADDRESS] = VAL_CC2500_ADDR;
	regValueArray[CHANNR_ADDRESS] = VAL_CC2500_CHANNR;
	regValueArray[FSCTRL1_ADDRESS] = VAL_CC2500_FSCTRL1;
	regValueArray[FSCTRL0_ADDRESS] = VAL_CC2500_FSCTRL0;
	regValueArray[FREQ2_ADDRESS] = VAL_CC2500_FREQ2;
	regValueArray[FREQ1_ADDRESS] = VAL_CC2500_FREQ1;
	regValueArray[FREQ0_ADDRESS] = VAL_CC2500_FREQ0;
	regValueArray[MDMCFG4_ADDRESS] = VAL_CC2500_MDMCFG4;
	regValueArray[MDMCFG3_ADDRESS] = VAL_CC2500_MDMCFG3;
	regValueArray[MDMCFG2_ADDRESS] = VAL_CC2500_MDMCFG2;
	regValueArray[MDMCFG1_ADDRESS] = VAL_CC2500_MDMCFG1;
	regValueArray[MDMCFG0_ADDRESS] = VAL_CC2500_MDMCFG0;
	regValueArray[DEVIATN_ADDRESS] = VAL_CC2500_DEVIATN;
	regValueArray[MCSM1_ADDRESS] = VAL_CC2500_MCSM1;
	regValueArray[MCSM0_ADDRESS] = VAL_CC2500_MCSM0;
	regValueArray[FOCCFG_ADDRESS] = VAL_CC2500_FOCCFG;
	regValueArray[BSCFG_ADDRESS] = VAL_CC2500_BSCFG;
	regValueArray[AGCCTRL2_ADDRESS] = VAL_CC2500_AGCTRL2;
	regValueArray[AGCCTRL1_ADDRESS] = VAL_CC2500_AGCTRL1;
	regValueArray[AGCCTRL0_ADDRESS] = VAL_CC2500_AGCTRL0;
	
	regValueArray[FREND1_ADDRESS] = VAL_CC2500_FREND1;
	regValueArray[FREND0_ADDRESS] = VAL_CC2500_FREND0;
	regValueArray[FSCAL3_ADDRESS] = VAL_CC2500_FSCAL3;
	regValueArray[FSCAL2_ADDRESS] = VAL_CC2500_FSCAL2;
	regValueArray[FSCAL1_ADDRESS] = VAL_CC2500_FSCAL1;
	regValueArray[FSCAL0_ADDRESS] = VAL_CC2500_FSCAL0;
	
	
	regValueArray[FSTEST_ADDRESS] = VAL_CC2500_FSTEST;
	
	
	regValueArray[TEST2_ADDRESS] = VAL_CC2500_TEST2;
	regValueArray[TEST1_ADDRESS] = VAL_CC2500_TEST1;
	regValueArray[TEST0_ADDRESS] = VAL_CC2500_TEST0;
	
	
	CC2500_Write(regValueArray, 0, NUM_REGISTERS_TO_INIT);
	
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