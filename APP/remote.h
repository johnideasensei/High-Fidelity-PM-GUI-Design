#include "stdint.h"

#ifndef __REMOTE_H__
#define __REMOTE_H__


#define RDATA GPIO_GetBit(GPION, PIN1)//读取红外引脚电平状态




void remote_init(void);//初始化红外遥控IO的相关口
uint8_t remote_loop(void);
uint8_t Remote_Process(void);//红外解码
uint16_t Pulse_Width_Check(void);//检测脉冲宽度
void showdblb(int soundB);

//void delay_us(uint32_t us);//利用定时器4进行精确延时,单位1=100us

#endif


