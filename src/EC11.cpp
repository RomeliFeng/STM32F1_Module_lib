/*
 * EC11.cpp
 *
 *  Created on: 2016��6��28��
 *      Author: Romeli
 */
#include "EC11.h"
#include "Delay.h"

uint8_t EC11::WiseFlag = 0;	//0:Clockwise	1:AntiClockwise

void EC11::Init() {
	GPIOInit();
	EXTIInit();
	NVICInit();
}

void EC11::GPIOInit() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource2);
}

void EC11::EXTIInit() {
	EXTI_InitTypeDef EXTI_InitStructure;

	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
}

void EC11::NVICInit() {
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_Init(&NVIC_InitStructure);
}

void __attribute__((weak)) EC11::Click() {
	return;
}

void __attribute__((weak)) EC11::Press() {
	return;
}

void __attribute__((weak)) EC11::Clockwise() {
	return;
}

void __attribute__((weak)) EC11::Anticlockwise() {
	return;
}

extern "C" {

void EXTI0_IRQHandler(void) {
	uint8_t Flag;
	if (EXTI_GetITStatus(EXTI_Line0) == SET) {
		Flag = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)
				^ GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);
		if (Flag == EC11::WiseFlag) {
			if (EC11::WiseFlag == 0)
				EC11::Anticlockwise();
			else
				EC11::Clockwise();
		}
		EC11::WiseFlag = Flag;
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void EXTI2_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line2) == SET) {
		uint32_t last = millis();
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2) == Bit_RESET) {
			if (millis() - last >= 1000) {
				EC11::Press();
				EXTI_ClearITPendingBit(EXTI_Line2);
				return;
			}
		}
		EC11::Click();
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
}

}

void fun() {
}
