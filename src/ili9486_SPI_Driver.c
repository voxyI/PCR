/**
 * Driver for interfacing with the ILI9486 over 4-wire SPI.
 * Built for the stm32l476rg on SPI1. If another SPI line
 * must be used, change all instances of "&hspi1" to the
 * address of the SPI handel you are using.
 *
 * Define DC_PORT, DC_PIN, CS_PIN, CS_PORT, RESET_PIN, RESET_PORT, LED_PIN, LED_Port in header file
 *
 * @author Ben Rappleye
 */

#include "main.h"
#include "ili9486.h"

extern SPI_HandleTypeDef hspi1;

/**
 * Initialize the LCD
 */
void LCD_Init(){
	send_command(0xb0);
	send_data(0x00);
	// Interface Pixel Format, 16 bits / pixel
	send_command(0x3A);
	send_data(0x55); // 5D
	// PGAMCTRL(Positive Gamma Control)
	send_command(0xE0);
	send_data(0x0F);
	send_data(0x1F);
	send_data(0x1C);
	send_data(0x0C);
	send_data(0x0F);
	send_data(0x08);
	send_data(0x48);
	send_data(0x98);
	send_data(0x37);
	send_data(0x0A);
	send_data(0x13);
	send_data(0x04);
	send_data(0x11);
	send_data(0x0D);
	send_data(0x00);
	// NGAMCTRL (Negative Gamma Correction)
	send_command(0xE1);
	send_data(0x0F);
	send_data(0x32);
	send_data(0x2E);
	send_data(0x0B);
	send_data(0x0D);
	send_data(0x05);
	send_data(0x47);
	send_data(0x75);
	send_data(0x37);
	send_data(0x06);
	send_data(0x10);
	send_data(0x03);
	send_data(0x24);
	send_data(0x20);
	send_data(0x00);
	// Digital Gamma Control 1
	send_command(0xE2);
	send_data(0x0F);
	send_data(0x32);
	send_data(0x2E);
	send_data(0x0B);
	send_data(0x0D);
	send_data(0x05);
	send_data(0x47);
	send_data(0x75);
	send_data(0x37);
	send_data(0x06);
	send_data(0x10);
	send_data(0x03);
	send_data(0x24);
	send_data(0x20);
	send_data(0x00);
	// Set rotation
	
	// Idle mode control + Power +  Frame rate ctrl
	
	// Display Inversion Control
	send_command(0xB4);
	send_data(0x02); // 2 dot inversion /-/ disabled | 0x12 to enable
	// Display Function Control
	send_command(0xB6);
	send_data(0x02);
	send_data(0x22);
	send_data(0x3B);
	// # Sleep OUT
	send_command(0x11);
	HAL_Delay(150);
	// Display ON
	send_command(0x29);
}

/**
 * Send a data byte to the LCD controller
 *
 * @param data
 * The byte to be sent to the LCD controller
 */
void send_data(uint8_t data){
	HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_SET); //set DC pin high for transmitting data
	if(HAL_SPI_Transmit(&hspi1, &data, 1, TIMEOUT) != HAL_OK){
		Error_Handler();
	}
}

/**
 * send a command byte to the LCD controller
 *
 * @param command
 * the byte to be sent to the LCD controller to be read as a command
 */
void send_command(uint8_t command){
	HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_RESET); //set DC pin low for transmitting commands
	if(HAL_SPI_Transmit(&hspi1, &command, 1, TIMEOUT) != HAL_OK){
		Error_Handler();
	}
}

/**
 * Sets the region in GRAM to be written to
 *
 * @param xStart
 * @param xEnd
 * @param yStart
 * @param yEnd
 */
void set_region(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd){
	send_command(0x2bu);				//set row address command
	send_data((uint8_t) xStart>>8);
	send_data((uint8_t) xStart);
	send_data((uint8_t) xEnd>>8);
	send_data((uint8_t) xEnd);

	send_command(0x2au);				//set column address command
	send_data((uint8_t) yStart>>8);
	send_data((uint8_t) yStart);
	send_data((uint8_t) yEnd>>8);
	send_data((uint8_t) yEnd);
}

/**
 * Writes the frame buffer in the region specified
 *
 * @param frame
 * The frame buffer to be written
 *
 * @param xStart
 * @param xEnd
 * @param yStart
 * @param yEnd
 */
void send_frame_buffer(uint8_t *frame, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd){
	set_region(xStart, yStart, xEnd, yEnd);
	format_frame_buffer(frame, (xEnd - xStart) * (yStart - yEnd) * 2);

	//Transmit bulk data over DMA to free the CPU during transfer
	HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *) frame, (xEnd - xStart) * (yStart - yEnd) * 2);
}

/**
 * Used to set a single pixel on the LCD
 *
 * @param x
 * @param y
 * @param color
 * The color to set the pixel to in RGB565
 */
void set_pixel(uint16_t x, uint16_t y, uint16_t color){
	set_region(x, y, x, y);
	uint8_t data[3];
	data[0] = (uint8_t) color>>16;
	data[1] = (uint8_t) color>>8;
	data[2] = (uint8_t) color;
	if(HAL_SPI_Transmit(&hspi1, data, 3, TIMEOUT) != HAL_OK){
		Error_Handler();
	}
}

/**
 * Formats the frame buffer so that the MSB is first by moving the second byte infront of the first
 *
 * @param pixles
 * The frame buffer to be formatted
 *
 * @param length
 * the length of the frame buffer
 */
void format_frame_buffer(uint8_t *buffer, uint16_t length) {
	while(length > 0) {
		uint8_t temp = *buffer;
		*buffer = *(buffer+1);
		*(buffer+1) = temp;
		length = length - 2;
	}
}

/**
 * Toggles the LCD backlight on or off
 *
 * @param state
 * true = backlight on
 * false = backlight off
 */
void set_backlight(bool state){
	if(state){
		HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
	}
}
