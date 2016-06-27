/*
 * BH1750.h
 *
 *  Created on: 2016年5月25日
 *      Author: Romeli
 */

#ifndef BH1750_H_
#define BH1750_H_

#include "stm32f10x.h"

#define BH_Address 0x46

typedef enum
{
	BH_Power_On = 0x01, BH_Reset = 0x07, BH_Con_H = 0x10, //1-lx 解析度 ，120ms转换时间，连续
	BH_Con_H2 = 0x11,	//0.5-lx 解析度 ，120ms转换时间，连续
	BH_Con_L = 0x13,	//1-lx 解析度 ，120ms转换时间，连续
	BH_One_H = 0x20,	//1-lx 解析度 ，120ms转换时间，单次
	BH_One_H2 = 0x21,	//0.5-lx 解析度 ，120ms转换时间，单次
	BH_One_L = 0x23		//4-lx 解析度 ，120ms转换时间，单次
} BHMode;


class BH1750
{
public:
	static void init();
	static void set(BHMode mode);
	static void reset();
	static uint16_t getData();
private:
};

#endif /* BH1750_H_ */
