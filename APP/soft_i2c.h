#ifndef _SOFT_I2C_H_
#define _SOFT_I2C_H_


#include "SWM341.h"
//SDA引脚接PA0
//SCL引脚接PA1


#define SOFT 1 
#define I2C_DELAY (2)
#define CT_SCL_GPIO GPIOA
#define CT_SCL_PIN PIN1
#define CT_SDA_GPIO GPIOA
#define CT_SDA_PIN PIN0

#define CT_SCL_L GPIO_ClrBit(CT_SCL_GPIO, CT_SCL_PIN)
#define CT_SCL_H GPIO_SetBit(CT_SCL_GPIO, CT_SCL_PIN)
#define CT_SDA_L GPIO_ClrBit(CT_SDA_GPIO, CT_SDA_PIN)
#define CT_SDA_H GPIO_SetBit(CT_SDA_GPIO, CT_SDA_PIN)
//设置DA为读操作或是写操作
#define SET_SDA_READ GPIO_Init(CT_SDA_GPIO, CT_SDA_PIN, 0, 1, 0, 0)
#define SET_SDA_WRITE GPIO_Init(CT_SDA_GPIO, CT_SDA_PIN, 1, 1, 0, 0)
#define READ_SDA GPIO_GetBit(CT_SDA_GPIO, CT_SDA_PIN)

void I2CMstInit(void);
void IIC_Init(void);
void soft_i2c_init(void);
uint8_t i2c_read_byte(void);
void i2c_write_byte(uint8_t data);
uint8_t i2c_waitack(void);
void i2c_ack(uint8_t ack);
void i2c_stop(void);
void i2c_start(void);




























#endif