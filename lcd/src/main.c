/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "stm32f4xx.h"                  // Device header

#include "stm32f4xx_conf.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_l3gd20.h"
#include "background16bpp.h"

#include "keypad.h"
#include "commands.h"
#include "kstate.h"
#include "accelerometer.h"

#include "../../receiver/src/spi.h"
#include "../../receiver/src/cc2500.h"

#include <stdio.h>
#include <string.h>

/* Mode variable indicates predetermined shapes or on the fly */
/* 0 = shapes; 1 = on the fly; 2 = reset position */
uint8_t mode;

// Shape variable selects the shape
/* 0 = Square; 1 = Rectangle; 2 = Triangle;*/
uint8_t shape;

uint8_t flash_display=1;

/* Direction variable keeps track of which direction free form drawing is going */
/* 0 = UP; 1 = DOWN; 2 = LEFT ; 3 = RIGHT */
uint8_t direction;
uint8_t linesDrawn[DIRECTION_BUFFER_SIZE];
int numDirections = 0;

// Define mutexes
osMutexDef(Mutex_Directions);				
osMutexId num_directions_mutex;

//Thread prototypes
void displayThreadDef(void const *argument);
void keypadThreadDef(void const *argument);
void transmitterThreadDef(void const *argument);
void accelerometerThreadDef(void const *argument);


//Thread declarations
osThreadDef(displayThreadDef, osPriorityNormal, 1, 0);
osThreadDef(keypadThreadDef, osPriorityNormal, 1, 0);
osThreadDef(transmitterThreadDef, osPriorityNormal, 1, 0);
osThreadDef(accelerometerThreadDef, osPriorityAboveNormal, 1, 0);

osThreadId display_thread;
osThreadId keypad_thread;
osThreadId transmitterThread;
osThreadId accelerometerThread;

float roll;
float pitch;

#define KEYPAD_FLAG 0x01
#define TRANSMITTER_FLAG 0x01
#define ACCELEROMETER_FLAG 0x01


static void delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = 100000*nCount; index != 0; index--)
  {
  }
}

