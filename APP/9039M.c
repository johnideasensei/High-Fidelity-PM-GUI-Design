#include "9039M.h"
#include "main.h"
#include "soft_i2c.h"



#include "lvgl/lvgl.h"

extern uint8_t valuetemp,debug_read,value;
bool write_9039m_regs(uint8_t reg, uint8_t buf)
{

    uint32_t i;

		i2c_start();

    //����������ַ
    i2c_write_byte(ARR9039_W);
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




bool read_9039m_regs(uint8_t reg, uint8_t *buf)
{
    uint32_t i;

    i2c_start();
    i2c_write_byte(ARR9039_W);
    if (i2c_waitack())
        goto rd_fail;	
	
    i2c_write_byte(reg);
    if (i2c_waitack())
        goto rd_fail;
		
    i2c_start();
    i2c_write_byte(ARR9039_R);
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



bool write_volume(uint8_t lVolume,uint8_t rVolume)
{
	write_lvolume(lVolume);
	write_rvolume(rVolume);
	return 1;
}

void write_lvolume(uint8_t lVolume)
{
	write_9039m_regs(ARR9039_volume_CH1,lVolume);//дͨ��1��9039�Ĵ���
//	swm_delay_ms(1);
	write_9039m_regs(ARR9039_volume_CH3,lVolume);//дͨ��3��9039�Ĵ���
	swm_delay_ms(1);
	write_9039m_regs(ARR9039_volume_CH5,lVolume);//дͨ��5��9039�Ĵ���
//	swm_delay_ms(1);
	write_9039m_regs(ARR9039_volume_CH7,lVolume);//дͨ��7��9039�Ĵ���
	swm_delay_ms(1);
}

void write_rvolume(uint8_t rVolume)
{
	write_9039m_regs(ARR9039_volume_CH2,rVolume);//дͨ��2��9039�Ĵ���
//	swm_delay_ms(1);
	write_9039m_regs(ARR9039_volume_CH4,rVolume);//дͨ��4��9039�Ĵ���
	swm_delay_ms(1);
	write_9039m_regs(ARR9039_volume_CH6,rVolume);//дͨ��6��9039�Ĵ���
//	swm_delay_ms(1);
	write_9039m_regs(ARR9039_volume_CH8,rVolume);//дͨ��8��9039�Ĵ���
	swm_delay_ms(1);
}


//�ڶ���PCB-9039MPRO��ʼ������
void es9039_init(void){
	GPIO_SetBit(GPIOD, PIN15);//����RESET �ߵ�ƽ��������
	swm_delay_ms(5);

//	write_9039m_regs(0x56, 0xFF);//��������ͨ��

	write_9039m_regs(0x00, 0x42);//PCM����ͨ��·��-��Ĭ�ϣ�����DAC������64FSģʽ
//	write_9039m_regs(0x00, 0x6E);//PCM����ͨ��·��-1�ָ�357��2�ָ�468������DAC������64FSģʽ
		swm_delay_ms(5);
		
	write_9039m_regs(0x01, 0xB1);//����DAC_CLKʱ�ӣ������첽ģʽ������TDM����0XB1��TDM������PCM�������Ҫ����DSD�����Ҫ���ó�0XB2��Dop����0XB4��SPDIF����0XB8
		swm_delay_ms(5);
 	write_9039m_regs(0x2A, 0xF8);//����GPIO4��5��6��7��8����
		swm_delay_ms(5);
  write_9039m_regs(0x3B, 0x01);//TDM���ã�����ÿ��TDM֡���ж��ٸ�ͨ�������ݣ�����Ϊ1+1=2ͨ��	
	swm_delay_ms(5);

//9039MPRO�Զ��������ּĴ���		
  write_9039m_regs(0x40, 0x00);//TDM CH1����,ѡ��TDM�е�L����
	swm_delay_ms(5);
	write_9039m_regs(0x41, 0x01);//TDM CH2����,ѡ��TDM�е�R����
	swm_delay_ms(5);
	write_9039m_regs(0x42, 0x00);//TDM CH3����,ѡ��TDM�е�L����
	swm_delay_ms(5);
	write_9039m_regs(0x43, 0x01);//TDM CH4����,ѡ��TDM�е�R����
	swm_delay_ms(5);
	write_9039m_regs(0x44, 0x00);//TDM CH5����,ѡ��TDM�е�L����
	swm_delay_ms(5);
	write_9039m_regs(0x45, 0x01);//TDM CH6����,ѡ��TDM�е�R����
	swm_delay_ms(5);
  write_9039m_regs(0x46, 0x00);//TDM CH7����,ѡ��TDM�е�L����
	swm_delay_ms(5);
	write_9039m_regs(0x47, 0x01);//TDM CH8����,ѡ��TDM�е�R����
	swm_delay_ms(5);

//	write_9039m_regs(0x57, 0xFF);//��ת�����������
//	write_9039m_regs(0x5A, 0x07);//����8x �������˲���������IIR�˲�������


//9039MPRO�Զ��������ּĴ���,��������л�������ʱ��Ų��ᱬ��
	write_9039m_regs(0x7C,0XFF);
	swm_delay_ms(5);
	write_9039m_regs(0x7D,0X07);
	swm_delay_ms(5);
	write_9039m_regs(0x7E,0X00);
	swm_delay_ms(5);
	write_9039m_regs(0x7F,0X00);
	swm_delay_ms(5);
	write_9039m_regs(0x80,0X00);//25
	swm_delay_ms(5);
	write_9039m_regs(0x81,0X00);//5B
	swm_delay_ms(5);
	
	//PCM
	
	write_9039m_regs(0x01, 0xBF);//BF�������н��루PCM�ܳ�����������DAC_CLKʱ�ӣ������첽ģʽ������TDM����0XB1��TDM������PCM�������Ҫ����DSD�����Ҫ���ó�0XB2��Dop����0XB4��SPDIF����0XB8
	swm_delay_ms(5);	
	write_9039m_regs(0x39, 0x41);//40TDM   41�Զ�ѡ��PCM�ܳ�����  42DSD   44DOP
	swm_delay_ms(50);		
//		//DSD
//		write_9039m_regs(0x01, 0xB2);//����DAC_CLKʱ�ӣ������첽ģʽ������TDM����0XB1��TDM������PCM�������Ҫ����DSD�����Ҫ���ó�0XB2��Dop����0XB4��SPDIF����0XB8
//		write_9039m_regs(0x39, 0x42);//	DSD
//		
		//Dop
//		write_9039m_regs(0x01, 0xB4);//����DAC_CLKʱ�ӣ������첽ģʽ������TDM����0XB1��TDM������PCM�������Ҫ����DSD�����Ҫ���ó�0XB2��Dop����0XB4��SPDIF����0XB8
//		write_9039m_regs(0x39, 0x44);//TDM
//	
}



