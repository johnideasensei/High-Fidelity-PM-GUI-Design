#include "main.h"

//#ifndef __AK4493_H__
//#define __AK4493_H__

#define AK4493_W 0x20
#define AK4493_R 0x21

//以下是寄存器地址---------------------
#define Control1_Set 0x01 //时钟设置 1都为打开 默认为0

#define SMUTE                   0x01  //软静音
#define Sampling_Speed 			    0x10	//采样率设置000x x000

#define Control3_Set 0x03//左声道音量

#define Control4_Set 0x04//右声道音量

#define Control6_Set 0x06//DSD

#define Control7_Set 0x07

#define SYNCE                 0x01  //开启SYNC模式
#define PCM_DSD_Gain 			    0x00	//采样率设置0000 xxx0

#define Control8_Set 0x08//音量控制

#define MUTE_Control 0x01  //0000 0xxx