void displayThreadDef(void const *argument){
	
	while(1){
		// Clear the display and write group info at top
		LCD_SetFont(&Font8x8);
		LCD_SetTextColor(LCD_COLOR_BLACK);
		LCD_DisplayStringLine(LINE(0), (uint8_t*)"          uP Group 3           ");
		LCD_DrawLine(0, 12, 240, LCD_DIR_HORIZONTAL);
		LCD_SetBackColor(LCD_COLOR_WHITE);
		LCD_SetTextColor(LCD_COLOR_BLUE);
		LCD_SetFont(&Font12x12);
		
		// Draw display based on current mode
		
		if(mode == SHAPE_MODE){
			switch(shape){
				case SQUARE:
					LCD_DisplayStringLine(LINE(5), (uint8_t*)"Now drawing :     ");
					LCD_DrawFullRect(95, 135, 50,50);
					LCD_DisplayStringLine(LINE(20), (uint8_t*)"       SQUARE        ");
					break;
					
				case RECTANGLE:
					LCD_DisplayStringLine(LINE(5), (uint8_t*)"Now drawing :     ");
					LCD_DrawFullRect(75, 135, 70,50);
					LCD_DisplayStringLine(LINE(20), (uint8_t*)"     RECTANGLE        ");
					break;
				
				case TRIANGLE:
					LCD_DisplayStringLine(LINE(5), (uint8_t*)"Now drawing :     ");
					LCD_FillTriangle(95, 120, 145, 185, 135, 185);
					LCD_DisplayStringLine(LINE(20), (uint8_t*)"      TRIANGLE        ");
					break;
			}
		}
		else{
			LCD_SetFont(&Font8x8);
			LCD_DisplayStringLine(LINE(5), (uint8_t*)"In free draw mode,     ");
			LCD_DisplayStringLine(LINE(6), (uint8_t*)"Shape:     ");
			LCD_SetFont(&Font16x24);	
			

			int count = 0;
			uint16_t x_start = 120;
			uint16_t y_start = 200;
			uint16_t length = 30;		

			// Display lines that have already been drawn on the page
			while(linesDrawn[count] != 0){
				
				uint8_t line = linesDrawn[count];
				uint8_t direction;
				
				// If the line is not diagonal
				if(line == UP || line == DOWN || line == LEFT || line == RIGHT){
					
					// Choose the direction of the line
					if(line == UP || line == DOWN){
						direction = LCD_DIR_VERTICAL;
					} else{
						direction = LCD_DIR_HORIZONTAL;
					}
					
					// Draw the line
					switch(line){
						case UP:
							y_start -= length;
							break;
						case LEFT:
							x_start -= length;
							break;
					}
					LCD_DrawLine(x_start, y_start, length, direction);			
					
					// Update starting point for the next line segment
					switch(line){
						case DOWN:
							y_start += length;
							break;
						case RIGHT:
							x_start += length;
							break;
					}
				}
				
				
				// The line IS diagonal
				else{
					uint16_t x_end = x_start;
					uint16_t y_end = y_start;
					switch(line){
						case UP_LEFT:
							x_end -= length;
							y_end -= length;
							break;
						case UP_RIGHT:
							x_end += length;
							y_end -= length;
							break;
						case DOWN_LEFT:
							x_end -= length;
							y_end += length;
							break;
						case DOWN_RIGHT:
							x_end += length;
							y_end += length;
							break;
					}
					LCD_DrawUniLine(x_start, y_start, x_end, y_end);
					
					// Update starting point for the next line segment
					x_start = x_end;
					y_start = y_end;
				}
				count++;
			}
			
			
			// Display a flashing line that has not yet been sent to the page
			int x_draw = x_start;
			int y_draw = y_start;
			int x_end = x_start;
			int y_end = y_start;
			
			int x_clear = x_start;
			int y_clear = y_start;
			switch(direction){
				case UP:
					y_draw -= length;
					LCD_DrawLine(x_draw, y_draw, length, LCD_DIR_VERTICAL);
					break;
				case DOWN:
					LCD_DrawLine(x_draw, y_draw, length, LCD_DIR_VERTICAL);
					break;
				case LEFT:
					x_draw -= length;
					LCD_DrawLine(x_draw, y_draw, length, LCD_DIR_HORIZONTAL);
					break;
				case RIGHT:
					LCD_DrawLine(x_draw, y_draw, length, LCD_DIR_HORIZONTAL);
					break;
				
				case UP_LEFT:
					x_end -= length;
					y_end -= length;
					LCD_DrawUniLine(x_draw, y_draw, x_end, y_end);
					break;
				case UP_RIGHT:
					x_end += length;
					y_end -= length;
					LCD_DrawUniLine(x_draw, y_draw, x_end, y_end);
					break;
				case DOWN_LEFT:
					x_end -= length;
					y_end += length;
					LCD_DrawUniLine(x_draw, y_draw, x_end, y_end);
					break;
				case DOWN_RIGHT:
					x_end += length;
					y_end += length;
					LCD_DrawUniLine(x_draw, y_draw, x_end, y_end);
					break;
				default:
					break;
			}
			
			// Wait 300ms and clear the last segment
			osDelay(300);

			LCD_SetTextColor(LCD_COLOR_WHITE);
			switch(direction){
				case UP:
					y_clear -= length;
					LCD_DrawLine(x_clear, y_clear, length, LCD_DIR_VERTICAL);
					break;
				case DOWN:
					LCD_DrawLine(x_clear, y_clear, length, LCD_DIR_VERTICAL);
					break;
				case LEFT:
					x_clear -= length;
					LCD_DrawLine(x_clear, y_clear, length, LCD_DIR_HORIZONTAL);
					break;
				case RIGHT:
					LCD_DrawLine(x_clear, y_clear, length, LCD_DIR_HORIZONTAL);
					break;
				
				case UP_LEFT:
					x_end -= length;
					y_end -= length;
					LCD_DrawUniLine(x_draw, y_draw, x_end, y_end);
					break;
				case UP_RIGHT:
					x_end += length;
					y_end -= length;
					LCD_DrawUniLine(x_draw, y_draw, x_end, y_end);
					break;
				case DOWN_LEFT:
					x_end -= length;
					y_end += length;
					LCD_DrawUniLine(x_draw, y_draw, x_end, y_end);
					break;
				case DOWN_RIGHT:
					x_end += length;
					y_end += length;
					LCD_DrawUniLine(x_draw, y_draw, x_end, y_end);
					break;
				default:
					break;
			}		
		}		
		osDelay(300);
	}
}

