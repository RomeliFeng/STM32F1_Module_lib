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
 * ������
 * run	ͬʱ����4��ͨ����PWM
 * ������
 * speed	pwmռ�ձ�
 */
void Car::run(uint16_t speed)
{
	PWM::set(speed);
	GPIO_SetBits(EN_Stop);
}
/**
 * ������
 * run	ͬʱ���Ƶ���ͨ����PWM
 * ������
 * ch	�޸ĵ�ͨ��
 * speed	pwmռ�ձ�
 */
void Car::run(uint8_t ch, uint16_t speed)
{
	PWM::set(ch,speed);
	GPIO_SetBits(EN_Stop);
}
/**
 * ������
 * stop	ʹ�ܼ�ͣ����
 */
void Car::stop()
{
	GPIO_ResetBits(EN_Stop);
}
/**
 * ������
 * direct	���Ʒ�������
 * ������
 * dir	����		0���0
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
 * ������
 * Car_GPIO_Config	��ͣ���źͷ������ŵ�GPIO��ʼ��
 */
void Car_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}
