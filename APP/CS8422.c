#include "CS8422.h"
#include "main.h"
#include "soft_i2c.h"


#include "lvgl/lvgl.h"

extern uint8_t valuetemp,debug_read,value;


bool write_CS8422_regs(uint8_t reg, uint8_t buf)
{
    uint32_t i;

		i2c_start();

    i2c_write_byte(CS8422_W);
	
	

    if (i2c_waitack())
			{
				i2c_stop();
				return 0;
			}

    i2c_write_byte(reg);
    if (i2c_waitack())
			{
				i2c_stop();
				return 0;
			}

		i2c_write_byte(buf);
    if (i2c_waitack())
			{
				i2c_stop();
				return 0;
		 	}



    i2c_stop();
			
		swm_delay_ms(1);


    return 1;



}


bool read_CS8422_regs(uint8_t reg, uint8_t *buf)
{
    uint32_t i;

    i2c_start();
    i2c_write_byte(CS8422_W);
    if (i2c_waitack())
        goto rd_fail;	
	
    i2c_write_byte(reg);
    if (i2c_waitack())
        goto rd_fail;
		
    i2c_start();
    i2c_write_byte(CS8422_R);
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

void cs8422_init(void){
	  GPIO_SetBit(GPIOM, PIN6);
	
	//初始化为同步模式
	
	swm_delay_ms(10);
	write_CS8422_regs(0x02,0x20);
	swm_delay_ms(2);
	write_CS8422_regs(0x03,0x84);// 0X84光纤输入	0XC4是同轴输入
	swm_delay_ms(2);
	write_CS8422_regs(0x04,0xA4);
	swm_delay_ms(2);
	write_CS8422_regs(0x05,0x00);
	swm_delay_ms(2);
	write_CS8422_regs(0x06,0x00);
	swm_delay_ms(2);
	write_CS8422_regs(0x07,0x00);
	swm_delay_ms(2);
	write_CS8422_regs(0x08,0x48);//使用外部时钟要除以2才能用  ，写入0X02就是SRC“工作时”使用PLL   写入00就是SRC“工作时”外部XTI时钟，采样率变化=OLRCK=MCLK/256=256fs，SAO_MCLK选择“输出”的时钟来自PLL还是来自XTI，选择来自XTI
	swm_delay_ms(2);
	write_CS8422_regs(0x09,0x48);// 恢复主时钟的，SRC中的配置不影响这个，输出的MCLK是采样率的多少倍  选择256倍，启动软静音
	swm_delay_ms(2);
	write_CS8422_regs(0x0A,0x52);//数据路由，写入02输出来自SRC，SRC数据来自AES3        写入52输出来自AES3直接输出，还有8422的静音控制也在这里
	swm_delay_ms(2);
	write_CS8422_regs(0x0B,0x00);//控制I2S输入端口的，没用（默认00）
	swm_delay_ms(2);
	write_CS8422_regs(0x0C,0x94);//SDOUT1输出端口是输出，I2S格式，位时钟速率/LRCK=64，禁用TDM，24位深
	swm_delay_ms(2);
	write_CS8422_regs(0x0D,0x94);//SDOUT2输出端口是输出，I2S格式，位时钟速率/LRCK=64，禁用TDM，24位深

}


