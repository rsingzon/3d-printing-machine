/**
  ******************************************************************************
  * @file    cc2500.h
  * @author  Jeffrey Tichelman, Ryan Singzon
  * @date    April 5, 2015
  * @brief   This file contains definitions for the CC2500 wireless transceiver
  ******************************************************************************
	**/
	
#ifndef __CC2500_H
#define __CC2500_H

// Configuration registers
#define RESET_COMMAND					0x30
#define CALIBRATE_COMMAND			0x33
#define RECEIVE_COMMAND				0x34
#define TRANSMIT_COMMAND			0x35
#define	IDLE_COMMAND					0x36
#define NO_OP_COMMAND					0x3D

#define FLUSH_RX_FIFO_COMMAND	0x3A

// Define states
#define TRANSMITTING					0x20
#define RECEIVING							0x10
#define IDLE									0x00

// Register addresses
#define TX_BYTES							0x3A
#define RX_BYTES							0x3B			

#define NUM_REGISTERS_TO_INIT 	47
 
// FIFO Addresses - From datasheet page 24
#define TX_FIFO_BYTE_ADDRESS	0x3F
#define TX_FIFO_BURST_ADDRESS	0x7F
#define RX_FIFO_BYTE_ADDRESS	0xBF
#define	RX_FIFO_BURST_ADDRESS	0xFF
 

// Addresses for each of the CC2500 registers, obtained from the SmartRF program
#define IOCFG2_ADDRESS        0x0000
#define IOCFG1_ADDRESS        0x0001
#define IOCFG0_ADDRESS        0x0002
#define FIFOTHR_ADDRESS       0x0003
#define SYNC1_ADDRESS         0x0004
#define SYNC0_ADDRESS         0x0005
#define PKTLEN_ADDRESS        0x0006
#define PKTCTRL1_ADDRESS      0x0007
#define PKTCTRL0_ADDRESS      0x0008
#define ADDR_ADDRESS          0x0009
#define CHANNR_ADDRESS        0x000A
#define FSCTRL1_ADDRESS       0x000B
#define FSCTRL0_ADDRESS       0x000C
#define FREQ2_ADDRESS         0x000D
#define FREQ1_ADDRESS         0x000E
#define FREQ0_ADDRESS         0x000F
#define MDMCFG4_ADDRESS       0x0010
#define MDMCFG3_ADDRESS       0x0011
#define MDMCFG2_ADDRESS       0x0012
#define MDMCFG1_ADDRESS       0x0013
#define MDMCFG0_ADDRESS       0x0014
#define DEVIATN_ADDRESS       0x0015
#define MCSM2_ADDRESS         0x0016
#define MCSM1_ADDRESS         0x0017
#define MCSM0_ADDRESS         0x0018
#define FOCCFG_ADDRESS        0x0019
#define BSCFG_ADDRESS         0x001A
#define AGCCTRL2_ADDRESS      0x001B
#define AGCCTRL1_ADDRESS      0x001C
#define AGCCTRL0_ADDRESS      0x001D
#define WOREVT1_ADDRESS       0x001E
#define WOREVT0_ADDRESS       0x001F
#define WORCTRL_ADDRESS       0x0020
#define FREND1_ADDRESS        0x0021
#define FREND0_ADDRESS        0x0022
#define FSCAL3_ADDRESS        0x0023
#define FSCAL2_ADDRESS        0x0024
#define FSCAL1_ADDRESS        0x0025
#define FSCAL0_ADDRESS        0x0026
#define RCCTRL1_ADDRESS       0x0027
#define RCCTRL0_ADDRESS       0x0028
#define FSTEST_ADDRESS        0x0029
#define PTEST_ADDRESS         0x002A
#define AGCTEST_ADDRESS       0x002B
#define TEST2_ADDRESS         0x002C
#define TEST1_ADDRESS         0x002D
#define TEST0_ADDRESS         0x002E
#define PARTNUM_ADDRESS       0x00F0
#define VERSION_ADDRESS       0x00F1
#define FREQEST_ADDRESS       0x0032
#define LQI_ADDRESS           0x0033
#define RSSI_ADDRESS          0x0034
#define MARCSTATE_ADDRESS     0x0035
#define WORTIME1_ADDRESS      0x0036
#define WORTIME0_ADDRESS      0x0037
#define PKTSTATUS_ADDRESS     0x0038
#define VCO_VC_DAC_ADDRESS    0x0039
#define TXBYTES_ADDRESS       0x003A
#define RXBYTES_ADDRESS       0x003B
#define RCCTRL1_STATUS_ADDRESS	0x003C
#define RCCTRL0_STATUS_ADDRESS	0x003D



