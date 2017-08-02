/*
 * nRF24.cpp
 *
 *  Created on: 2016��5��28��
 *      Author: Romeli
 */

#include "nRF24.h"
#include "SPI.h"

nRF24Class nRF24;

#define nRF_CE_Set GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_SET)
#define nRF_CE_Reset GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_RESET)
#define nRF_CSN_Set GPIO_WriteBit(GPIOA,GPIO_Pin_12,Bit_SET)
#define nRF_CSN_Reset GPIO_WriteBit(GPIOA,GPIO_Pin_12,Bit_RESET)
#define nRF_IRQ_Set GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_SET)
#define nRF_IRQ_Reset GPIO_WriteBit(GPIOB,GPIO_Pin_4,Bit_RESET)
#define nRF_IRQ_Read GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)

#define nRF_MAX_TX 0x10
#define nRF_TX_OK 0x20
#define nRF_RX_OK 0x40

#define RX_PLOAD_WIDTH 32
#define TX_PLOAD_WIDTH 32

uint8_t nRF_TX_ADD[5] = { 0x01, 0x23, 0x45, 0x67, 0x89 };
uint8_t nRF_RX_ADD[5] = { 0x01, 0x23, 0x45, 0x67, 0x89 };

typedef enum {
	/*nRF24ָ��*/
	nRFCom_R_REGISTER = 0x00,	//读配置寄存器
	nRFCom_W_REGISTER = 0x20,	//写配置寄存器
	nRFCom_RX_PAYLOAD = 0x61,	//读RX有效数据1-32字节
	nRFCom_TX_PAYLOAD = 0xa0,	//写TX有效数据 1-32字节
	nRFCom_FLUSH_TX = 0xe1,		//清除TX FIFO寄存器 应用于发射模式下
	nRFCom_FLUSH_RX = 0xe2,		//清除RX FIFO寄存器 应用于接收模式下
	nRFCom_REUSE_TX_PL = 0xe3,	//重新使用上一包有效数
	nRFCom_NOP = 0xff,			//空操作。可以用来读状态寄存器
} nRFCom;

typedef enum {
	/*nRF24配置寄存器  需要和读写指令相加*/
	nRFAdd_CONFIG = 0x00,				//配置寄存器
	nRFAdd_EN_AA = 0x01,				//使能“自动应答”功能
	nRFAdd_EN_RXADDR = 0x02,			//接收地址允许
	nRFAdd_SETUP_AW = 0x03,			//设置地址宽度所有数据通道
	nRFAdd_SETUP_RETR = 0x04,			//建立自动重发
	nRFAdd_RF_CH = 0x05,				//射频通道
	nRFAdd_RF_SETUP = 0x06,			//射频寄存器
	nRFAdd_STATUS = 0x07,				//状态寄存器
	nRFAdd_OBSERVE_TX = 0x08,			//发送检测寄存器
	nRFAdd_CD = 0x09,					//载波检测
	nRFAdd_RX_ADDR_P0 = 0x0a,			//数据通道0接收地址
	nRFAdd_RX_ADDR_P1 = 0x0b,			//数据通道1接收地址
	nRFAdd_RX_ADDR_P2 = 0x0c,			//数据通道2接收地址
	nRFAdd_RX_ADDR_P3 = 0x0d,			//数据通道3接收地址
	nRFAdd_RX_ADDR_P4 = 0x0e,			//数据通道4接收地址
	nRFAdd_RX_ADDR_P5 = 0x0f,			//数据通道5接收地址
	nRFAdd_TX_ADDR = 0x10,				//发送地址，先写低字节
	nRFAdd_RX_PW_P0 = 0x11,			//接收数据通道0有效数据宽度(1到32字节)
	nRFAdd_RX_PW_P1 = 0x12,			//接收数据通道1有效数据宽度(1到32字节)
	nRFAdd_RX_PW_P2 = 0x13,			//接收数据通道2有效数据宽度(1到32字节)
	nRFAdd_RX_PW_P3 = 0x14,			//接收数据通道3有效数据宽度(1到32字节)
	nRFAdd_RX_PW_P4 = 0x15,			//接收数据通道4有效数据宽度(1到32字节)
	nRFAdd_RX_PW_P5 = 0x16,			//接收数据通道5有效数据宽度(1到32字节)
	nRFAdd_FIFO_statusTUS = 0x17			//FIFO 状态寄存器
} nRFAdd;

