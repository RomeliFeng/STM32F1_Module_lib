#include "Car.h"

#define Car_PWM_Resolution 1024

CarClass Car;

void CarClass::Init(CarCount CountEnable) {
	//init pwm contrl gpio PA0-PA3
	PWM.Init(Car_PWM_Resolution, Car_PWM_Resolution);

	//init en_stop and direct gpio
	Car_GPIO_Config();
	Car_Count_Config(CountEnable);
	//stop the CarClass and set direction default
	GPIO_WriteBit(GPIOB, Car_E_Stop_Pin, Bit_SET);		//free motor
	GPIO_WriteBit(GPIOA, Car_DIR1_Pin, Bit_RESET);		//default direct
	GPIO_WriteBit(GPIOA, Car_DIR2_Pin, Bit_SET);		//default direct
}

void CarClass::Run() {
	GPIO_WriteBit(GPIOB, Car_E_Stop_Pin, Bit_SET);		//free motor
}

void CarClass::Stop() {
	GPIO_WriteBit(GPIOB, Car_E_Stop_Pin, Bit_RESET);		//freeze motor
}

void CarClass::SetSpeed(double Speed) {
	SetSpeed(CarChLeft, Speed);
	SetSpeed(CarChRight, Speed);
}

void CarClass::SetSpeed(CarCh Ch, double Speed) {
	switch (Ch) {
	case CarChLeft:
		if (Speed < 0) {
			Speed = -Speed;
			LeftDir = false;
			GPIO_WriteBit(GPIOA, Car_DIR1_Pin, Bit_SET);	//exchange direct
		} else {
			LeftDir = true;
			GPIO_WriteBit(GPIOA, Car_DIR1_Pin, Bit_RESET);		//default direct
		}
		PWM.Set(PWMCh1, Car_PWM_Resolution - (Speed * Car_PWM_Resolution));
		break;
	case CarChRight:
		if (Speed < 0) {
			Speed = -Speed;
			RightDir = false;
			GPIO_WriteBit(GPIOA, Car_DIR2_Pin, Bit_RESET);
		} else {
			RightDir = true;
			GPIO_WriteBit(GPIOA, Car_DIR2_Pin, Bit_SET);		//default direct
		}
		PWM.Set(PWMCh2, Car_PWM_Resolution - (Speed * Car_PWM_Resolution));
		break;
	default:
		break;
	}
}

void Car_GPIO_Config() {
	GPIO_InitTypeDef GPIO_InitStructure;
	/*
	 * PB0---E_STOP
	 * PA0---PWM1		//init in pwm.cpp
	 * PA1---PWM2		//init in pwm.cpp
	 * PA2---DIR1
	 * PA3---DIR2
	 * PA4---OUT1
	 * PA5---OUT2
	 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = Car_E_Stop_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = Car_DIR1_Pin | Car_DIR2_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void Car_Count_Config(CarCount CountEnable) {
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = Car_OUT1_Pin | Car_OUT1_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	if (CountEnable == CarCountLeft || CountEnable == CarCountBoth) {
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
		EXTI_InitStructure.EXTI_Line = EXTI_Line4;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_Init(&EXTI_InitStructure);

		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
		NVIC_Init(&NVIC_InitStructure);
	}
	if (CountEnable == CarCountRight || CountEnable == CarCountBoth) {
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
		EXTI_InitStructure.EXTI_Line = EXTI_Line5;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_Init(&EXTI_InitStructure);

		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
		NVIC_Init(&NVIC_InitStructure);
	}
}

extern "C" void EXTI4_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line4)) {
		if (Car.LeftDir)
			++Car.LeftCount;
		else
			--Car.LeftCount;
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

extern "C" void EXTI9_5_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line5)) {
		if (Car.RightDir)
			++Car.RightCount;
		else
			--Car.RightCount;
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
}