/**
*@brief Thread to control keypad scanning/mode switching
*/
void keypadThreadDef(void const *argument){
	while(1){
		osSignalWait(KEYPAD_FLAG, osWaitForever);
		char c = get_button_pressed();
		if(c != 'e'){
			//printf("%c\n", c);
			
			// Change state/direction based on key pressed
			switch(c){
				case '1':
					shape = SQUARE;
					break;
				case '2':
					shape = RECTANGLE;
					break;
				case '3':
					shape = TRIANGLE;
					break;
				case '4':
					if(mode == FREE_DRAW_MODE){
						mode = SHAPE_MODE;
					}
					else{
						mode = FREE_DRAW_MODE;
					}
					break;
				case '6':
					direction = UP;
					break;
				case '#':
					direction = DOWN;
					break;
				case '8':
					direction = LEFT;
					break;
				case 'C':
					direction = RIGHT;
					break;
				case '5':
					direction = UP_LEFT;
					break;
				case 'B':
					direction = UP_RIGHT;
					break;
				case 'D':
					direction = DOWN_RIGHT;
					break;
				case '0':
					direction = DOWN_LEFT;
					break;
				
				// Signal the transmitter thread to send a command to the other board
				case '9':			
					// If the mode is on the fly, then save the direction into the direction buffer
					if(mode == FREE_DRAW_MODE){
						linesDrawn[numDirections] = direction;
						numDirections++;
					}
				
					osSignalSet(transmitterThread, TRANSMITTER_FLAG);
					break;
				
				// Resets the arm back to its original position and clears the free draw buffer
				case '*':
					mode = RESET_MODE;
					osSignalSet(transmitterThread, TRANSMITTER_FLAG);
					break;
				
				default:
					break;
			}
			LCD_Clear(LCD_COLOR_WHITE);
		}
		
//		else{
//			printf("keypad error?\n");
//		}
	
		osSignalClear(keypad_thread, KEYPAD_FLAG);
	}
}

/*
 * @Brief Thread to transmit instructions to the motor controller board
 */
void transmitterThreadDef(void const *argument){
	// Enable SPI1
	init_SPI1();
	
	// Variable definitions
	uint8_t statusByte;
	uint8_t readByte;
	uint8_t numBytes = 1;
	uint8_t channel = CHANNEL;

	// Reset chip and initialize registers
	statusByte = CC2500_Reset();	
	CC2500_Init_Registers();
	
	// Set the channel on which to transmit
	statusByte = CC2500_Set_Channel(&channel);
	
	// Print out the register values to ensure correctness
	CC2500_Read_Registers();	

	// Wait for the transceiver to enter transmitting mode
	statusByte = CC2500_Start_Transmit();
	
	while((statusByte & 0xF0) != TRANSMITTING){
		statusByte = CC2500_No_Op();
		printf("Status: %02x\n", statusByte);
	}
	
	uint8_t bytesAvailable;
	uint8_t message = 0x7D;
	
	while(1){
				
		// Check that the transmitter is in the transmitting state
		while((statusByte & 0xF0) == TRANSMITTING){
			
			// Wait until the user has pressed a button on the keypad
			osSignalWait(TRANSMITTER_FLAG, osWaitForever);
						
			// Send either a shape or on the fly directions
			if(mode == SHAPE_MODE){
				sendShape(shape);
			}
			else if(mode == FREE_DRAW_MODE){
				sendFreeDraw(direction);
			}
			
			// Reset the position of the arm and reset mode
			else{
				resetPosition();
				mode = SHAPE_MODE;
			}
			
			// Make sure the transmitter is still in transmitting mode
			statusByte = CC2500_No_Op();
			osSignalClear(transmitterThread, TRANSMITTER_FLAG);
		}	
		
		// Put the receiver back in receiving state
		statusByte = CC2500_Start_Transmit();
		
		// Wait for the receiver to enter the correct state
		while((statusByte & 0xF0) != TRANSMITTING){
			statusByte = CC2500_No_Op();
		}
		
		osDelay(250);
	}
}

