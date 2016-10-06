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
#define TX_PLOAD_WIDTH 8

uint8_t nRF_TX_ADD[5] = { 0x01, 0x23, 0x45, 0x67, 0x89 };
uint8_t nRF_RX_ADD[5] = { 0x01, 0x23, 0x45, 0x67, 0x89 };

typedef enum {
	/*nRF24ָ��*/
	nRFCom_R_REGISTER = 0x00,	//�����üĴ���
	nRFCom_W_REGISTER = 0x20,	//д���üĴ���
	nRFCom_RX_PAYLOAD = 0x61,	//��RX��Ч���ݪ�1-32�ֽ�
	nRFCom_TX_PAYLOAD = 0xa0,	//дTX��Ч���ݪ�1-32�ֽ�
	nRFCom_FLUSH_TX = 0xe1,		//���TX FIFO�Ĵ�����Ӧ���ڷ���ģʽ��
	nRFCom_FLUSH_RX = 0xe2,		//���RX FIFO�Ĵ�����Ӧ���ڽ���ģʽ��
	nRFCom_REUSE_TX_PL = 0xe3,	//����ʹ����һ����Ч��
	nRFCom_NOP = 0xff,			//�ղ���������������״̬�Ĵ���
} nRFCom;

typedef enum {
	/*nRF24���üĴ���  ��Ҫ�Ͷ�дָ�����*/
	nRFAdd_CONFIG = 0x00,				//���üĴ���
	nRFAdd_EN_AA = 0x01,				//ʹ�ܡ��Զ�Ӧ�𡱹���
	nRFAdd_EN_RXADDR = 0x02,			//���յ�ַ����
	nRFAdd_SETUP_AW = 0x03,			//���õ�ַ��Ȫ���������ͨ����
	nRFAdd_SETUP_RETR = 0x04,			//�����Զ��ط�
	nRFAdd_RF_CH = 0x05,				//��Ƶͨ��
	nRFAdd_RF_SETUP = 0x06,			//��Ƶ�Ĵ���
	nRFAdd_STATUS = 0x07,				//״̬�Ĵ���
	nRFAdd_OBSERVE_TX = 0x08,			//���ͼ��Ĵ���
	nRFAdd_CD = 0x09,					//�ز����
	nRFAdd_RX_ADDR_P0 = 0x0a,			//����ͨ��0���յ�ַ
	nRFAdd_RX_ADDR_P1 = 0x0b,			//����ͨ��1���յ�ַ
	nRFAdd_RX_ADDR_P2 = 0x0c,			//����ͨ��2���յ�ַ
	nRFAdd_RX_ADDR_P3 = 0x0d,			//����ͨ��3���յ�ַ
	nRFAdd_RX_ADDR_P4 = 0x0e,			//����ͨ��4���յ�ַ
	nRFAdd_RX_ADDR_P5 = 0x0f,			//����ͨ��5���յ�ַ
	nRFAdd_TX_ADDR = 0x10,				//���͵�ַ����д���ֽ�
	nRFAdd_RX_PW_P0 = 0x11,			//��������ͨ��0��Ч���ݿ��(1��32�ֽ�)
	nRFAdd_RX_PW_P1 = 0x12,			//��������ͨ��1��Ч���ݿ��(1��32�ֽ�)
	nRFAdd_RX_PW_P2 = 0x13,			//��������ͨ��2��Ч���ݿ��(1��32�ֽ�)
	nRFAdd_RX_PW_P3 = 0x14,			//��������ͨ��3��Ч���ݿ��(1��32�ֽ�)
	nRFAdd_RX_PW_P4 = 0x15,			//��������ͨ��4��Ч���ݿ��(1��32�ֽ�)
	nRFAdd_RX_PW_P5 = 0x16,			//��������ͨ��5��Ч���ݿ��(1��32�ֽ�)
	nRFAdd_FIFO_statusTUS = 0x17			//FIFO ״̬�Ĵ���
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

void nRF24Class::Init(nRFMode mode, uint8_t ch) {
	SPI.Init();
	nRF24_GPIO_Init();
	SetMode(mode, ch);
}

void nRF24Class::SetMode(nRFMode mode, uint8_t ch) {
	nRF_CE_Reset;	//ʹ��nRF24
	switch (mode) {
	case nRFMode_Rx:
		Send(nRFCom_W_REGISTER + nRFAdd_RX_ADDR_P0, nRF_RX_ADD, 5);	//дRX�ڵ��ַ
		Send(nRFCom_W_REGISTER + nRFAdd_EN_AA, 0x01);    //ʹ��ͨ��0���Զ�Ӧ��
		Send(nRFCom_W_REGISTER + nRFAdd_EN_RXADDR, 0x01);    //ʹ��ͨ��0�Ľ��յ�ַ
		Send(nRFCom_W_REGISTER + nRFAdd_RF_CH, ch);	     //����RFͨ��Ƶ��
		Send(nRFCom_W_REGISTER + nRFAdd_RX_PW_P0, RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ��
		Send(nRFCom_W_REGISTER + nRFAdd_RF_SETUP, 0x0f);//����TX�������,0db����,2Mbps,���������濪��
		Send(nRFCom_W_REGISTER + nRFAdd_CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ
		break;
	case nRFMode_Tx:
		Send(nRFCom_W_REGISTER + nRFAdd_TX_ADDR, nRF_TX_ADD, 5);	   //дTX�ڵ��ַ
		Send(nRFCom_W_REGISTER + nRFAdd_RX_ADDR_P0, nRF_RX_ADD, 5); //����RX�ڵ��ַ,��ҪΪ��ʹ��ACK
		Send(nRFCom_W_REGISTER + nRFAdd_EN_AA, 0x01);     //ʹ��ͨ��0���Զ�Ӧ��
		Send(nRFCom_W_REGISTER + nRFAdd_EN_RXADDR, 0x01); //ʹ��ͨ��0�Ľ��յ�ַ
		Send(nRFCom_W_REGISTER + nRFAdd_SETUP_RETR, 0); //�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10�� 0x1a
		Send(nRFCom_W_REGISTER + nRFAdd_RF_CH, ch);       //����RFͨ��Ϊ40
		Send(nRFCom_W_REGISTER + nRFAdd_RF_SETUP, 0x0f); //����TX�������,0db����,2Mbps,���������濪��
		Send(nRFCom_W_REGISTER + nRFAdd_CONFIG, 0x0e); //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
		break;
	default:
		break;
	}
	nRF_CE_Set;	//CEΪ��,10us����������
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
	nRF_CSN_Reset; //ʹ��SPI
	status = SPI.transfer(Reg_Add);
	SPI.transfer(data);
	nRF_CSN_Set; //��ֹSPI
	return status;
}

uint8_t nRF24Class::Send(uint8_t Reg_Add, uint8_t* dataBuf, uint8_t size) {
	uint8_t status;
	nRF_CSN_Reset; //ʹ��SPI
	status = SPI.transfer(Reg_Add);
	for (uint8_t i = 0; i < size; ++i) {
		SPI.transfer(dataBuf[i]);
	}
	nRF_CSN_Set; //��ֹSPI
	return status;
}

uint8_t nRF24Class::Receive(uint8_t Reg_Add) {
	uint8_t data;
	nRF_CSN_Reset; //ʹ��SPI
	SPI.transfer(Reg_Add);
	data = SPI.transfer(0xff);
	nRF_CSN_Set; //��ֹSPI
	return data;
}

uint8_t nRF24Class::Receive(uint8_t Reg_Add, uint8_t* dataBuf, uint8_t size) {
	uint8_t status;
	nRF_CSN_Reset; //ʹ��SPI
	status = SPI.transfer(Reg_Add);
	for (uint8_t i = 0; i < size; ++i) {
		dataBuf[i] = SPI.transfer(0xff);
	}
	nRF_CSN_Set; //��ֹSPI
	return status;
}

uint8_t nRF24Class::RxPacket(uint8_t* dataBuf) {
	uint8_t status;
	status = Receive(nRFAdd_STATUS);  //��ȡ״̬�Ĵ�����ֵ
	Send(nRFCom_W_REGISTER + nRFAdd_STATUS, status); //���TX_DS��MAX_RT�жϱ�־
	if (status & nRF_RX_OK) //���յ�����
	{
		Send(nRFCom_RX_PAYLOAD, dataBuf, RX_PLOAD_WIDTH); //��ȡ����
		Send(nRFCom_FLUSH_RX, 0xff); //���RX FIFO�Ĵ���
		return 1;
	}
	return 0; //û�յ��κ�����
}

uint8_t nRF24Class::TxPacket(uint8_t* dataBuf) {
	uint8_t status;
	nRF_CE_Reset;
	Send(nRFCom_TX_PAYLOAD, dataBuf, TX_PLOAD_WIDTH);	//д���ݵ�TX BUF  32���ֽ�
	nRF_CE_Set;				//��������

#ifdef USE_IRQ
	while (nRF_IRQ_Read != 0)
		;			//�ȴ��������
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
	Send(nRFCom_W_REGISTER + nRFAdd_STATUS, status); 	//���TX_DS��MAX_RT�жϱ�־
	if (status & nRF_MAX_TX)					//�ﵽ����ط�����
	{
		Send(nRFCom_FLUSH_TX, 0xff);		//���TX FIFO�Ĵ���
		return 0;
	}
	if (status & nRF_TX_OK)		//�������
	{
		return 1;
	}
	return 0;		//����ԭ����ʧ��
}

