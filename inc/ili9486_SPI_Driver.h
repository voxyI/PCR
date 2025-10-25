/*
 * Definitions and function prototypes for the correspounding source file.
 *
 * @Author Ben Rappleye
 */

#ifndef INC_ILI9486_H_
#define INC_ILI9486_H_

#include <stdbool.h>
#include <stdint.h>

#define DC_PIN				SPI1_DC_Pin
#define DC_PORT 			SPI1_DC_GPIO_Port

#define CS_PIN				SPI1_NSS_Pin
#define CS_PORT				SPI1_NSS_GPIO_Port

#define RESET_PIN			SPI1_RESET_Pin
#define RESET_PORT			SPI1_RESET_GPIO_Port

#define LED_PIN				LED_H_Pin //This and the associated function can be left out if not needed
#define LED_PORT			LED_H_GPIO_Port

#define TIMEOUT				1000

void LCD_Init();
void hard_reset();
void soft_reset();
void send_data(uint8_t data);
void send_command(uint8_t command);
void send_frame_buffer(uint8_t *frame, uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
void set_region(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
void set_pixel(uint16_t x, uint16_t y, uint16_t color);
void format_frame_buffer(uint8_t *buffer, uint16_t length);
void lcd_on(bool state);

#endif /* INC_ILI9486_H_ */
