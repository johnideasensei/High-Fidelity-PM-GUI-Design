#include "9038Q2M.h"
#include "main.h"
#include "soft_i2c.h"



#include "lvgl/lvgl.h"



/**
 * @brief gt9xд�Ĵ���-��һƬ9038��������������
 * @param reg Ҫд��ļĴ���
 * @param buf Ҫд�������
 * @param len Ҫд�������ĳ���
 * @return true
 * @return false
 */
bool write_9038q2m_regs_channe_L(uint8_t reg, uint8_t buf)
{
    uint32_t i;

		i2c_start();

    //����������ַ
    i2c_write_byte(ARR9038_channel_L_W);
    if (i2c_waitack())
        goto wr_fail;

    i2c_write_byte(reg);//ѡ��д��ļĴ�����ַ
    if (i2c_waitack())
        goto wr_fail;
		
		i2c_write_byte(buf);//д��Ĵ�������ֵ
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
 * @brief gt9x���Ĵ���-��һƬ9038��������������
 * @param reg Ҫ��ȡ�ļĴ���
 * @param buf Ҫ��ȡ������
 * @param len Ҫ��ȡ������ĳ���
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
 * @brief gt9xд�Ĵ���-�ڶ�Ƭ9038��������������
 * @param reg Ҫд��ļĴ���
 * @param buf Ҫд�������
 * @param len Ҫд�������ĳ���
 * @return true
 * @return false
 */
bool write_9038q2m_regs_channe_R(uint8_t reg, uint8_t buf)
{
    uint32_t i;

		i2c_start();

    //����������ַ
    i2c_write_byte(ARR9038_channel_R_W);
    if (i2c_waitack())
        goto wr_fail;

    i2c_write_byte(reg);  //ѡ��д��ļĴ�����ַ
    if (i2c_waitack())
        goto wr_fail;
		
		i2c_write_byte(buf);//д��Ĵ�������ֵ
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
 * @brief gt9x���Ĵ���-�ڶ�Ƭ9038��������������
 * @param reg Ҫ��ȡ�ļĴ���
 * @param buf Ҫ��ȡ������
 * @param len Ҫ��ȡ������ĳ���
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




 uint8_t Volume=224;//������ֵ0~255��Ӧ0DB~-127.5DB
//��������--������Ƭ9038һ��������
bool witer_volume(uint8_t set_vol)
{
	bool flag=1;
	bool temp=0;//��ȡ�Ƿ�ɹ�д��
	
	temp=write_9038q2m_regs_channe_L(ARR9038_volume_L,Volume);//д��������9038�Ĵ���
	flag &=temp;//�Ա��Ƿ�ɹ�д��
	temp=write_9038q2m_regs_channe_L(ARR9038_volume_R,Volume);//д��������9038�Ĵ���
	flag &=temp;//�Ա��Ƿ�ɹ�д��
	
	temp=write_9038q2m_regs_channe_R(ARR9038_volume_L,Volume);//д��������9038�Ĵ���
	flag &=temp;//�Ա��Ƿ�ɹ�д��
	temp=write_9038q2m_regs_channe_R(ARR9038_volume_R,Volume);//д��������9038�Ĵ���
	flag &=temp;//�Ա��Ƿ�ɹ�д��
	
	return flag;//�����Ƿ�ɹ�д����Ƭ9038
	
}

//ͬʱд����Ƭ9038
bool Witer_LR_9038(uint8_t reg, uint8_t buf)
{
	bool flag=1;
	bool temp=0;//��ȡ�Ƿ�ɹ�д��
	
	temp=write_9038q2m_regs_channe_L(reg,buf);//д��������9038�Ĵ���
	flag &=temp;//�Ա��Ƿ�ɹ�д��
	temp=write_9038q2m_regs_channe_R(reg,buf);//д��������9038�Ĵ���
	flag &=temp;//�Ա��Ƿ�ɹ�д��
	
	return flag;//�����Ƿ�ɹ�д����Ƭ9038
	
}




////ͬʱ������Ƭ9038-�����⣬��û���
//bool Read_LR_9038(uint8_t reg, uint8_t* buf)
//{
//	bool flag=1;
//	bool temp=0;//��ȡ�Ƿ�ɹ�д��
//	uint8_t buf_L;//���������Ƭ9038����ʱ���
//	uint8_t buf_R;//�����ұ���Ƭ9038����ʱ���
//	
//	temp=read_9038q2m_regs_channe_L(reg,&buf_L);
//	flag &=temp;//�Ա��Ƿ�ɹ���ȡ
//	swm_delay_ms(2);
//	temp=read_9038q2m_regs_channe_R(reg,&buf_R);
//	flag &=temp;//�Ա��Ƿ�ɹ���ȡ	
//	
//	if(buf_L==buf_R)//�����������ֵһ��
//	{
//		buf=buf_L;
//	}else{
//		buf=0XFF;//��һ�������0XFF
//	}
//		
//	return flag;//�����Ƿ�ɹ���ȡ��Ƭ9038
//}











