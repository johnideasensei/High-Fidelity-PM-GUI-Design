
#include "main.h"

#ifndef __9039M_H__
#define __9039M_H__

//9039-��д��ַ
#define ARR9039_W 0x90
#define ARR9039_R 0x91



//�����ǼĴ�����ַ---------------------





//29-SSRC/DPLL����
#define SSRC_DPLL 0x1d

//57����ѡ�� 
#define INSEL_SET 0x39 //--0x46

#define SET_PCM		0x06	//PCM
#define SET_DSD		0x02	//DSD

//74-81�������ƼĴ���ch1-ch8 0~255 0~-127.5dB
#define ARR9039_volume_CH1 0x4A
#define ARR9039_volume_CH2 0x4B
#define ARR9039_volume_CH3 0x4C
#define ARR9039_volume_CH4 0x4D

#define ARR9039_volume_CH5 0x4E
#define ARR9039_volume_CH6 0x4F
#define ARR9039_volume_CH7 0x50
#define ARR9039_volume_CH8 0x51

//88�˲���
#define FIR_Shape_Set 			0x58	//�˲����Ĵ�����ַ

#define M_P				              0x00	//��С��λ
#define APODIZING 				      0x01	//Linear phase apodizing
#define L_P_FAST 					      0x02  //������λ�콵
#define L_P_SLOW 					      0x04  //������λ����
#define M_P_FAST 					      0x05  //��С��λ�콵
#define M_P_SLOW				        0x06	//��С��λ����
#define M_P_SLOW_LOWDP				  0x07	//��С��λ������ɫɢ

//89�˲���
#define SPDIF_SEL 			0x59	//spdif��PCM��ѡ�� --0x04

#define SPDIF_DATA2 		0x50	//4d'5

//124-125�Զ�����ʱ������00 00-07 FF
#define Mute_Timea 0x7c 
#define Mute_Timeb 0x7d 

//130����������
#define Sofe_RAMP 0x82

#define Sofe_RAMP_TIME 0x02






bool write_9039m_regs(uint8_t reg, uint8_t buf);
bool read_9039m_regs(uint8_t reg, uint8_t *buf);
bool write_volume(uint8_t lVolume,uint8_t rVolume);
void write_lvolume(uint8_t lVolume);
void write_rvolume(uint8_t rVolume);
void es9039_init(void);



#endif