/**
*@brief Thread to read accelerometer values
*/
void accelerometerThreadDef(void const *argument){
	float angles[2];
	// Define kalman states for each accelerometer output
	kalman_state x_state = {0.05, 0.981, 0.0, 0.0, 0.0};
	kalman_state y_state = {0.05, 0.981, 0.0, 0.0, 0.0};
	kalman_state z_state = {0.05, 0.981, 0.0, 0.0, 0.0};
	
	while(1){
		
		// Wait for the accelerometer to set an interrupt
		osSignalWait(ACCELEROMETER_FLAG, osWaitForever ); 			
			
		// Read accelerometers and set the display to the roll
		readAcc(angles, &x_state, &y_state, &z_state);
			
		// Wait for angle mutex before setting the angle
		roll = angles[0];	
		pitch = angles[1];
		
		osSignalClear(accelerometerThread, ACCELEROMETER_FLAG);
	}
}


/*
 * main: initialize and start the system
 */
int main (void) {
  osKernelInitialize ();                    // initialize CMSIS-RTOS
	
  // initialize peripherals here
	/* LCD initiatization */
  LCD_Init();
  
  /* LCD Layer initiatization */
  LCD_LayerInit();
    
  /* Enable the LTDC controler */
  LTDC_Cmd(ENABLE);
  
  /* Set LCD foreground layer as the current layer */
  LCD_SetLayer(LCD_FOREGROUND_LAYER);
	
	// Initialize keypad
	initKeypad();
	
	initAccelerometer();
	initAccelerometerInterrupt();
	
	// Create threads
	display_thread = osThreadCreate(osThread(displayThreadDef), NULL);
	keypad_thread = osThreadCreate(osThread(keypadThreadDef), NULL);
	transmitterThread = osThreadCreate(osThread(transmitterThreadDef), NULL);
	accelerometerThread = osThreadCreate(osThread(accelerometerThreadDef), NULL);
		
	// Set default mode
	mode = SHAPE_MODE;
	shape = SQUARE;
	direction = UP;
	
	osKernelStart ();                         // start thread execution 
}


/**
*@brief Interupt handler for EXTI10 to EXTI15.  Informs uP that a button on the keypad has been pressed
*@retval None
*/
void EXTI15_10_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line11|EXTI_Line12|EXTI_Line13);
	osSignalSet(keypad_thread, KEYPAD_FLAG);
}

/**
*@brief Interupt handler for EXTI5 to EXTI9.  Informs uP that a button on the keypad has been pressed
*@retval None
*/
void EXTI9_5_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line8);
	osSignalSet(keypad_thread, KEYPAD_FLAG);
}


/**
*@brief Interupt handler for EXTI0.  Informs uP that a sample is ready
*@retval None
*/
void EXTI0_IRQHandler(void)
{
	osSignalSet(accelerometerThread, ACCELEROMETER_FLAG);			// Set a signal for the accelerometer thread
	EXTI_ClearITPendingBit(EXTI_Line0); 						//Clear the EXTI0 interupt flag
}
