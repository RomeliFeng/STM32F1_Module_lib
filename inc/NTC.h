/*
 * NTC.h
 *
 *  Created on: 2016Äê11ÔÂ23ÈÕ
 *      Author: Romeli
 */

#ifndef NTC_H_
#define NTC_H_

#include "stm32f10x.h"

typedef enum _NTC_Div {
	NTCDiv_Top, NTCDiv_Btm
} NTCDiv;

uint16_t NTC_GetRes(uint16_t adc);
uint8_t NTC_GetTemp(NTCDiv div, uint16_t adc);
#endif /* NTC_H_ */
