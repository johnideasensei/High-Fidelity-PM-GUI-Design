#include "9039M.h"
#include "main.h"
#include "soft_i2c.h"



#include "lvgl/lvgl.h"

extern uint8_t valuetemp,debug_read,value;
bool write_9039m_regs(uint8_t reg, uint8_t buf)
{

    uint32_t i;

		i2c_start();

    //发送器件地址
    i2c_write_byte(ARR9039_W);
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
	write_9039m_regs(ARR9039_volume_CH1,lVolume);//写通道1的9039寄存器
//	swm_delay_ms(1);
	write_9039m_regs(ARR9039_volume_CH3,lVolume);//写通道3的9039寄存器
	swm_delay_ms(1);
	write_9039m_regs(ARR9039_volume_CH5,lVolume);//写通道5的9039寄存器
//	swm_delay_ms(1);
	write_9039m_regs(ARR9039_volume_CH7,lVolume);//写通道7的9039寄存器
	swm_delay_ms(1);
}

void write_rvolume(uint8_t rVolume)
{
	write_9039m_regs(ARR9039_volume_CH2,rVolume);//写通道2的9039寄存器
//	swm_delay_ms(1);
	write_9039m_regs(ARR9039_volume_CH4,rVolume);//写通道4的9039寄存器
	swm_delay_ms(1);
	write_9039m_regs(ARR9039_volume_CH6,rVolume);//写通道6的9039寄存器
//	swm_delay_ms(1);
	write_9039m_regs(ARR9039_volume_CH8,rVolume);//写通道8的9039寄存器
	swm_delay_ms(1);
}


//第二块PCB-9039MPRO初始化配置
void es9039_init(void){
	GPIO_SetBit(GPIOD, PIN15);//引脚RESET 高电平正常工作
	swm_delay_ms(5);

//	write_9039m_regs(0x56, 0xFF);//静音所有通道

	write_9039m_regs(0x00, 0x42);//PCM数据通道路由-按默认，开启DAC，开启64FS模式
//	write_9039m_regs(0x00, 0x6E);//PCM数据通道路由-1分给357，2分给468，开启DAC，开启64FS模式
		swm_delay_ms(5);
		
	write_9039m_regs(0x01, 0xB1);//启动DAC_CLK时钟，启动异步模式，启动TDM解码0XB1（TDM包括了PCM），如果要启动DSD解码就要配置成0XB2，Dop解码0XB4，SPDIF解码0XB8
		swm_delay_ms(5);
 	write_9039m_regs(0x2A, 0xF8);//启用GPIO4、5、6、7、8输入
		swm_delay_ms(5);
  write_9039m_regs(0x3B, 0x01);//TDM配置，配置每个TDM帧中有多少个通道的数据，配置为1+1=2通道	
	swm_delay_ms(5);

//9039MPRO自动静音部分寄存器		
  write_9039m_regs(0x40, 0x00);//TDM CH1配置,选择TDM中的L声道
	swm_delay_ms(5);
	write_9039m_regs(0x41, 0x01);//TDM CH2配置,选择TDM中的R声道
	swm_delay_ms(5);
	write_9039m_regs(0x42, 0x00);//TDM CH3配置,选择TDM中的L声道
	swm_delay_ms(5);
	write_9039m_regs(0x43, 0x01);//TDM CH4配置,选择TDM中的R声道
	swm_delay_ms(5);
	write_9039m_regs(0x44, 0x00);//TDM CH5配置,选择TDM中的L声道
	swm_delay_ms(5);
	write_9039m_regs(0x45, 0x01);//TDM CH6配置,选择TDM中的R声道
	swm_delay_ms(5);
  write_9039m_regs(0x46, 0x00);//TDM CH7配置,选择TDM中的L声道
	swm_delay_ms(5);
	write_9039m_regs(0x47, 0x01);//TDM CH8配置,选择TDM中的R声道
	swm_delay_ms(5);

//	write_9039m_regs(0x57, 0xFF);//反转所有声道输出
//	write_9039m_regs(0x5A, 0x07);//跳过8x 超采样滤波器、跳过IIR滤波器配置


//9039MPRO自动静音部分寄存器,做了这个切换歌曲的时候才不会爆音
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
	
	write_9039m_regs(0x01, 0xBF);//BF开启所有解码（PCM能出声），启动DAC_CLK时钟，启动异步模式，启动TDM解码0XB1（TDM包括了PCM），如果要启动DSD解码就要配置成0XB2，Dop解码0XB4，SPDIF解码0XB8
	swm_delay_ms(5);	
	write_9039m_regs(0x39, 0x41);//40TDM   41自动选择（PCM能出声）  42DSD   44DOP
	swm_delay_ms(50);		
//		//DSD
//		write_9039m_regs(0x01, 0xB2);//启动DAC_CLK时钟，启动异步模式，启动TDM解码0XB1（TDM包括了PCM），如果要启动DSD解码就要配置成0XB2，Dop解码0XB4，SPDIF解码0XB8
//		write_9039m_regs(0x39, 0x42);//	DSD
//		
		//Dop
//		write_9039m_regs(0x01, 0xB4);//启动DAC_CLK时钟，启动异步模式，启动TDM解码0XB1（TDM包括了PCM），如果要启动DSD解码就要配置成0XB2，Dop解码0XB4，SPDIF解码0XB8
//		write_9039m_regs(0x39, 0x44);//TDM
//	
}



