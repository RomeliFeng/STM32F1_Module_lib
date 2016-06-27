/*
 * BH1750.cpp
 *
 *  Created on: 2016Äê5ÔÂ25ÈÕ
 *      Author: Romeli
 *      Depand on:"I2C.h"
 */

#include "BH1750.h"
#include "I2C.h"

void BH1750::init()
{
	I2C.init();
	set(BH_Power_On);
}

void BH1750::set(BHMode mode)
{
	I2C.send(BH_Address, mode);
}

void BH1750::reset()
{
	set(BH_Reset);
}

uint16_t BH1750::getData()
{
	uint8_t dataBuf[2];
	I2C.receive(BH_Address, dataBuf, 2);
	return (uint16_t) dataBuf[0] << 8 | dataBuf[1];
}