void nRF24_GPIO_Init() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	nRF_CE_Reset; //ʹ��nRF24
	nRF_CSN_Set; //��ֹSPI
}

uint8_t nRF24Class::Init(nRFMode mode, uint8_t ch) {
	SPI.Init();
	nRF24_GPIO_Init();
	return SetMode(mode, ch);
}

uint8_t nRF24Class::SetMode(nRFMode mode, uint8_t ch) {
	uint8_t status = Check();
	if (status == 0)
		return 0;
	nRF_CE_Reset;	//ʹ��nRF24
	switch (mode) {
	case nRFMode_Rx:
		Send(nRFCom_W_REGISTER + nRFAdd_RX_ADDR_P0, nRF_RX_ADD, 5);	//写RX节点地址
		Send(nRFCom_W_REGISTER + nRFAdd_EN_AA, 0x01);    //使能通道0的自动应答
		Send(nRFCom_W_REGISTER + nRFAdd_EN_RXADDR, 0x01);    //使能通道0的接收地址ַ
		Send(nRFCom_W_REGISTER + nRFAdd_RF_CH, ch);	     //设置RF通信频率
		Send(nRFCom_W_REGISTER + nRFAdd_RX_PW_P0, RX_PLOAD_WIDTH);//选择通道0的有效数据宽度
		Send(nRFCom_W_REGISTER + nRFAdd_RF_SETUP, 0x0f);//设置TX发射参数,0db增益,2Mbps,低噪声增益开启
		Send(nRFCom_W_REGISTER + nRFAdd_CONFIG, 0x0f);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式
		break;
	case nRFMode_Tx:
		Send(nRFCom_W_REGISTER + nRFAdd_TX_ADDR, nRF_TX_ADD, 5);	  //写TX节点地址
		Send(nRFCom_W_REGISTER + nRFAdd_RX_ADDR_P0, nRF_RX_ADD, 5); //设置RX节点地址,主要为了使能ACK
		Send(nRFCom_W_REGISTER + nRFAdd_EN_AA, 0x01);     //使能通道0的自动应答
		Send(nRFCom_W_REGISTER + nRFAdd_EN_RXADDR, 0x01); //使能通道0的接收地址ַ
		Send(nRFCom_W_REGISTER + nRFAdd_SETUP_RETR, 0x1a); //设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次 0x1a
		Send(nRFCom_W_REGISTER + nRFAdd_RF_CH, ch);       //设置RF通道为40
		Send(nRFCom_W_REGISTER + nRFAdd_RF_SETUP, 0x0f); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
		Send(nRFCom_W_REGISTER + nRFAdd_CONFIG, 0x0e); //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
		break;
	default:
		break;
	}
	nRF_CE_Set;	//CE为高,10us后启动发送
	return 1;
}

uint8_t nRF24Class::Check() {
	uint8_t buf[5] = { 0xa5, 0xa5, 0xa5, 0xa5, 0xa5 };
	Send(nRFCom_W_REGISTER + nRFAdd_TX_ADDR, buf, 5);
	Receive(nRFAdd_TX_ADDR, buf, 5);
	for (uint8_t i = 0; i < 5; i++)
		if (buf[i] != 0xa5) {
			return 0;
		}
	return 1;
}

