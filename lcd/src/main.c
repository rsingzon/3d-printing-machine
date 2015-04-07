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

#include "../../receiver/src/spi.h"
#include "../../receiver/src/cc2500.h"

#include <stdio.h>
#include <string.h>

/* State variable indicates what to write to display */
/* 0 = Square; 1 = Rectangle; 2 = Triangle; 3 = free draw */
int state;

/* Direction variable keeps track of which direction free form drawing is going */
/* 0 = UP; 1 = DOWN; 2 = LEFT ; 3 = RIGHT */
int direction;

//Thread prototypes
void displayThreadDef(void const *argument);
void keypadThreadDef(void const *argument);
void transmitterThreadDef(void const *argument);


//Thread declarations
osThreadDef(displayThreadDef, osPriorityNormal, 1, 0);
osThreadDef(keypadThreadDef, osPriorityNormal, 1, 0);
osThreadDef(transmitterThreadDef, osPriorityNormal, 1, 0);

osThreadId display_thread;
osThreadId keypad_thread;
osThreadId transmitterThread;

#define KEYPAD_FLAG 0x01
#define TRANSMITTER_FLAG 0x01


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
		LCD_Clear(LCD_COLOR_WHITE);
		LCD_SetFont(&Font8x8);
		LCD_SetTextColor(LCD_COLOR_BLACK);
		LCD_DisplayStringLine(LINE(0), (uint8_t*)"          uP Group 3           ");
		LCD_DrawLine(0, 12, 240, LCD_DIR_HORIZONTAL);
		LCD_SetBackColor(LCD_COLOR_WHITE);
		LCD_SetTextColor(LCD_COLOR_BLUE);
		LCD_SetFont(&Font12x12);
		
		// Draw display based on current state
		switch(state){
			case 0 :
				LCD_DisplayStringLine(LINE(5), (uint8_t*)"Now drawing :     ");
				LCD_DrawFullRect(95, 135, 50,50);
				LCD_DisplayStringLine(LINE(20), (uint8_t*)"       SQUARE        ");
				break;
				
			case 1:
				LCD_DisplayStringLine(LINE(5), (uint8_t*)"Now drawing :     ");
				LCD_DrawFullRect(75, 135, 70,50);
				LCD_DisplayStringLine(LINE(20), (uint8_t*)"     RECTANGLE        ");
				break;
			
			case 2:
				LCD_DisplayStringLine(LINE(5), (uint8_t*)"Now drawing :     ");
				LCD_FillTriangle(95, 120, 145, 185, 135, 185);
				LCD_DisplayStringLine(LINE(20), (uint8_t*)"      TRIANGLE        ");
				break;
			
			case 3:
				LCD_SetFont(&Font8x8);
				LCD_DisplayStringLine(LINE(5), (uint8_t*)"In free draw mode,     ");
				LCD_DisplayStringLine(LINE(6), (uint8_t*)"current direction is:     ");
				LCD_SetFont(&Font16x24);
				switch(direction){
					case 0:
						LCD_DisplayStringLine(LINE(6), (uint8_t*)"      UP          ");
						break;
					case 1:
						LCD_DisplayStringLine(LINE(6), (uint8_t*)"     DOWN          ");
						break;
					case 2:
						LCD_DisplayStringLine(LINE(6), (uint8_t*)"     LEFT          ");
						break;
					case 3:
						LCD_DisplayStringLine(LINE(6), (uint8_t*)"     RIGHT          ");
						break;
					default:
						break;
				}
				break;
				
			default:
				break;
		}
		
		osDelay(250);
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
			printf("%c\n", c);
			
			// Change state/direction based on key pressed
			switch(c){
				case '1':
					state = TRIANGLE;
					break;
				case '2':
					state = SQUARE;
					break;
				case '3':
					state = RECTANGLE;
					break;
				case '4':
					state = FREE_DRAW_MODE;
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
				default:
					break;
			}
		}
		
		// Signal the transmitter thread to send a command to the other board
		osSignalSet(transmitterThread, TRANSMITTER_FLAG);
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
	uint8_t channel = 5;

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
			
			// If the FIFO has space available available, transmit
			statusByte = CC2500_Read(&bytesAvailable, TX_BYTES, 2);
			
			
			
			if(bytesAvailable < 5){
				statusByte = CC2500_Write(&message, TX_FIFO_BYTE_ADDRESS , 1);
				printf("Data sent: %02x\n", message);
			}
			
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
	
	// Create threads
	display_thread = osThreadCreate(osThread(displayThreadDef), NULL);
	keypad_thread = osThreadCreate(osThread(keypadThreadDef), NULL);
	transmitterThread = osThreadCreate(osThread(transmitterThreadDef), NULL);
		
	state  = 3;
	direction = 2;
	
	osKernelStart ();                         // start thread execution 
}


/**
*@brief Interupt handler for EXTI10 to EXTI15.  Informs uP that a button on the keypad has been pressed
*@retval None
*/
void EXTI15_10_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line12|EXTI_Line13|EXTI_Line14|EXTI_Line15);
	osSignalSet(keypad_thread, KEYPAD_FLAG);
}


