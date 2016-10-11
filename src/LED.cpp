/*
 * LED.cpp
 *
 *  Created on: 2016Äê8ÔÂ12ÈÕ
 *      Author: Romeli
 */
#include "LED.h"

void LED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_SET);
}

void LED_Turn(LEDColor color)
{
	switch (color)
	{
	case LEDColorGreen:
		GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_RESET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_SET);
		break;
	case LEDColorRed:
		GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_RESET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_SET);
		break;
	case LEDColorOff:
		GPIO_WriteBit(GPIOB, GPIO_Pin_8, Bit_SET);
		GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_SET);
		break;
	default:
		break;
	}
}
