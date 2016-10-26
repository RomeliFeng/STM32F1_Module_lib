/*
 * HMC.h
 *
 *  Created on: 2016Äê10ÔÂ24ÈÕ
 *      Author: Romeli
 */

#ifndef HMC_H_
#define HMC_H_

#include "stm32f10x.h"

typedef struct {
	int16_t X;
	int16_t Y;
	int16_t Z;
	double Angle;
} _HMCData;

class HMCClass {
public:
	void Init();
	void RefreshData();
	int Xoff=0;
	int Yoff=0;
	int Zoff=0;
};

extern _HMCData HMCData;
extern HMCClass HMC;

#endif /* HMC_H_ */
