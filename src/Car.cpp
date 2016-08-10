#include "Car.h"

#define Car_PWM_Resolution 10000.0

CarClass Car;

void CarClass::Init() {
	//init pwm contrl gpio PA0-PA3
	PWM.Init(Car_PWM_Resolution, Car_PWM_Resolution);

	//init en_stop and direct gpio
	Car_GPIO_Config();

	//stop the CarClass and set direction default
	GPIO_WriteBit(GPIOA, Car_E_Stop_Pin, Bit_SET);		//free motor
	GPIO_WriteBit(GPIOA, Car_DIR1_Pin, Bit_RESET);		//default direct
	GPIO_WriteBit(GPIOA, Car_DIR2_Pin, Bit_SET);		//default direct
}

void CarClass::Stop() {
	GPIO_WriteBit(GPIOA, Car_E_Stop_Pin, Bit_RESET);		//freeze motor
}

void CarClass::SetSpeed(double Speed) {
	SetSpeed(Car_Motor_Left, Speed);
	SetSpeed(Car_Motor_Right, Speed);
}

void CarClass::SetSpeed(CarCh Ch, double Speed) {
	switch (Ch) {
	case Car_Motor_Left:
		if (Speed < 0) {
			Speed = -Speed;
			GPIO_WriteBit(GPIOA, Car_DIR1_Pin, Bit_SET);	//exchange direct
		} else {
			GPIO_WriteBit(GPIOA, Car_DIR1_Pin, Bit_RESET);		//default direct
		}
		PWM.Set(PWMCh2, Car_PWM_Resolution - Speed * Car_PWM_Resolution);
		break;
	case Car_Motor_Right:
		if (Speed < 0) {
			Speed = -Speed;
			GPIO_WriteBit(GPIOA, Car_DIR2_Pin, Bit_RESET);
		} else {
			GPIO_WriteBit(GPIOA, Car_DIR2_Pin, Bit_SET);		//default direct
		}
		PWM.Set(PWMCh3, Car_PWM_Resolution - Speed * Car_PWM_Resolution);
		break;
	default:
		break;
	}
}

void Car_GPIO_Config() {
	GPIO_InitTypeDef GPIO_InitStructure;
	/*
	 * PA0---E_STOP
	 * PA1---PWM1		//init in pwm.cpp
	 * PA2---PWM2		//init in pwm.cpp
	 * PA3---DIR1
	 * PA4---DIR2
	 * PA5---OUT1
	 * PA6---OUT2
	 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = Car_E_Stop_Pin | Car_DIR1_Pin | Car_DIR2_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = Car_OUT1_Pin | Car_OUT1_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
