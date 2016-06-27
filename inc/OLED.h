/*
 * OLED.h
 *
 *  Created on: 2016Äê5ÔÂ22ÈÕ
 *      Author: Romeli
 *  Depend on: "Delay.h" & "SPI.h"
 */

#ifndef OLED_H_
#define OLED_H_

#include "stm32f10x.h"

#define I2C_Intface
//#define SPI_Intface

#ifdef I2C_Intface
#define OLED_Address 0x78
#endif
#ifdef SPI_Intface
#define RCC_APB2Periph_OLED_GPIOX RCC_APB2Periph_GPIOB
#define OLED_Reset GPIO_Pin_1
#define OLED_GPIOX GPIOB
#define OLED_DC GPIO_Pin_0

void OLED_GPIO_Init();
#endif

typedef enum
{
	Cmd_Mode = 0, Data_Mode = 1
} WriteMode;

typedef enum
{
	C6x8 = 6, C8x16 = 8, C16x16 = 16
} CharMode;

class OLED
{
public:

	static void init();
	static void write(uint8_t data, WriteMode mode);
	static void set_pos(uint8_t x, uint8_t y);
	static void fill(uint8_t bmp);
	static void print_c(uint8_t x, uint8_t y, char data, CharMode mode);
	static void print(uint8_t x, uint8_t y, char *str, CharMode mode);
	static void print(uint8_t x, uint8_t y, int num, CharMode mode);
	static void print(uint8_t x, uint8_t y, long num, CharMode mode);
	static void print(uint8_t x, uint8_t y, float f, CharMode mode);
	static void print(uint8_t x, uint8_t y, double lf, CharMode mode);
};

#endif /* OLED_H_ */
