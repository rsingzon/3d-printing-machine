/**
 * @file 	registers.h
 * @author 	Ryan Singzon, Jeffrey Tichelman
 * @date 	March 30, 2015
 * @brief 	Header file containing the addresses and default values for the CC2500 transceiver
 */

 #ifndef __REGISTERS_H
 #define __REGISTERS_H

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
#define PARTNUM_ADDRESS       0x0030
#define VERSION_ADDRESS       0x0031
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
#define IOCFG2_VALUE         0x29
#define IOCFG1_VALUE         0x2E
#define IOCFG0_VALUE         0x06
#define FIFOTHR_VALUE        0x07
#define SYNC1_VALUE          0xD3
#define SYNC0_VALUE          0x91
#define PKTLEN_VALUE         0xFF
#define PKTCTRL1_VALUE       0x04
#define PKTCTRL0_VALUE       0x05
#define ADDR_VALUE           0x00
#define CHANNR_VALUE         0x03
#define FSCTRL1_VALUE        0x0A
#define FSCTRL0_VALUE        0x00
#define FREQ2_VALUE          0x5D
#define FREQ1_VALUE          0x44
#define FREQ0_VALUE          0xEC
#define MDMCFG4_VALUE        0x2D
#define MDMCFG3_VALUE        0x3B
#define MDMCFG2_VALUE        0x73
#define MDMCFG1_VALUE        0x23
#define MDMCFG0_VALUE        0x3B
#define DEVIATN_VALUE        0x01
#define MCSM2_VALUE          0x07
#define MCSM1_VALUE          0x30
#define MCSM0_VALUE          0x18
#define FOCCFG_VALUE         0x1D
#define BSCFG_VALUE          0x1C
#define AGCCTRL2_VALUE       0xC7
#define AGCCTRL1_VALUE       0x00
#define AGCCTRL0_VALUE       0xB0
#define WOREVT1_VALUE        0x87
#define WOREVT0_VALUE        0x6B
#define WORCTRL_VALUE        0xF8
#define FREND1_VALUE         0xB6
#define FREND0_VALUE         0x10
#define FSCAL3_VALUE         0xEA
#define FSCAL2_VALUE         0x0A
#define FSCAL1_VALUE         0x00
#define FSCAL0_VALUE         0x11
#define RCCTRL1_VALUE        0x41
#define RCCTRL0_VALUE        0x00
#define FSTEST_VALUE         0x59
#define PTEST_VALUE          0x7F
#define AGCTEST_VALUE        0x3F
#define TEST2_VALUE          0x88
#define TEST1_VALUE          0x31
#define TEST0_VALUE          0x0B
#define PARTNUM_VALUE        0x80
#define VERSION_VALUE        0x03
#define FREQEST_VALUE        0x00
#define LQI_VALUE            0x00
#define RSSI_VALUE           0x00
#define MARCSTATE_VALUE      0x00
#define WORTIME1_VALUE       0x00
#define WORTIME0_VALUE       0x00
#define PKTSTATUS_VALUE      0x00
#define VCO_VC_DAC_VALUE     0x00
#define TXBYTES_VALUE        0x00
#define RXBYTES_VALUE        0x00
#define RCCTRL1_STATUS_VALUE 0x00
#define RCCTRL0_STATUS_VALUE 0x00


#endif