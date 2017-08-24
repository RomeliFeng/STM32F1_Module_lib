/*
 * OLED.h
 *
 *  Created on: 2016��5��22��
 *      Author: Romeli
 *  Depend on: "Delay.h" & "SPI.h"
 */

#ifndef OLED_H_
#define OLED_H_

#include "cmsis_device.h"
#include "Parse.h"

//#define I2C_Intface
#define SPI_Intface

#ifdef I2C_Intface
#define OLED_Address 0x78
#endif

typedef enum _WriteMode_Typedef {
	WriteMode_Cmd = 0, WriteMode_Data = 1
} WriteMode_Typedef;

typedef enum _CharMode_Typedef {
	CharMode_6x8 = 6, CharMode_8x16 = 8, CharMode_16x16 = 16
} CharMode_Typedef;

class OLED: Parse {
public:
	static void Init();
	static void Write(uint8_t data, WriteMode_Typedef mode);
	static void SetPos(uint8_t x, uint8_t y);
	static void Fill(uint8_t bmp);
	static void Print_c(uint8_t x, uint8_t y, uint8_t data,
			CharMode_Typedef mode = CharMode_6x8);
	static void Print(uint8_t x, uint8_t y, uint8_t *str,
			CharMode_Typedef mode = CharMode_6x8);
	static void Print(uint8_t x, uint8_t y, const char *str,
			CharMode_Typedef mode = CharMode_6x8) {
		Print(x, y, (uint8_t*) str, mode);
	}
	static void Print(uint8_t x, uint8_t y, int32_t num, CharMode_Typedef mode =
			CharMode_6x8);

	static inline void Print(uint8_t x, uint8_t y, char num,
			CharMode_Typedef mode = CharMode_6x8) {
		Print(x, y, (int32_t) num, mode);
	}
	static void Print(uint8_t x, uint8_t y, int8_t num, CharMode_Typedef mode =
			CharMode_6x8) {
		Print(x, y, (int32_t) num, mode);
	}
	static void Print(uint8_t x, uint8_t y, uint8_t num, CharMode_Typedef mode =
			CharMode_6x8) {
		Print(x, y, (int32_t) num, mode);
	}
	static void Print(uint8_t x, uint8_t y, int num, CharMode_Typedef mode =
			CharMode_6x8) {
		Print(x, y, (int32_t) num, mode);
	}
	static void Print(uint8_t x, uint8_t y, int16_t num, CharMode_Typedef mode =
			CharMode_6x8) {
		Print(x, y, (int32_t) num, mode);
	}
	static void Print(uint8_t x, uint8_t y, uint16_t num,
			CharMode_Typedef mode = CharMode_6x8) {
		Print(x, y, (int32_t) num, mode);
	}
	static void Print(uint8_t x, uint8_t y, uint32_t num,
			CharMode_Typedef mode = CharMode_6x8) {
		Print(x, y, (int32_t) num, mode);
	}

	static void Print(uint8_t x, uint8_t y, double lf, uint8_t ndigit,
			CharMode_Typedef mode = CharMode_6x8);

	static void Print(uint8_t x, uint8_t y, float f, uint8_t ndigit,
			CharMode_Typedef mode = CharMode_6x8) {
		Print(x, y, (double) f, ndigit, mode);
	}

private:
	static void GPIOInit();
};

#endif /* OLED_H_ */
