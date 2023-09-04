#include "AK4493.h"
#include "main.h"
#include "soft_i2c.h"



#include "lvgl/lvgl.h"

/**
 * @brief gt9x写寄存器
 * @param reg 要写入的寄存器
 * @param buf 要写入的数组
 * @param len 要写入的数组的长度
 * @return true
 * @return false
 */
bool write_AK4493_regs(uint8_t reg, uint8_t buf)
{
    uint32_t i;

		i2c_start();

    //发送器件地址
    i2c_write_byte(AK4493_W);
    if (i2c_waitack())
        goto wr_fail;

    i2c_write_byte(reg);//选择写入的寄存器地址
    if (i2c_waitack())
        goto wr_fail;
		
		i2c_write_byte(buf);//写入寄存器的数值
		if (i2c_waitack())
				goto wr_fail;



    i2c_stop();
    for (i = 0; i < CyclesPerUs; i++)
        __NOP();
    return true;

wr_fail:
    i2c_stop();
    for (i = 0; i < CyclesPerUs; i++)
        __NOP();
    return false;
}


bool read_AK4493_regs(uint8_t reg, uint8_t *buf)
{
    uint32_t i;

    i2c_start();
    i2c_write_byte(AK4493_W);
    if (i2c_waitack())
        goto rd_fail;	
	
    i2c_write_byte(reg);
    if (i2c_waitack())
        goto rd_fail;
		
    i2c_start();
    i2c_write_byte(AK4493_R);
    if (i2c_waitack())
        goto rd_fail;		
	
		*buf = i2c_read_byte();
		i2c_ack(1); // NAck = 1

    i2c_stop();
    for (i = 0; i < CyclesPerUs; i++)
        __NOP();
    return true;		
rd_fail:
    i2c_stop();
    for (i = 0; i < CyclesPerUs; i++)
        __NOP();
    return false;
}



