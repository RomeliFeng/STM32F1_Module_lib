/*
 * HMC.cpp
 *
 *  Created on: 2016Äê10ÔÂ24ÈÕ
 *      Author: Romeli
 */

#include "HMC.h"
#include "I2C.h"
#include "math.h"

#define RAD_TO_DEG 57.295779513082320876798154814105f
#define CONFIG_A 	0x00
#define CONFIG_B	0x01
#define MODE		0x02
#define DATA_X		0x03
#define DATA_Z		0x05
#define DATA_Y		0x07
#define	HMC_ADD 	0x3C

_HMCData HMCData;
HMCClass HMC;

void HMCClass::Init() {
	I2C.Send(HMC_ADD, CONFIG_A, 0x70);	//75HZ
	I2C.Send(HMC_ADD, MODE, 0x00);	//Continuous Mode
}

void HMCClass::RefreshData() {
	static uint8_t Buffer[6];
	I2C.Receive(HMC_ADD, DATA_X, Buffer, 6);
	HMCData.X = Buffer[0] << 8 | Buffer[1];
	HMCData.Y = Buffer[2] << 8 | Buffer[3];
	HMCData.Z = Buffer[4] << 8 | Buffer[5];

	HMCData.Angle = atan2((double) -HMCData.X, (double) -HMCData.Z) * RAD_TO_DEG
			+ 180;
}
