/*!
 * @file commands.c
 * @brief Commands for the transmitter to send to the receiver board
 * @author Ryan Singzon
 * @date April 6, 2015
 */
 
#include "commands.h"

/**
* @brief Sends a command to draw a shape
* @param Shape to draw
* @retval Status byte
*/
uint8_t sendShape(uint8_t shape){
	uint8_t statusByte;
	uint8_t message;
	
	// Send a message containing the shape being sent
	switch(shape){
		case TRIANGLE:
			message = TRIANGLE_COMMAND;
			break;
		
		case SQUARE:
			message = SQUARE_COMMAND;
			break;
		
		case RECTANGLE:
			message = RECTANGLE_COMMAND;
			break;
	}
	
	statusByte = CC2500_Write(&message, TX_FIFO_BYTE_ADDRESS , 1);		
	return statusByte;
}

/**
* @brief Sends a command to draw a free drawing
* @param line to draw
* @retval Status byte
*/
uint8_t sendFreeDraw(uint8_t direction){
	uint8_t statusByte;
	uint8_t message;
	int count = 0;
	
	message = direction;
	statusByte = CC2500_Write(&message, TX_FIFO_BYTE_ADDRESS, 1);
	
	return statusByte;
}

/**
* @brief Resets the arm back to the (0,0) position
* @param None
* @retval Status byte
*/
uint8_t resetPosition(void){
	uint8_t statusByte;
	uint8_t message = RESET_POSITION_COMMAND;
	
	statusByte = CC2500_Write(&message, TX_FIFO_BYTE_ADDRESS, 1);
	return statusByte;
}