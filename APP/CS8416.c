#include "CS8416.h"
#include "main.h"
#include "soft_i2c.h"


#include "lvgl/lvgl.h"

extern uint8_t valuetemp,debug_read,value;

/**
 * @brief gt9x写寄存器
 * @param reg 要写入的寄存器
 * @param buf 要写入的数组
 * @param len 要写入的数组的长度
 * @return true
 * @return false
 */
 
 
bool write_CS8416_regs(uint8_t reg, uint8_t buf)
{

	uint32_t i;
	uint8_t ack;
#if SOFT
	
		i2c_start();

    //发送器件地址
    i2c_write_byte(CS8416_W);
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
		
#else
		ack = I2C_Start(I2C0,(CS8416_W)|0, 1);
		if(ack == 0)
			goto nextloop;

		ack = I2C_Write(I2C0,reg, 1);
		if(ack == 0)
			goto nextloop;
		
		ack = I2C_Write(I2C0,buf, 1);
		if(ack == 0)
			goto nextloop;
		
		I2C_Stop(I2C0, 1);
    for (i = 0; i < CyclesPerUs; i++)
        __NOP();
    return false;
nextloop:
		I2C_Stop(I2C0, 1);
    for (i = 0; i < CyclesPerUs; i++)
        __NOP();
    return false;
#endif
}


bool read_CS8416_regs(uint8_t reg, uint8_t *buf)
{
	uint32_t i;
	uint8_t ack;
#if SOFT

    i2c_start();
    i2c_write_byte(CS8416_W);
    if (i2c_waitack())
        goto rd_fail;	
	
    i2c_write_byte(reg);
    if (i2c_waitack())
        goto rd_fail;
		
    i2c_start();
    i2c_write_byte(CS8416_R);
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
		
#else
		ack = I2C_Start(I2C0,(CS8416_W)|0, 1);
		if(ack == 0)
			goto nextloop;

		ack = I2C_Write(I2C0,reg, 1);
		if(ack == 0)
			goto nextloop;
		
		ack = I2C_Start(I2C0,(CS8416_W)|1, 1);
		if(ack == 0)
			goto nextloop;
		
		*buf = I2C_Read(I2C0, 1, 1);
		I2C_Read(I2C0, 0, 0);// NAck = 1
				
nextloop:
		I2C_Stop(I2C0, 1);
    for (i = 0; i < CyclesPerUs; i++)
        __NOP();
    return false;
#endif
}

void cs8416_init(void){
	  GPIO_SetBit(GPIOM, PIN6);//引脚RESET 高电平正常工作
//		read_CS8416_regs(Control4_Set,&valuetemp);//先读出寄存器值	
//		value=valuetemp;
		value |=CLOCKS;//把选择的寄存器值写入寄存器 [7]1内部时钟开启 [6]0RMCK输出
		write_CS8416_regs(Control4_Set,value);
	
			swm_delay_ms(5);
	
//		read_CS8416_regs(Control5_Set,&valuetemp);//先读出寄存器值	
//		value=valuetemp;
//		value |= SCLK_LRCKIO;//把选择的寄存器值写入寄存器 [7]1内部时钟开启 [6]0RMCK输出
//		write_CS8416_regs(Control5_Set, value);//0x05  [7]1输出
	
	
	write_CS8416_regs(Control5_Set, 0x85);//0x05  [7]1输出
	swm_delay_ms(5);
	write_CS8416_regs(0x00, 0x04);
	//TRUNC=1 - Incoming data is truncated according to the length specified in the channel status data.
	//根据通道状态数据中指定的长度截断传入数据
	swm_delay_ms(5);
	write_CS8416_regs(0x01, 0x86);
	swm_delay_ms(5);
	write_CS8416_regs(0x02, 0x45);
	swm_delay_ms(5);
//	write_CS8416_regs(0x04, 0x80);
//	swm_delay_ms(10);


	
}


