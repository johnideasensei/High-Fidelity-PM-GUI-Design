#include "main.h"

//#ifndef __AK4493_H__
//#define __AK4493_H__

#define AK4493_W 0x20
#define AK4493_R 0x21

//�����ǼĴ�����ַ---------------------
#define Control1_Set 0x01 //ʱ������ 1��Ϊ�� Ĭ��Ϊ0

#define SMUTE                   0x01  //����
#define Sampling_Speed 			    0x10	//����������000x x000

#define Control3_Set 0x03//����������

#define Control4_Set 0x04//����������

#define Control6_Set 0x06//DSD

#define Control7_Set 0x07

#define SYNCE                 0x01  //����SYNCģʽ
#define PCM_DSD_Gain 			    0x00	//����������0000 xxx0

#define Control8_Set 0x08//��������

#define MUTE_Control 0x01  //0000 0xxx