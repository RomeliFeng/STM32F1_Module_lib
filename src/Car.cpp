#include "Car.h"
#include "PWM.h"

Car::Car()
{
	init();
}
void Car::init()
{
	//init pwm contrl gpio PA0-PA3
	PWM::init(4095, 4095);

	//init en_stop and direct gpio
	Car_GPIO_Config();

	//stop the car and set direction default
	GPIO_SetBits(EN_Stop);
	GPIO_SetBits(Direct);
}
/**
 * 函数：
 * run	同时控制4个通道的PWM
 * 参数：
 * speed	pwm占空比
 */
void Car::run(uint16_t speed)
{
	PWM::set(speed);
	GPIO_SetBits(EN_Stop);
}
/**
 * 函数：
 * run	同时控制单个通道的PWM
 * 参数：
 * ch	修改的通道
 * speed	pwm占空比
 */
void Car::run(uint8_t ch, uint16_t speed)
{
	PWM::set(ch,speed);
	GPIO_SetBits(EN_Stop);
}
/**
 * 函数：
 * stop	使能急停引脚
 */
void Car::stop()
{
	GPIO_ResetBits(EN_Stop);
}
/**
 * 函数：
 * direct	控制方向引脚
 * 参数：
 * dir	方向		0或非0
 */
void Car::direct(uint8_t dir)
{
	if (dir == 0)
	{
		GPIO_ResetBits(Direct);
	}
	else
	{
		GPIO_SetBits(Direct);
	}
}
/**
 * 函数：
 * Car_GPIO_Config	急停引脚和方向引脚的GPIO初始化
 */
void Car_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}
