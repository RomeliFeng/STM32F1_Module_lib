/*
 * NTC.cpp
 *
 *  Created on: 2016年11月23日
 *      Author: Romeli
 */

#include "NTC.h"

#define Max_Adc 4095

const uint16_t NTC_Tab[] = { 28017, 26826, 25697, 24629, 23618, 22660, 21752,
		20892, 20075, 19299, 18560, 18482, 18149, 17632, 16992, 16280, 15535,
		14787, 14055, 13354, 12690, 12068, 11490, 10954, 10458, 10000, 9576,
		9184, 8819, 8478, 8160, 7861, 7579, 7311, 7056, 6813, 6581, 6357, 6142,
		5934, 5734, 5541, 5353, 5173, 4998, 4829, 4665, 4507, 4355, 4208, 4065,
		3927, 3794, 3664, 3538, 3415, 3294, 3175, 3058, 2941, 2825, 2776, 2718,
		2652, 2582, 2508, 2432, 2356, 2280, 2207, 2135, 2066, 2000, 1938, 1879,
		1823, 1770, 1720, 1673, 1628, 1586, 1546, 1508, 1471, 1435, 1401, 1367,
		1334, 1301, 1268, 1236, 1204, 1171, 1139, 1107, 1074, 1042, 1010, 979,
		948, 918, 889, 861, 835, 810, 787, 767, 749, 733, 721, 713 };

uint16_t NTC_GetRes(uint16_t adc) {
	return ((double) Max_Adc / adc * 10000) - 10000;
}

uint8_t NTC_FineTab(uint16_t data) //表中数据从大到小
		{
	uint8_t m = 0;
	uint8_t st = 0;
	uint8_t ed = (sizeof(NTC_Tab) / 2) - 1;

	if (data >= NTC_Tab[st])
		return st;
	else if (data <= NTC_Tab[ed])
		return ed;

	while (st < ed) {
		m = (st + ed) / 2;

		if (data == NTC_Tab[m])
			break;
		if (data < NTC_Tab[m] && data > NTC_Tab[m + 1])
			break;

		if (data > NTC_Tab[m])
			ed = m;
		else
			st = m;
	}
	if (st > ed)
		return 0;
	return m;
}

uint8_t NTC_GetTemp(NTCDiv div, uint16_t adc) {
	switch (div) {
	case NTCDiv_Top:
		break;
	case NTCDiv_Btm:
		adc = Max_Adc - adc;
		break;
	default:
		break;
	}
	return NTC_FineTab(NTC_GetRes(adc));
}

