/*
 * LED.h
 *
 *  Created on: 2016Äê8ÔÂ12ÈÕ
 *      Author: Romeli
 */

#ifndef LED_H_
#define LED_H_

#include "stm32f10x.h"

typedef enum _LEDColor
{
	LED_Off = -1, LED_Green, LED_Red
} LEDColor;

void LED_Init();
void LED_Turn(LEDColor color);

#endif /* LED_H_ */
