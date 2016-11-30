/*
 * HR202L.cpp
 *
 *  Created on: 2016Äê11ÔÂ28ÈÕ
 *      Author: Romeli
 */

#include "HR202L.h"

uint32_t HR202L_GetRes(uint16_t adc) {
	adc = HR202L_Max_Adc - adc;
	return ((double) HR202L_Max_Adc / adc * HR202L_R_DIV) - HR202L_R_DIV;
}
