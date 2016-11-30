/*
 * HR202L.h
 *
 *  Created on: 2016年11月28日
 *      Author: Romeli
 */

#ifndef HR202L_H_
#define HR202L_H_

#include "stm32f10x.h"

#define HR202L_Max_Adc 4095
#define HR202L_R_DIV 470 //单位0.1k

uint32_t HR202L_GetRes(uint16_t adc);

#endif /* HR202L_H_ */
