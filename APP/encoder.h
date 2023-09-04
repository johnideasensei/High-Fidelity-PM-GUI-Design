#include "stdint.h"

#ifndef __ENCODER_H__
#define __ENCODER_H__


#define ENCODER_L        GPIO_GetBit(GPIOM, PIN1)   //KB
#define ENCODER_R        GPIO_GetBit(GPIOM, PIN0)   //KA	
#define ENCODER_SW       GPIO_GetBit(GPIOD, PIN11)   //SW	


#define GETDSD      GPIO_GetBit(GPIOD, PIN14)   //DSD
#define GETDOP      GPIO_GetBit(GPIOM, PIN15)   //DOP	
#define GETF1       GPIO_GetBit(GPIOD, PIN13)   //F1	
#define GETF2       GPIO_GetBit(GPIOD, PIN12)   //F2
#define GETF3       GPIO_GetBit(GPIOM, PIN7)   //F3	
#define GETF4       GPIO_GetBit(GPIOM, PIN4)   //F4	

#define HIN0       GPIO_AtomicSetBit(GPIOM, PIN9)   //IN0	
#define LIN0       GPIO_AtomicClrBit(GPIOM, PIN9)   //IN0		
#define HIN1       GPIO_AtomicSetBit(GPIOM, PIN10)   //IN1		
#define LIN1       GPIO_AtomicClrBit(GPIOM, PIN10)   //IN1		

#define GET_IN0       GPIO_GetBit(GPIOM, PIN9)   //IN0	
#define GET_IN1       GPIO_GetBit(GPIOM, PIN10)   //IN1		


void ENCODER_INIT(void);//初始化旋转编码器IO的相关口
uint8_t ENCODER_READ(uint8_t idflag);//编码器状态读取uint8_t mode
//uint8_t KEYPAD_READ(void);
uint8_t check_wkup(void);//旋钮编码器SW按下两秒计数
void ENCODER_LOOP(void);//扫描旋转编码器函数
void sam_fre_init(void);
void sam_fre_chose(void);
uint8_t ENCODER_DB(void);
void ENCODER_DBLOOP(void);

#endif


