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

//#define I2C_Intface
#define SPI_Intface

#ifdef I2C_Intface
#define OLED_Address 0x78
#endif
#ifdef SPI_Intface

// Port numbers: 0=A, 1=B, 2=C, 3=D, 4=E, 5=F, 6=G, ...
#define OLED_DC_PORT_NUMBER               (1)
#define OLED_DC_PIN_NUMBER                (0)
#define OLED_RESET_PORT_NUMBER            (1)
#define OLED_RESET_PIN_NUMBER             (1)

#define OLED_GPIOx(_N)                 ((GPIO_TypeDef *)(GPIOA_BASE + (GPIOB_BASE-GPIOA_BASE)*(_N)))
#define OLED_PIN_MASK(_N)              (1 << (_N))
#define OLED_RCC_MASKx(_N)             (RCC_APB2Periph_GPIOA << (_N))

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
	static void print(uint8_t x, uint8_t y, float f, uint8_t ndigit,
			CharMode mode);
	static void print(uint8_t x, uint8_t y, double lf, uint8_t ndigit,
			CharMode mode);
};

#endif /* OLED_H_ */
