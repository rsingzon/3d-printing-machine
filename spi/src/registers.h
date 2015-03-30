/**
 * @file 	registers.h
 * @author 	Ryan Singzon, Jeffrey Tichelman
 * @date 	March 30, 2015
 * @brief 	Header file containing the addresses and default values for the CC2500 transceiver
 */

 #ifndef __REGISTERS_H
 #define __REGISTERS_H

// Addresses for each of the CC2500 registers, obtained from the SmartRF program
#define IOCFG2_ADDRESS         0x29
#define IOCFG1_ADDRESS         0x2E
#define IOCFG0_ADDRESS         0x06
#define FIFOTHR_ADDRESS        0x07
#define SYNC1_ADDRESS          0xD3
#define SYNC0_ADDRESS          0x91
#define PKTLEN_ADDRESS         0xFF
#define PKTCTRL1_ADDRESS       0x04
#define PKTCTRL0_ADDRESS       0x05
#define ADDR_ADDRESS           0x00
#define CHANNR_ADDRESS         0x03
#define FSCTRL1_ADDRESS        0x0A
#define FSCTRL0_ADDRESS        0x00
#define FREQ2_ADDRESS          0x5D
#define FREQ1_ADDRESS          0x44
#define FREQ0_ADDRESS          0xEC
#define MDMCFG4_ADDRESS        0x2D
#define MDMCFG3_ADDRESS        0x3B
#define MDMCFG2_ADDRESS        0x73
#define MDMCFG1_ADDRESS        0x23
#define MDMCFG0_ADDRESS        0x3B
#define DEVIATN_ADDRESS        0x01
#define MCSM2_ADDRESS          0x07
#define MCSM1_ADDRESS          0x30
#define MCSM0_ADDRESS          0x18
#define FOCCFG_ADDRESS         0x1D
#define BSCFG_ADDRESS          0x1C
#define AGCCTRL2_ADDRESS       0xC7
#define AGCCTRL1_ADDRESS       0x00
#define AGCCTRL0_ADDRESS       0xB0
#define WOREVT1_ADDRESS        0x87
#define WOREVT0_ADDRESS        0x6B
#define WORCTRL_ADDRESS        0xF8
#define FREND1_ADDRESS         0xB6
#define FREND0_ADDRESS         0x10
#define FSCAL3_ADDRESS         0xEA
#define FSCAL2_ADDRESS         0x0A
#define FSCAL1_ADDRESS         0x00
#define FSCAL0_ADDRESS         0x11
#define RCCTRL1_ADDRESS        0x41
#define RCCTRL0_ADDRESS        0x00
#define FSTEST_ADDRESS         0x59
#define PTEST_ADDRESS          0x7F
#define AGCTEST_ADDRESS        0x3F
#define TEST2_ADDRESS          0x88
#define TEST1_ADDRESS          0x31
#define TEST0_ADDRESS          0x0B
#define PARTNUM_ADDRESS        0x80
#define VERSION_ADDRESS        0x03
#define FREQEST_ADDRESS        0x00
#define LQI_ADDRESS            0x00
#define RSSI_ADDRESS           0x00
#define MARCSTATE_ADDRESS      0x00
#define WORTIME1_ADDRESS       0x00
#define WORTIME0_ADDRESS       0x00
#define PKTSTATUS_ADDRESS      0x00
#define VCO_VC_DAC_ADDRESS     0x00
#define TXBYTES_ADDRESS        0x00
#define RXBYTES_ADDRESS        0x00
#define RCCTRL1_STATUS_ADDRESS 0x00
#define RCCTRL0_STATUS_ADDRESS 0x00


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