/*
 * CarClass.cpp
 *
 *  Created on: 2016Äê5ÔÂ22ÈÕ
 *      Author: Romeli
 *  Depend on: "PWM.h"
 */
#ifndef _Car_h_
#define _Car_h_

#include <stm32f10x.h>
#include "PWM.h"
#include "Delay.h"

#define Car_E_Stop_Pin GPIO_Pin_0
#define Car_DIR1_Pin GPIO_Pin_2
#define Car_DIR2_Pin GPIO_Pin_3
#define Car_OUT1_Pin GPIO_Pin_4
#define Car_OUT2_Pin GPIO_Pin_5

typedef enum _CarCh {
	CarChLeft, CarChRight
} CarCh;
typedef enum _CarCount {
	CarCountLeft, CarCountRight, CarCountBoth
} CarCount;

class CarClass {
public:
	void Init(CarCount CountEnable);
	void SetSpeed(double Speed);
	void SetSpeed(CarCh Ch, double Speed);
	void Run();
	void Stop();
	bool LeftDir;
	bool RightDir;
	int32_t LeftCount = 0;
	int32_t RightCount = 0;
private:
};

void Car_GPIO_Config();
void Car_Count_Config(CarCount CountEnable);

extern CarClass Car;

#endif
