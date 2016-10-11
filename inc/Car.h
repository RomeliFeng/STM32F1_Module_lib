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

#define Car_E_Stop_Pin GPIO_Pin_0
#define Car_DIR1_Pin GPIO_Pin_2
#define Car_DIR2_Pin GPIO_Pin_3
#define Car_OUT1_Pin GPIO_Pin_4
#define Car_OUT2_Pin GPIO_Pin_5

typedef enum _CarCh {
	CarChLeft, CarChRight		//PWMCh2=left PWMCh3=right
} CarCh;

class CarClass {
public:
	void Init();
	void SetSpeed(double Speed);
	void SetSpeed(CarCh Ch, double Speed);
	void Run();
	void Stop();
private:
};

void Car_GPIO_Config();

extern CarClass Car;

#endif
