#include "main.h"

//#ifndef __CS8416_H__
//#define __CS8416_H__

#define CS8416_W 0x20
#define CS8416_R 0x21

//以下是寄存器地址---------------------
#define Control1_Set 0x01 //时钟设置 1都为打开 默认为0

#define OMCK_EN           0x80  //时钟自动切换 bit7
#define FIR_MUTE 			    0x40	//静音控制 bit6

//控制器寄存器件4
#define Control4_Set 0x04 

#define CLOCKS 			   0x80	//控制内部时钟RUN,bit7
#define RXP1 			     0x08	//RXP0~7输入接收器 00xx1001
#define RXP2 			     0x10	//RXP0~7输入接收器 00x1x001
#define RXD 			     0x40	//RMCK时钟输出控制 bit6 高阻抗
#define RXD 			     0x40	

#define Control5_Set         0x05 //设置OLRCK和OSCLK是输出还是输入

#define SCLK_LRCKIO 			   0x80	//输出


bool write_CS8416_regs(uint8_t reg, uint8_t buf);
bool read_CS8416_regs(uint8_t reg, uint8_t *buf);
void cs8416_init(void);