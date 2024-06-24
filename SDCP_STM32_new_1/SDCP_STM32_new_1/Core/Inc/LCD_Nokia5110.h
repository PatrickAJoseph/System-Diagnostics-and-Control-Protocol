/*
 * Nokia5110.h
 *
 */

#ifndef INC_NOKIA5110_H_
#define INC_NOKIA5110_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "main.h"
#include "LCD_characterBitmaps.h"

#define Nokia5110_SoftwareSPI		0
#define Nokia5110_SPI_DMAEN			1

#define Nokia5110_Normal_Display	0x0C
#define Nokia5110_Invert_Display	0x0D

struct Nokia5110{
  uint8_t byte[504];
  uint8_t x_pos;
  uint8_t y_pos;
  uint8_t updated;
  uint8_t pending;
}Nokia5110;

void Nokia5110_clear();
void Nokia5110_GPIO_Init();
void Nokia5110_SPI_Init();
void Nokia5110_SPI_TxP(uint8_t byte);
void Nokia5110_SPI_DMA_TransferByte(uint8_t byte,uint8_t type);
void Nokia5110_writeByte(uint8_t byte, uint8_t type);
void Nokia5110_updateDisplay();
void Nokia5110_Init();
void Nokia5110_setPosition(uint8_t x_position, uint8_t y_position);
void Nokia5110_printASCII(char character);
void Nokia5110_printString(char* str);
void Nokia5110_printU8(uint8_t n);
void Nokia5110_printU16(uint16_t n);
void Nokia5110_printU32(uint32_t n);
void Nokia5110_printU32_NB(uint32_t n,uint8_t d);
void Nokia5110_printI8(int8_t n);
void Nokia5110_printI16(int16_t n);
void Nokia5110_printI32(int32_t n);
void Nokia5110_printFloat(float n, uint8_t p);
void Nokia5110_drawHorizontalLine(uint16_t xpxl_pos,
		                     uint16_t ypxl_pos,
		                     uint8_t length, uint8_t width);
void Nokia5110_drawVerticalLine(uint16_t xpxl_pos,
		                     uint16_t ypxl_pos,
		                     uint8_t length, uint8_t width);
void Nokia5110_drawRectangle(uint16_t xpxl_pos, uint16_t ypxl_pos,
		                     uint16_t length, uint16_t width, uint16_t borderWidth);
void Nokia5110_drawBitmap(uint8_t* buffer,
		                  uint16_t x_org, uint16_t y_org,
						  uint16_t x_len, uint16_t y_len);






#endif /* INC_NOKIA5110_H_ */