// Default values for registers obtained from the SmartRF program
#define VAL_CC2500_IOCFG2 	(uint8_t)0x29 		
#define VAL_CC2500_IOCFG0 	(uint8_t)0x06	

#define VAL_CC2500_FIFOTHR 	(uint8_t)0x07

#define VAL_CC2500_PKTLEN 	(uint8_t)0x01

#define VAL_CC2500_PKTCTRL1 (uint8_t)0x08	
#define VAL_CC2500_PKTCTRL0 (uint8_t)0x04	

#define VAL_CC2500_ADDR 		(uint8_t)0x00	

#define VAL_CC2500_CHANNR 	(uint8_t)0x00

#define VAL_CC2500_FSCTRL1 	(uint8_t)0x0C	
#define VAL_CC2500_FSCTRL0 	(uint8_t)0x00	

// freq set to 2.433 GHz, note f_xosc = 26MHz
#define VAL_CC2500_FREQ2 		(uint8_t)0x5D	
#define VAL_CC2500_FREQ1 		(uint8_t)0x94	
#define VAL_CC2500_FREQ0 		(uint8_t)0x02	

#define VAL_CC2500_MDMCFG4 	(uint8_t)0x0E
#define VAL_CC2500_MDMCFG3 	(uint8_t)0x3B
#define VAL_CC2500_MDMCFG2 	(uint8_t)0x73	
#define VAL_CC2500_MDMCFG1 	(uint8_t)0x42	
#define VAL_CC2500_MDMCFG0 	(uint8_t)0xF8	

#define VAL_CC2500_DEVIATN 	(uint8_t)0x00	


#define VAL_CC2500_MCSM1 		(uint8_t)0x02	
#define VAL_CC2500_MCSM0 		(uint8_t)0x18	

#define VAL_CC2500_FOCCFG 	(uint8_t)0x1D	
#define VAL_CC2500_BSCFG 		(uint8_t)0x1C	

#define VAL_CC2500_AGCTRL2 	(uint8_t)0xC7	 
#define VAL_CC2500_AGCTRL1 	(uint8_t)0x40	
#define VAL_CC2500_AGCTRL0 	(uint8_t)0xB0

#define VAL_CC2500_FREND1 	(uint8_t)0xB6	 
#define VAL_CC2500_FREND0 	(uint8_t)0x10	

#define VAL_CC2500_FSCAL3 	(uint8_t)0xEA	 
#define VAL_CC2500_FSCAL2 	(uint8_t)0x0A	
#define VAL_CC2500_FSCAL1 	(uint8_t)0x00	 
#define VAL_CC2500_FSCAL0 	(uint8_t)0x19	

#define VAL_CC2500_FSTEST 	(uint8_t)0x59

#define VAL_CC2500_TEST2 		(uint8_t)0x88	
#define VAL_CC2500_TEST1 		(uint8_t)0x31	
#define VAL_CC2500_TEST0 		(uint8_t)0x0B

// Function prototypes
uint8_t CC2500_Init_Registers(void);
void CC2500_Read_Registers(void);
uint8_t CC2500_Start_Transmit(void);
uint8_t CC2500_Start_Receive(void);
uint8_t CC2500_Reset(void);
uint8_t CC2500_No_Op(void);
uint8_t CC2500_Set_Channel(uint8_t *channel);
#endif