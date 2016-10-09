/*
 * MPU.h
 *
 *  Created on: Aug 8, 2016
 *      Author: Romeli
 */

#ifndef MPU_H_
#define MPU_H_

#include "stm32f10x.h"

typedef struct {
	int16_t X;
	int16_t Y;
	int16_t Z;
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
