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
	uint8_t bigMessage[8];
	
	// Send a message containing the shape being sent
	switch(shape){
		case TRIANGLE:
			message = TRIANGLE_COMMAND;
			bigMessage[0] = 20;
			bigMessage[1] = 21;
			bigMessage[2] = 22;
			bigMessage[3] = 23;
			bigMessage[4] = 24;
			bigMessage[5] = 25;
			bigMessage[6] = 26;
			bigMessage[7] = 27;
			break;
		
		case SQUARE:
			message = SQUARE_COMMAND;
			break;
		
		case RECTANGLE:
			message = RECTANGLE_COMMAND;
			break;
	}
	
	//statusByte = CC2500_Write(&message, TX_FIFO_BYTE_ADDRESS , 1);	
	statusByte = CC2500_Write(bigMessage, TX_FIFO_BURST_ADDRESS, 8);
	
	return statusByte;
}

/**
* @brief Sends a command to draw a free drawing
* @param list of directions to draw
* @retval Status byte
*/
uint8_t sendFreeDraw(uint8_t *directions){
	uint8_t statusByte;
	uint8_t message;
	int count = 0;
	
	// Iterate through the directions in the buffer 
	while(directions[count] != 0x00){
		message = directions[count];
		statusByte = CC2500_Write(&message, TX_FIFO_BYTE_ADDRESS, 1);
	}
	
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