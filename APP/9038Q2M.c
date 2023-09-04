#include "9038Q2M.h"
#include "main.h"
#include "soft_i2c.h"



#include "lvgl/lvgl.h"



/**
 * @brief gt9x写寄存器-第一片9038（控制左声道）
 * @param reg 要写入的寄存器
 * @param buf 要写入的数组
 * @param len 要写入的数组的长度
 * @return true
 * @return false
 */
bool write_9038q2m_regs_channe_L(uint8_t reg, uint8_t buf)
{
    uint32_t i;

		i2c_start();

    //发送器件地址
    i2c_write_byte(ARR9038_channel_L_W);
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









/**
 * @brief gt9x读寄存器-第一片9038（控制左声道）
 * @param reg 要读取的寄存器
 * @param buf 要读取的数组
 * @param len 要读取的数组的长度
 * @return true
 * @return false
 */
bool read_9038q2m_regs_channe_L(uint8_t reg, uint8_t *buf)
{
    uint32_t i;

    i2c_start();
    i2c_write_byte(ARR9038_channel_L_W);
    if (i2c_waitack())
        goto rd_fail;	
	
    i2c_write_byte(reg);
    if (i2c_waitack())
        goto rd_fail;
		
    i2c_start();
    i2c_write_byte(ARR9038_channel_L_R);
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





/**
 * @brief gt9x写寄存器-第二片9038（控制右声道）
 * @param reg 要写入的寄存器
 * @param buf 要写入的数组
 * @param len 要写入的数组的长度
 * @return true
 * @return false
 */
bool write_9038q2m_regs_channe_R(uint8_t reg, uint8_t buf)
{
    uint32_t i;

		i2c_start();

    //发送器件地址
    i2c_write_byte(ARR9038_channel_R_W);
    if (i2c_waitack())
        goto wr_fail;

    i2c_write_byte(reg);  //选择写入的寄存器地址
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



/**
 * @brief gt9x读寄存器-第二片9038（控制右声道）
 * @param reg 要读取的寄存器
 * @param buf 要读取的数组
 * @param len 要读取的数组的长度
 * @return true
 * @return false
 */
bool read_9038q2m_regs_channe_R(uint8_t reg, uint8_t *buf)
{
    uint32_t i;

    i2c_start();
    i2c_write_byte(ARR9038_channel_R_W);
    if (i2c_waitack())
        goto rd_fail;	
	
    i2c_write_byte(reg);
    if (i2c_waitack())
        goto rd_fail;
		
    i2c_start();
    i2c_write_byte(ARR9038_channel_R_R);
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




 uint8_t Volume=224;//音量数值0~255对应0DB~-127.5DB
//控制音量--左右两片9038一样的音量
bool witer_volume(uint8_t set_vol)
{
	bool flag=1;
	bool temp=0;//获取是否成功写入
	
	temp=write_9038q2m_regs_channe_L(ARR9038_volume_L,Volume);//写左声道的9038寄存器
	flag &=temp;//对比是否成功写入
	temp=write_9038q2m_regs_channe_L(ARR9038_volume_R,Volume);//写左声道的9038寄存器
	flag &=temp;//对比是否成功写入
	
	temp=write_9038q2m_regs_channe_R(ARR9038_volume_L,Volume);//写右声道的9038寄存器
	flag &=temp;//对比是否成功写入
	temp=write_9038q2m_regs_channe_R(ARR9038_volume_R,Volume);//写右声道的9038寄存器
	flag &=temp;//对比是否成功写入
	
	return flag;//返回是否成功写入两片9038
	
}

//同时写入两片9038
bool Witer_LR_9038(uint8_t reg, uint8_t buf)
{
	bool flag=1;
	bool temp=0;//获取是否成功写入
	
	temp=write_9038q2m_regs_channe_L(reg,buf);//写左声道的9038寄存器
	flag &=temp;//对比是否成功写入
	temp=write_9038q2m_regs_channe_R(reg,buf);//写右声道的9038寄存器
	flag &=temp;//对比是否成功写入
	
	return flag;//返回是否成功写入两片9038
	
}




////同时读出两片9038-有问题，还没搞好
//bool Read_LR_9038(uint8_t reg, uint8_t* buf)
//{
//	bool flag=1;
//	bool temp=0;//获取是否成功写入
//	uint8_t buf_L;//读出左边那片9038，临时存放
//	uint8_t buf_R;//读出右边那片9038，临时存放
//	
//	temp=read_9038q2m_regs_channe_L(reg,&buf_L);
//	flag &=temp;//对比是否成功读取
//	swm_delay_ms(2);
//	temp=read_9038q2m_regs_channe_R(reg,&buf_R);
//	flag &=temp;//对比是否成功读取	
//	
//	if(buf_L==buf_R)//如果左右两个值一样
//	{
//		buf=buf_L;
//	}else{
//		buf=0XFF;//不一样就输出0XFF
//	}
//		
//	return flag;//返回是否成功读取两片9038
//}











