/*
 * Servo.cpp
 *
 *  Created on: 2016年5月1日
 *      Author: Romeli
 */
#include "Servo.h"

static uint16_t PWM1_Period = 1500;
static uint8_t Order = 0;

Servo::Servo()
{
}

void Servo::init()
{
	Servo_GPIO_Config();
	Servo_NVIC_Config();
	Servo_PWM_Config();
}

void Servo::write(uint8_t ch, uint8_t angle)
{
	if (angle <= 180)
	{
		switch (ch)
		{
		case 1:
			PWM1_Period = 500+ ((double)angle/90)*1000;
			break;
		default:
			break;
		}
	}

}

void Servo_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//开启GPIOC的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//设置PC13为推挽输出 50M
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOC, &GPIO_InitStructure);
	PWM1_Set_Low;
}

void Servo_PWM_Config()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructrue;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	TIM_TimeBaseInitStructrue.TIM_Prescaler = 71;		//预分频系数  72M/72 = 1uS
	TIM_TimeBaseInitStructrue.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructrue.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructrue.TIM_Period = PWM1_Period;	//加载第一路输出的值
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructrue);
	TIM_ARRPreloadConfig(TIM4, DISABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_Cmd(TIM4, ENABLE);
}
void Servo_NVIC_Config()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStructure);
}
extern "C" void TIM4_IRQHandler()
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		//trace_printf("i am in,now is %d\n",Order);
		switch (Order)
		{
		case 0:
			PWM1_Set_High;
			TIM_SetAutoreload(TIM4, PWM1_Period);
			Order++;
			break;
		case 1:
			PWM1_Set_Low;
			TIM_SetAutoreload(TIM4, PWM_Over_Time - PWM1_Period);
			Order = 0;
			break;
		default:
			Order = 0;
		}
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}

