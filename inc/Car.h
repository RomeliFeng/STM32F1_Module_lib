/*
 * Car.cpp
 *
 *  Created on: 2016Äê5ÔÂ22ÈÕ
 *      Author: Romeli
 *  Depend on: "PWM.h"
 */
#ifndef _Car_h_
#define _Car_h_

#include "stm32f10x.h"

#define EN_Stop GPIOC,GPIO_Pin_14
#define Direct GPIOC,GPIO_Pin_15



class Car
{
public:
	Car();
	static void init();
	static void run(uint16_t speed);
	static void run(uint8_t ch, uint16_t speed);
	static void stop();
	static void direct(uint8_t dir);
	~Car();

private:
};

void Car_GPIO_Config();

#endif
