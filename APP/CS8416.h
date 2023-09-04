#include "main.h"

//#ifndef __CS8416_H__
//#define __CS8416_H__

#define CS8416_W 0x20
#define CS8416_R 0x21

//�����ǼĴ�����ַ---------------------
#define Control1_Set 0x01 //ʱ������ 1��Ϊ�� Ĭ��Ϊ0

#define OMCK_EN           0x80  //ʱ���Զ��л� bit7
#define FIR_MUTE 			    0x40	//�������� bit6

//�������Ĵ�����4
#define Control4_Set 0x04 

#define CLOCKS 			   0x80	//�����ڲ�ʱ��RUN,bit7
#define RXP1 			     0x08	//RXP0~7��������� 00xx1001
#define RXP2 			     0x10	//RXP0~7��������� 00x1x001
#define RXD 			     0x40	//RMCKʱ��������� bit6 ���迹
#define RXD 			     0x40	

#define Control5_Set         0x05 //����OLRCK��OSCLK�������������

#define SCLK_LRCKIO 			   0x80	//���


bool write_CS8416_regs(uint8_t reg, uint8_t buf);
bool read_CS8416_regs(uint8_t reg, uint8_t *buf);
void cs8416_init(void);