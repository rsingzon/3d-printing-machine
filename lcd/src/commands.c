/*!
 * @file commands.c
 * @brief Commands for the transmitter to send to the receiver board
 * @author Ryan Singzon
 * @date April 6, 2015
 */
 
#include "commands.h"

/**
* @brief Sends a command to draw a shape
* @retval None
*/
uint8_t sendShape(uint8_t shape){
	uint8_t statusByte;
	uint8_t message;
	
	// Send a message containing the shape being sent
	switch(shape){
		case TRIANGLE:
			message = 0x40;
			break;
		
		case SQUARE:
			message = 0x41;
			break;
		
		case RECTANGLE:
			message = 0x42;
			break;
	}
	
	statusByte = CC2500_Write(&message, TX_FIFO_BYTE_ADDRESS , 1);	
	
	return statusByte;
}

