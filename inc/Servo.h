/*
 * Servo.h
 *
 *  Created on: 2016年5月1日
 *      Author: Romeli
 */

#ifndef SERVO_H_
#define SERVO_H_

#include "stm32f10x.h"

#define PWM1_Set_High GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define PWM1_Set_Low GPIO_ResetBits(GPIOC,GPIO_Pin_13)

#define PWM_Over_Time 20000/1		//一路输出

class Servo {
public:
	Servo();
	static void init();
	static void write(uint8_t ch,uint8_t angle);
private:

};

void Servo_GPIO_Config();
void Servo_NVIC_Config();
void Servo_PWM_Config();
#endif /* SERVO_H_ */
