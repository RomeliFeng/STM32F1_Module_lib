/*
 * nRF24.h
 *
 *  Created on: 2016Äê5ÔÂ28ÈÕ
 *      Author: Romeli
 */

#ifndef NRF24_H_
#define NRF24_H_

#include "stm32f10x.h"

//#define USE_IRQ

typedef enum {
	nRFMode_Tx, nRFMode_Rx
} nRFMode;

void nRF24_GPIO_Init();

class nRF24Class {
public:
	uint8_t Init(nRFMode mode, uint8_t ch);
	uint8_t SetMode(nRFMode mode, uint8_t ch);
	uint8_t Check();
	uint8_t Send(uint8_t Reg_Add, uint8_t data);
	uint8_t Send(uint8_t Reg_Add, uint8_t* dataBuf, uint8_t size);
	uint8_t Receive(uint8_t Reg_Add);
	uint8_t Receive(uint8_t Reg_Add, uint8_t* dataBuf, uint8_t size);
	uint8_t RxPacket(uint8_t* dataBuf, uint8_t size);
	uint8_t TxPacket(uint8_t* dataBuf, uint8_t size);
};

extern nRF24Class nRF24;

#endif /* NRF24_H_ */
