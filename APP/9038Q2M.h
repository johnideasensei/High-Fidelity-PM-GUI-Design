
#include "main.h"

#ifndef __9038Q2M_H__
#define __9038Q2M_H__

//������9038-��д��ַ
#define ARR9038_channel_L_W 0x90
#define ARR9038_channel_L_R 0x91

#define ARR9038_channel_R_W 0x92
#define ARR9038_channel_R_R 0x93


//�����ǼĴ�����ַ---------------------


//04�Զ�����ʱ������----�Ƽ�0xF0
#define Mute_Time 0x04

//07�˲����;���-----�����궨���  λ0-1�Ǿ�������
#define FIR_MUTE_Set 			0x07	//�˲����Ĵ�����ַ

#define BRICK_WALL_FILTER 0xE0  //��ש
#define CORRECTED  			  0xC0	//������С��λ�콵
#define APODIZING 				0x80	//Ĭ��apodizing fast roll-off filter
#define M_P_SLOW				  0x60	//��С��λ����
#define M_P_FAST 					0x40  //��С��λ�콵
#define L_P_SLOW 					0x20  //������λ����
#define L_P_FAST 					0x00  //������λ�콵



//12-ASRC/DPLL����----�Ƽ�0xCC
#define ASRC_DPLL 0x0C

//14����������----�Ƽ�0xCA
#define Sofe_Start 0x0E

//15-16�������ƼĴ���
#define ARR9038_volume_L 0x0F
#define ARR9038_volume_R 0x10

//27����----�Ƽ�0x54
#define DB_Set 0x1B
#define GAIN_0 0x00    		//һ��9038�У�����ͨ����û������
#define GAIN_18 0x03			//һ��9038�У�����ͨ��������+18



//39���ſ���----�Ƽ�0xc0
#define HEAD_Phone 0x27
#define HEADPHONE_DISABLE 0x00
#define HEADPHONE_ENABLE 0x40






static bool read_9038q2m_regs_text(uint16_t reg, uint8_t *buf, uint32_t len);
static bool write_9038q2m_regs_text(uint16_t reg, uint8_t *buf, uint32_t len);
void fuck_you(void);



bool read_9038q2m_regs_channe_L(uint8_t reg, uint8_t *buf);
bool write_9038q2m_regs_channe_L(uint8_t reg, uint8_t buf);

bool read_9038q2m_regs_channe_R(uint8_t reg, uint8_t *buf);
bool write_9038q2m_regs_channe_R(uint8_t reg, uint8_t buf);
bool witer_volume(uint8_t set_vol);
bool Witer_LR_9038(uint8_t reg, uint8_t buf);
bool Read_LR_9038(uint8_t reg, uint8_t* buf);




#endif


