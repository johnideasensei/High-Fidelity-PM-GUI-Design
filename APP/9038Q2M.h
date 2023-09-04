
#include "main.h"

#ifndef __9038Q2M_H__
#define __9038Q2M_H__

//左声道9038-读写地址
#define ARR9038_channel_L_W 0x90
#define ARR9038_channel_L_R 0x91

#define ARR9038_channel_R_W 0x92
#define ARR9038_channel_R_R 0x93


//以下是寄存器地址---------------------


//04自动静音时间设置----推荐0xF0
#define Mute_Time 0x04

//07滤波器和静音-----做个宏定义吧  位0-1是静音控制
#define FIR_MUTE_Set 			0x07	//滤波器寄存器地址

#define BRICK_WALL_FILTER 0xE0  //砌砖
#define CORRECTED  			  0xC0	//修正最小相位快降
#define APODIZING 				0x80	//默认apodizing fast roll-off filter
#define M_P_SLOW				  0x60	//最小相位慢降
#define M_P_FAST 					0x40  //最小相位快降
#define L_P_SLOW 					0x20  //线性相位慢降
#define L_P_FAST 					0x00  //线性相位快降



//12-ASRC/DPLL带宽----推荐0xCC
#define ASRC_DPLL 0x0C

//14软启动配置----推荐0xCA
#define Sofe_Start 0x0E

//15-16音量控制寄存器
#define ARR9038_volume_L 0x0F
#define ARR9038_volume_R 0x10

//27增益----推荐0x54
#define DB_Set 0x1B
#define GAIN_0 0x00    		//一个9038中，两个通道都没有增益
#define GAIN_18 0x03			//一个9038中，两个通道都增益+18



//39耳放开关----推荐0xc0
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


