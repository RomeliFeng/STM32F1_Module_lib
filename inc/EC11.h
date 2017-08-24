/*
 * EC11.h
 *
 *  Created on: 2016��6��28��
 *      Author: Romeli
 */

#ifndef EC11_H_
#define EC11_H_

#include "cmsis_device.h"

class EC11 {
public:
	static uint8_t WiseFlag;
	static void Init();
	static void Click();
	static void Press();
	static void Clockwise();
	static void Anticlockwise();
private:
	static void GPIOInit();
	static void EXTIInit();
	static void NVICInit();

};

#endif /* EC11_H_ */
