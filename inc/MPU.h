/*
 * MPU.h
 *
 *  Created on: Aug 8, 2016
 *      Author: Romeli
 */

#ifndef MPU_H_
#define MPU_H_

#include "stm32f10x.h"

typedef union {
	int16_t Word;
	uint8_t Byte[2];
} _WordandByte;
typedef struct {
	_WordandByte X;
	_WordandByte Y;
	_WordandByte Z;
} _MPUDataUnit;
typedef struct {
	_MPUDataUnit ACC;
	_MPUDataUnit GYRO;
} _MPUData;

class MPUClass {
public:
	void Init();
	void RefreshData();
private:
};

extern MPUClass MPU;
extern _MPUData MPUData;

#endif /* MPU_H_ */