uint8_t nRF24Class::Send(uint8_t Reg_Add, uint8_t data) {
	uint8_t status;
	nRF_CSN_Reset; //使能SPI
	status = SPI.transfer(Reg_Add);
	SPI.transfer(data);
	nRF_CSN_Set;  //禁止SPI
	return status;
}

uint8_t nRF24Class::Send(uint8_t Reg_Add, uint8_t* dataBuf, uint8_t size) {
	uint8_t status;
	nRF_CSN_Reset;//使能SPI
	status = SPI.transfer(Reg_Add);
	for (uint8_t i = 0; i < size; ++i) {
		SPI.transfer(dataBuf[i]);
	}
	nRF_CSN_Set;  //禁止SPI
	return status;
}

uint8_t nRF24Class::Receive(uint8_t Reg_Add) {
	uint8_t data;
	nRF_CSN_Reset; //使能SPI
	SPI.transfer(Reg_Add);
	data = SPI.transfer(0xff);
	nRF_CSN_Set;  //禁止SPI
	return data;
}

uint8_t nRF24Class::Receive(uint8_t Reg_Add, uint8_t* dataBuf, uint8_t size) {
	uint8_t status;
	nRF_CSN_Reset; //使能SPI
	status = SPI.transfer(Reg_Add);
	for (uint8_t i = 0; i < size; ++i) {
		dataBuf[i] = SPI.transfer(0xff);
	}
	nRF_CSN_Set; //禁止SPI
	return status;
}

uint8_t nRF24Class::RxPacket(uint8_t* dataBuf, uint8_t size) {
	uint8_t status;
	status = Receive(nRFAdd_STATUS);  //读取状态寄存器的值ֵ
	Send(nRFCom_W_REGISTER + nRFAdd_STATUS, status); //清除TX_DS或MAX_RT中断标志
	if (status & nRF_RX_OK)  //接收到数据
	{
		nRF_CSN_Reset; //使能SPI
		status = SPI.transfer(nRFCom_RX_PAYLOAD);
		for (uint8_t i = 0; i < size; ++i) {
			dataBuf[i] = SPI.transfer(0xff);//读取数据
		}
		for (uint8_t i = 0; i < RX_PLOAD_WIDTH - size; ++i) {
			SPI.transfer(0xff);
		}
		nRF_CSN_Set; //禁止SPI
		Send(nRFCom_FLUSH_RX, 0xff); //清除RX FIFO寄存器
		return 1;
	}
	return 0;  //没收到任何数据
}

uint8_t nRF24Class::TxPacket(uint8_t* dataBuf, uint8_t size) {
	uint8_t status;
	nRF_CE_Reset;
	nRF_CSN_Reset; //使能SPI
	status = SPI.transfer(nRFCom_TX_PAYLOAD);
	for (uint8_t i = 0; i < size; ++i) {
		SPI.transfer(dataBuf[i]);
	}
	for (uint8_t i = 0; i < TX_PLOAD_WIDTH - size; ++i) {
		SPI.transfer(0);
	}
	nRF_CSN_Set; //禁止SPI
	nRF_CE_Set;	//启动发送

#ifdef USE_IRQ
	while (nRF_IRQ_Read != 0)
	;			//等待发送完成
	status = Receive(nRFAdd_STATUS);
#else
	uint8_t flag = 1;
	while (flag) {
		status = Receive(nRFAdd_STATUS);
		if ((status & nRF_MAX_TX) | (status & nRF_TX_OK))
			flag = 0;
	}
#endif
	status = Receive(nRFAdd_STATUS);
	Send(nRFCom_W_REGISTER + nRFAdd_STATUS, status); 	//清除TX_DS或MAX_RT中断标志
	if (status & nRF_MAX_TX)					//达到最大重发次数
	{
		Send(nRFCom_FLUSH_TX, 0xff);		//清除TX FIFO寄存器
		return 0;
	}
	if (status & nRF_TX_OK)		//发送完成
	{
		return 1;
	}
	return 0;		//其他原因发送失败
}

