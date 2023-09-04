//----------------------------头文件依赖----------------------------//
#include "LCD_TL040WVS03.h"
#include "dev_systick.h"

/* TFT-LCD 模组名 : TL040WVS03 (冠显光电), 屏内 COG 芯片型号 : ST7701S */

//----------------------------本地宏----------------------------//


#define LCD_GPIO_CS 			GPIOB
#define LCD_PIN_CS 				PIN0

#define LCD_GPIO_SCK 			GPION
#define LCD_PIN_SCK 			PIN5

#define LCD_GPIO_SDA 			GPION
#define LCD_PIN_SDA 			PIN4



//操作宏
#if 	0 //库函数

#define PIN_HIGH_RST() 		GPIO_AtomicSetBit(LCD_GPIO_RST, LCD_PIN_RST) //GPIO_SetBit(LCD_GPIO_RST, LCD_PIN_RST)
#define PIN_LOW_RST() 		GPIO_AtomicClrBit(LCD_GPIO_RST, LCD_PIN_RST) //GPIO_ClrBit(LCD_GPIO_RST, LCD_PIN_RST)

#define PIN_HIGH_CS() 		GPIO_AtomicSetBit(LCD_GPIO_CS, LCD_PIN_CS) //GPIO_SetBit(LCD_GPIO_CS, LCD_PIN_CS)
#define PIN_LOspi_write_cmdS() 		GPIO_AtomicClrBit(LCD_GPIO_CS, LCD_PIN_CS) //GPIO_ClrBit(LCD_GPIO_CS, LCD_PIN_CS)

#define PIN_HIGH_SCK() 		GPIO_AtomicSetBit(LCD_GPIO_SCK, LCD_PIN_SCK) //GPIO_SetBit(LCD_GPIO_SCK, LCD_PIN_SCK)
#define PIN_LOW_SCK() 		GPIO_AtomicClrBit(LCD_GPIO_SCK, LCD_PIN_SCK) //GPIO_ClrBit(LCD_GPIO_SCK, LCD_PIN_SCK)

#define PIN_HIGH_SDA() 		GPIO_AtomicSetBit(LCD_GPIO_SDA, LCD_PIN_SDA) //GPIO_SetBit(LCD_GPIO_SDA, LCD_PIN_SDA)
#define PIN_LOW_SDA() 		GPIO_AtomicClrBit(LCD_GPIO_SDA, LCD_PIN_SDA) //GPIO_ClrBit(LCD_GPIO_SDA, LCD_PIN_SDA)

#else //寄存器

#define PIN_HIGH_RST() 		(*(&LCD_GPIO_RST->DATAPIN0 + LCD_PIN_RST) = 1) //LCD_GPIO_RST->ODR |= (0x01 << LCD_PIN_RST)
#define PIN_LOW_RST() 		(*(&LCD_GPIO_RST->DATAPIN0 + LCD_PIN_RST) = 0) //LCD_GPIO_RST->ODR &= ~(0x01 << LCD_PIN_RST)

#define PIN_HIGH_CS() 		(*(&LCD_GPIO_CS->DATAPIN0 + LCD_PIN_CS) = 1) //LCD_GPIO_CS->ODR |= (0x01 << LCD_PIN_CS)
#define PIN_LOspi_write_cmdS() 		(*(&LCD_GPIO_CS->DATAPIN0 + LCD_PIN_CS) = 0) //LCD_GPIO_CS->ODR &= ~(0x01 << LCD_PIN_CS)

#define PIN_HIGH_SCK() 		(*(&LCD_GPIO_SCK->DATAPIN0 + LCD_PIN_SCK) = 1) //LCD_GPIO_SCK->ODR |= (0x01 << LCD_PIN_SCK)
#define PIN_LOW_SCK() 		(*(&LCD_GPIO_SCK->DATAPIN0 + LCD_PIN_SCK) = 0) //LCD_GPIO_SCK->ODR &= ~(0x01 << LCD_PIN_SCK)

#define PIN_HIGH_SDA() 		(*(&LCD_GPIO_SDA->DATAPIN0 + LCD_PIN_SDA) = 1) //LCD_GPIO_SDA->ODR |= (0x01 << LCD_PIN_SDA)
#define PIN_LOW_SDA() 		(*(&LCD_GPIO_SDA->DATAPIN0 + LCD_PIN_SDA) = 0) //LCD_GPIO_SDA->ODR &= ~(0x01 << LCD_PIN_SDA)

#endif

//----------------------------本地函数定义----------------------------//
static inline void spi_delay_time(void)
{
	for (int us = 0; us < 100; us++)
		__NOP();
}

static void spi_send_data(unsigned char i)
{
	for (unsigned char n = 0; n < 8; n++)
	{
		if (i & 0x80)
		{
			PIN_HIGH_SDA();
		}
		else
		{
			PIN_LOW_SDA();
		}
		i <<= 1;
		spi_delay_time();
		PIN_LOW_SCK();
		spi_delay_time();
		PIN_HIGH_SCK();
		spi_delay_time();
	}
}

static void spi_write_cmd(unsigned char i)
{
	PIN_LOspi_write_cmdS();
	spi_delay_time();
	PIN_LOW_SDA();
	spi_delay_time();
	PIN_LOW_SCK();
	spi_delay_time();

	PIN_HIGH_SCK();
	spi_delay_time();
	spi_send_data(i);
	spi_delay_time();
	PIN_HIGH_CS();
	spi_delay_time();
}

static void spi_write_data(unsigned char i)
{
	PIN_LOspi_write_cmdS();
	spi_delay_time();
	PIN_HIGH_SDA();
	spi_delay_time();
	PIN_LOW_SCK();
	spi_delay_time();

	PIN_HIGH_SCK();
	spi_delay_time();
	spi_send_data(i);
	spi_delay_time();
	PIN_HIGH_CS();
	spi_delay_time();
}

static void spi_port_init(void)
{
	GPIO_Init(LCD_GPIO_CS, LCD_PIN_CS, 1, 1, 0, 0);
	GPIO_Init(LCD_GPIO_SCK, LCD_PIN_SCK, 1, 1, 0, 0);
	GPIO_Init(LCD_GPIO_SDA, LCD_PIN_SDA, 1, 1, 0, 0);
	GPIO_Init(LCD_GPIO_RST, LCD_PIN_RST, 1, 1, 0, 0);
	PIN_HIGH_CS();
	PIN_HIGH_SCK();
	PIN_HIGH_SDA();

	PIN_HIGH_RST(); //复位
	spi_delay_time();//swm_delay_ms(1);
	PIN_LOW_RST();
	swm_delay_ms(1);
	PIN_HIGH_RST();
}

//----------------------------对外函数定义----------------------------//
void lcd_spi_init_TL040WVS03(void)
{
	spi_port_init();

	PIN_LOspi_write_cmdS();
	spi_delay_time();
	PIN_HIGH_SDA();
	swm_delay_ms(8);

	PIN_LOW_SDA();
	swm_delay_ms(50);
	PIN_HIGH_SDA();
	swm_delay_ms(50);

	spi_write_cmd(0x11); //BOE3.97IPS
	swm_delay_ms(150);
	
	
	
	
//老板买的960*400屏幕初始化	
 #if 1   
//st7701s+BOE3.99_400X960
//spi_write_cmd(0xFF);
//spi_write_data(0x77);
//spi_write_data(0x01);
//spi_write_data(0x00);
//spi_write_data(0x00);
//spi_write_data(0x13);
//spi_write_cmd(0xEF);
//spi_write_data(0x08);
//spi_write_cmd(0xFF);
//spi_write_data(0x77);
//spi_write_data(0x01);
//spi_write_data(0x00);
//spi_write_data(0x00);
//spi_write_data(0x10);
//spi_write_cmd(0xC0);
//spi_write_data(0x77);
//spi_write_data(0x00);
//spi_write_cmd(0xC1);
//spi_write_data(0x0E);
//spi_write_data(0x0C);
//spi_write_cmd(0xC2);
//spi_write_data(0x07);
//spi_write_data(0x02);
//spi_write_cmd(0xCC);
//spi_write_data(0x30);

//spi_write_cmd(0xCD);
//spi_write_data(0x00);//08

//spi_write_cmd(0xB0);
//spi_write_data(0x00);
//spi_write_data(0x17);
//spi_write_data(0x1F);
//spi_write_data(0x0E);
//spi_write_data(0x11);
//spi_write_data(0x06);
//spi_write_data(0x0D);
//spi_write_data(0x08);
//spi_write_data(0x07);
//spi_write_data(0x26);
//spi_write_data(0x03);
//spi_write_data(0x11);
//spi_write_data(0x0F);
//spi_write_data(0x2A);
//spi_write_data(0x31);
//spi_write_data(0x1C);
//spi_write_cmd(0xB1);
//spi_write_data(0x00);
//spi_write_data(0x17);
//spi_write_data(0x1F);
//spi_write_data(0x0D);
//spi_write_data(0x11);
//spi_write_data(0x07);
//spi_write_data(0x0C);
//spi_write_data(0x08);
//spi_write_data(0x08);
//spi_write_data(0x26);
//spi_write_data(0x04);
//spi_write_data(0x11);
//spi_write_data(0x0F);
//spi_write_data(0x2A);
//spi_write_data(0x31);
//spi_write_data(0x1C);
//spi_write_cmd(0xFF);
//spi_write_data(0x77);
//spi_write_data(0x01);
//spi_write_data(0x00);
//spi_write_data(0x00);
//spi_write_data(0x11);
//spi_write_cmd(0xB0);
//spi_write_data(0x5C);
//spi_write_cmd(0xB1);
//spi_write_data(0x68);
//spi_write_cmd(0xB2);
//spi_write_data(0x85);
//spi_write_cmd(0xB3);
//spi_write_data(0x80);
//spi_write_cmd(0xB5);
//spi_write_data(0x49);
//spi_write_cmd(0xB7);
//spi_write_data(0x87);
//spi_write_cmd(0xB8);
//spi_write_data(0x22);
//spi_write_cmd(0xB9);
//spi_write_data(0x10);
//spi_write_data(0x1F);
//spi_write_cmd(0xBB);
//spi_write_data(0x03);
//spi_write_cmd(0xC1);
//spi_write_data(0x88);
//spi_write_cmd(0xC2);
//spi_write_data(0x88);
//spi_write_cmd(0xD0);
//spi_write_data(0x88);
//spi_write_cmd(0xE0); 
//spi_write_data(0x00);
//spi_write_data(0x00);
//spi_write_data(0x02); 
//spi_write_data(0x00);
//spi_write_data(0x00);
//spi_write_data(0x0C);
//spi_write_cmd(0xE1); 
//spi_write_data(0x03);
//spi_write_data(0x96);
//spi_write_data(0x05);
//spi_write_data(0x96);
//spi_write_data(0x02);
//spi_write_data(0x96);
//spi_write_data(0x04);
//spi_write_data(0x96);
//spi_write_data(0x00);
//spi_write_data(0x44);
//spi_write_data(0x44);
//spi_write_cmd(0xE2); 
//spi_write_data(0x00);
//spi_write_data(0x00);
//spi_write_data(0x03);
//spi_write_data(0x03);
//spi_write_data(0x00);
//spi_write_data(0x00);
//spi_write_data(0x02);
//spi_write_data(0x00);
//spi_write_data(0x00);
//spi_write_data(0x00);
//spi_write_data(0x02);
//spi_write_data(0x00);
//spi_write_cmd(0xE3); 
//spi_write_data(0x00);
//spi_write_data(0x00);
//spi_write_data(0x33);
//spi_write_data(0x33);
//spi_write_cmd(0xE4); 
//spi_write_data(0x44);
//spi_write_data(0x44);
//spi_write_cmd(0xE5); 
//spi_write_data(0x0B);
//spi_write_data(0xD4);
//spi_write_data(0x28);
//spi_write_data(0x8C);
//spi_write_data(0x0D);
//spi_write_data(0xD6);
//spi_write_data(0x28);
//spi_write_data(0x8C);
//spi_write_data(0x07);
//spi_write_data(0xD0);
//spi_write_data(0x28);
//spi_write_data(0x8C);
//spi_write_data(0x09);
//spi_write_data(0xD2);
//spi_write_data(0x28);
//spi_write_data(0x8C);
//spi_write_cmd(0xE6);  
//spi_write_data(0x00);
//spi_write_data(0x00);
//spi_write_data(0x33);
//spi_write_data(0x33);
//spi_write_cmd(0xE7); 
//spi_write_data(0x44);
//spi_write_data(0x44);
//spi_write_cmd(0xE8); 
//spi_write_data(0x0A);
//spi_write_data(0xD5);
//spi_write_data(0x28);
//spi_write_data(0x8C);
//spi_write_data(0x0C);
//spi_write_data(0xD7);
//spi_write_data(0x28);
//spi_write_data(0x8C);
//spi_write_data(0x06);
//spi_write_data(0xD1);
//spi_write_data(0x28);
//spi_write_data(0x8C);
//spi_write_data(0x08);
//spi_write_data(0xD3);
//spi_write_data(0x28);
//spi_write_data(0x8C);
//spi_write_cmd(0xEB); 
//spi_write_data(0x00);
//spi_write_data(0x01);
//spi_write_data(0xE4);
//spi_write_data(0xE4);
//spi_write_data(0x44);
//spi_write_data(0x00);
//spi_write_cmd(0xED);
//spi_write_data(0xFF);
//spi_write_data(0x45);
//spi_write_data(0x67);
//spi_write_data(0xFC);
//spi_write_data(0x01);
//spi_write_data(0x3F);
//spi_write_data(0xAB);
//spi_write_data(0xFF);
//spi_write_data(0xFF);
//spi_write_data(0xBA);
//spi_write_data(0xF3);
//spi_write_data(0x10);
//spi_write_data(0xCF);
//spi_write_data(0x76);
//spi_write_data(0x54);
//spi_write_data(0xFF);
//spi_write_cmd(0xEF);
//spi_write_data(0x10);
//spi_write_data(0x0D);
//spi_write_data(0x04);
//spi_write_data(0x08);
//spi_write_data(0x3F);
//spi_write_data(0x1F);
//spi_write_cmd(0xFF);
//spi_write_data(0x77);
//spi_write_data(0x01);
//spi_write_data(0x00);
//spi_write_data(0x00);
//spi_write_data(0x13);
//spi_write_cmd(0xE8);
//spi_write_data(0x00);
//spi_write_data(0x0E);
//spi_write_cmd(0x11);
//swm_delay_ms(120);
//spi_write_cmd(0xE8);
//spi_write_data(0x00);
//spi_write_data(0x0C);
//swm_delay_ms(20);
//spi_write_cmd(0xE8);
//spi_write_data(0x40);
//spi_write_data(0x00);
//spi_write_cmd(0xFF);
//spi_write_data(0x77);
//spi_write_data(0x01);
//spi_write_data(0x00);
//spi_write_data(0x00);
//spi_write_data(0x00);
//spi_write_cmd(0x29);
//spi_write_cmd(0x36);
//spi_write_data(0x00);
//spi_write_cmd(0x35);
//spi_write_data(0x00);




//spi_write_cmd(0x3a);
//spi_write_data(0x66);

//spi_write_cmd(0x35);
//spi_write_data(0x00);//TE ON

//spi_write_cmd(0x36);
//spi_write_data(0x00);//写00正常扫描方向     写10   反向扫描

////spi_write_cmd(0x11);
////Delay(150);

////spi_write_cmd(0x29);
////Delay(20);


//spi_write_cmd(0x29);     //Display on

//swm_delay_ms(20); 
  spi_write_cmd (0xFF);
spi_write_data (0x77);
spi_write_data (0x01);
spi_write_data (0x00);
spi_write_data (0x00);
spi_write_data (0x13);
  spi_write_cmd (0xEF);
spi_write_data (0x08);
  spi_write_cmd (0xFF);
spi_write_data (0x77);
spi_write_data (0x01);
spi_write_data (0x00);
spi_write_data (0x00);
spi_write_data (0x10);
  spi_write_cmd (0xC0);
spi_write_data (0x63);
spi_write_data (0x00);
  spi_write_cmd (0xC1);
spi_write_data (0x0D);
spi_write_data (0x0D);
  spi_write_cmd (0xC2);
spi_write_data (0x07);
spi_write_data (0x03);
  spi_write_cmd (0xB0);
spi_write_data (0x00);
spi_write_data (0x0B);
spi_write_data (0x12);
spi_write_data (0x0E);
spi_write_data (0x11);
spi_write_data (0x06);
spi_write_data (0x02);
spi_write_data (0x08);
spi_write_data (0x08);
spi_write_data (0x1F);
spi_write_data (0x06);
spi_write_data (0x14);
spi_write_data (0x12);
spi_write_data (0x29);
spi_write_data (0x31);
spi_write_data (0x1F);
  spi_write_cmd (0xB1);
spi_write_data (0x00);
spi_write_data (0x0C);
spi_write_data (0x13);
spi_write_data (0x0C);
spi_write_data (0x10);
spi_write_data (0x06);
spi_write_data (0x01);
spi_write_data (0x07);
spi_write_data (0x06);
spi_write_data (0x1F);
spi_write_data (0x03);
spi_write_data (0x12);
spi_write_data (0x10);
spi_write_data (0x29);
spi_write_data (0x32);
spi_write_data (0x1F);
  spi_write_cmd (0x36);
spi_write_data (0x00);
spi_write_data (0x00);
  spi_write_cmd (0xFF);
spi_write_data (0x77);
spi_write_data (0x01);
spi_write_data (0x00);
spi_write_data (0x00);
spi_write_data (0x11);
  spi_write_cmd (0xB0);
spi_write_data (0x65);
  spi_write_cmd (0xB1);
spi_write_data (0x5A);
  spi_write_cmd (0xB2);
spi_write_data (0x87);
  spi_write_cmd (0xB3);
spi_write_data (0x80);
  spi_write_cmd (0xB5);
spi_write_data (0x46);
  spi_write_cmd (0xB7);
spi_write_data (0x85);
  spi_write_cmd (0xB8);
spi_write_data (0x31);
  spi_write_cmd (0xC1);
spi_write_data (0x78);
  spi_write_cmd (0xC2);
spi_write_data (0x78);

  spi_write_cmd (0xE0);
spi_write_data (0x00);
spi_write_data (0x29);
spi_write_data (0x02);
  spi_write_cmd (0xE1);
spi_write_data (0x08);
spi_write_data (0xAE);
spi_write_data (0x00);
spi_write_data (0x00);
spi_write_data (0x07);
spi_write_data (0xAE);
spi_write_data (0x00);
spi_write_data (0x00);
spi_write_data (0x00);
spi_write_data (0x43);
spi_write_data (0x43);
  spi_write_cmd (0xE2);
spi_write_data (0x30);
spi_write_data (0x30);
spi_write_data (0x40);
spi_write_data (0x40);
spi_write_data (0x2D);
spi_write_data (0xAE);
spi_write_data (0x00);
spi_write_data (0x00);
spi_write_data (0x2C);
spi_write_data (0xAE);
spi_write_data (0x00);
spi_write_data (0x00);
spi_write_data (0x00);
  spi_write_cmd (0xE3);
spi_write_data (0x00);
spi_write_data (0x00);
spi_write_data (0x33);
spi_write_data (0x33);
  spi_write_cmd (0xE4);
spi_write_data (0x44);
spi_write_data (0x44);
  spi_write_cmd (0xE5);
spi_write_data (0x0A);
spi_write_data (0x30);
spi_write_data (0x0E);
spi_write_data (0xAE);
spi_write_data (0x0C);
spi_write_data (0x32);
spi_write_data (0x0E);
spi_write_data (0xAE);
spi_write_data (0x0E);
spi_write_data (0x34);
spi_write_data (0x0E);
spi_write_data (0xAE);
spi_write_data (0x10);
spi_write_data (0x36);
spi_write_data (0x0E);
spi_write_data (0xAE);
  spi_write_cmd (0xE6);
spi_write_data (0x00);
spi_write_data (0x00);
spi_write_data (0x33);
spi_write_data (0x33);
  spi_write_cmd (0xE7);
spi_write_data (0x44);
spi_write_data (0x44);
  spi_write_cmd (0xE8);
spi_write_data (0x09);
spi_write_data (0x2F);
spi_write_data (0x0E);
spi_write_data (0xAE);
spi_write_data (0x0B);
spi_write_data (0x31);
spi_write_data (0x0E);
spi_write_data (0xAE);
spi_write_data (0x0D);
spi_write_data (0x33);
spi_write_data (0x0E);
spi_write_data (0xAE);
spi_write_data (0x0F);
spi_write_data (0x35);
spi_write_data (0x0E);
spi_write_data (0xAE);
  spi_write_cmd (0xEB);
spi_write_data (0x00);
spi_write_data (0x01);
spi_write_data (0xE4);
spi_write_data (0xE4);
spi_write_data (0x99);
spi_write_data (0x33);
spi_write_data (0x40);  
  spi_write_cmd (0xEC);
spi_write_data (0x3D);
spi_write_data (0x00); 
  spi_write_cmd (0xED);
spi_write_data (0x20);
spi_write_data (0x76);
spi_write_data (0x54);
spi_write_data (0x89);
spi_write_data (0xBA);
spi_write_data (0xFF);
spi_write_data (0xFF);
spi_write_data (0xFF);
spi_write_data (0xFF);
spi_write_data (0xFF);
spi_write_data (0xFF);
spi_write_data (0xAB);
spi_write_data (0x98);
spi_write_data (0x45);
spi_write_data (0x67);
spi_write_data (0x02); 
  spi_write_cmd (0x3A);
spi_write_data (0x66);

  spi_write_cmd (0x36);
spi_write_data (0x00);
  spi_write_cmd (0x11);
swm_delay_ms(120);
  spi_write_cmd (0x29);
swm_delay_ms(20);
  spi_write_cmd (0x35);
spi_write_data (0x00);
#endif
	



 #if 0
 
 //自己买的960*320屏幕初始化	
spi_write_cmd( 0xFF);     spi_write_data(0x77); spi_write_data(0x01);   
spi_write_data(0x00);   
spi_write_data(0x00);   
spi_write_data(0x13);   

spi_write_cmd( 0xEF);     
spi_write_data(0x08);   

spi_write_cmd( 0xFF);     
spi_write_data(0x77);   
spi_write_data(0x01);   
spi_write_data(0x00);   
spi_write_data(0x00);   
spi_write_data(0x10);   

spi_write_cmd( 0xC0);     
spi_write_data(0x77);   
spi_write_data(0x00);   

spi_write_cmd( 0xC1);     
spi_write_data(0x09);   
spi_write_data(0x08);   

spi_write_cmd( 0xC2);//inv     
spi_write_data(0x01);   
spi_write_data(0x02);  

spi_write_cmd( 0xC3); //极性设置    
spi_write_data(0x02); //82 HVmode    02 DEmode 
 

spi_write_cmd( 0xCC);     
spi_write_data(0x10);   

spi_write_cmd( 0xB0);     
spi_write_data(0x40);   
spi_write_data(0x14);   
spi_write_data(0x59);   
spi_write_data(0x10);   
spi_write_data(0x12);   
spi_write_data(0x08);   
spi_write_data(0x03);   
spi_write_data(0x09);   
spi_write_data(0x05);   
spi_write_data(0x1E);   
spi_write_data(0x05);   
spi_write_data(0x14);   
spi_write_data(0x10);   
spi_write_data(0x68);   
spi_write_data(0x33);   
spi_write_data(0x15);   

spi_write_cmd( 0xB1);     
spi_write_data(0x40);   
spi_write_data(0x08);   
spi_write_data(0x53);   
spi_write_data(0x09);   
spi_write_data(0x11);   
spi_write_data(0x09);   
spi_write_data(0x02);   
spi_write_data(0x07);   
spi_write_data(0x09);   
spi_write_data(0x1A);   
spi_write_data(0x04);   
spi_write_data(0x12);   
spi_write_data(0x12);   
spi_write_data(0x64);   
spi_write_data(0x29);   
spi_write_data(0x29);   

spi_write_cmd( 0xFF);     
spi_write_data(0x77);   
spi_write_data(0x01);   
spi_write_data(0x00);   
spi_write_data(0x00);   
spi_write_data(0x11);   

spi_write_cmd( 0xB0);     
spi_write_data(0x6D);   

spi_write_cmd( 0xB1);   //vcom  
spi_write_data(0x1D);   

spi_write_cmd( 0xB2);     
spi_write_data(0x87);   

spi_write_cmd( 0xB3);     
spi_write_data(0x80);   

spi_write_cmd( 0xB5);     
spi_write_data(0x49);   

spi_write_cmd( 0xB7);     
spi_write_data(0x85);   

spi_write_cmd( 0xB8);     
spi_write_data(0x20);   

spi_write_cmd( 0xC1);     
spi_write_data(0x78);   

spi_write_cmd( 0xC2);     
spi_write_data(0x78);   

spi_write_cmd( 0xD0);     
spi_write_data(0x88);   

spi_write_cmd( 0xE0);     
spi_write_data(0x00);   
spi_write_data(0x00);   
spi_write_data(0x02);   

spi_write_cmd( 0xE1);     
spi_write_data(0x02);   
spi_write_data(0x8C);   
spi_write_data(0x00);   
spi_write_data(0x00);   
spi_write_data(0x03);   
spi_write_data(0x8C);   
spi_write_data(0x00);   
spi_write_data(0x00);   
spi_write_data(0x00);   
spi_write_data(0x33);   
spi_write_data(0x33);   

spi_write_cmd( 0xE2);     
spi_write_data(0x33);   
spi_write_data(0x33);   
spi_write_data(0x33);   
spi_write_data(0x33);   
spi_write_data(0xC9);   
spi_write_data(0x3C);   
spi_write_data(0x00);   
spi_write_data(0x00);   
spi_write_data(0xCA);   
spi_write_data(0x3C);   
spi_write_data(0x00);   
spi_write_data(0x00);   
spi_write_data(0x00);   

spi_write_cmd( 0xE3);     
spi_write_data(0x00);   
spi_write_data(0x00);   
spi_write_data(0x33);   
spi_write_data(0x33);   

spi_write_cmd( 0xE4);     
spi_write_data(0x44);   
spi_write_data(0x44);   

spi_write_cmd( 0xE5);     
spi_write_data(0x05);   
spi_write_data(0xCD);   
spi_write_data(0x82);   
spi_write_data(0x82);   
spi_write_data(0x01);   
spi_write_data(0xC9);   
spi_write_data(0x82);   
spi_write_data(0x82);   
spi_write_data(0x07);   
spi_write_data(0xCF);   
spi_write_data(0x82);   
spi_write_data(0x82);   
spi_write_data(0x03);   
spi_write_data(0xCB);   
spi_write_data(0x82);   
spi_write_data(0x82);   

spi_write_cmd( 0xE6);     
spi_write_data(0x00);   
spi_write_data(0x00);   
spi_write_data(0x33);   
spi_write_data(0x33);   

spi_write_cmd( 0xE7);     
spi_write_data(0x44);   
spi_write_data(0x44);   

spi_write_cmd( 0xE8);     
spi_write_data(0x06);   
spi_write_data(0xCE);   
spi_write_data(0x82);   
spi_write_data(0x82);   
spi_write_data(0x02);   
spi_write_data(0xCA);   
spi_write_data(0x82);   
spi_write_data(0x82);   
spi_write_data(0x08);   
spi_write_data(0xD0);   
spi_write_data(0x82);   
spi_write_data(0x82);   
spi_write_data(0x04);   
spi_write_data(0xCC);   
spi_write_data(0x82);   
spi_write_data(0x82);   

spi_write_cmd( 0xEB);     
spi_write_data(0x08);   
spi_write_data(0x01);   
spi_write_data(0xE4);   
spi_write_data(0xE4);   
spi_write_data(0x88);   
spi_write_data(0x00);   
spi_write_data(0x40);   

spi_write_cmd( 0xEC);     
spi_write_data(0x00);   
spi_write_data(0x00);   
spi_write_data(0x00);   

spi_write_cmd( 0xED);     
spi_write_data(0xFF);   
spi_write_data(0xF0);   
spi_write_data(0x07);   
spi_write_data(0x65);   
spi_write_data(0x4F);   
spi_write_data(0xFC);   
spi_write_data(0xC2);   
spi_write_data(0x2F);   
spi_write_data(0xF2);   
spi_write_data(0x2C);   
spi_write_data(0xCF);   
spi_write_data(0xF4);   
spi_write_data(0x56);   
spi_write_data(0x70);   
spi_write_data(0x0F);   
spi_write_data(0xFF);   

spi_write_cmd( 0xEF);     
spi_write_data(0x10);   
spi_write_data(0x0D);   
spi_write_data(0x04);   
spi_write_data(0x08);   
spi_write_data(0x3F);   
spi_write_data(0x1F);   

spi_write_cmd( 0xFF);     
spi_write_data(0x77);   
spi_write_data(0x01);   
spi_write_data(0x00);   
spi_write_data(0x00);   
spi_write_data(0x00);   

spi_write_cmd( 0x11);     

swm_delay_ms(120);                

spi_write_cmd( 0x35);     
spi_write_data(0x00);   

spi_write_cmd( 0x3A);     
spi_write_data(0x66);   

spi_write_cmd( 0x29);
#endif


	swm_delay_ms(1);

	spi_write_cmd(0x11);
	spi_write_cmd(0x29);

	swm_delay_ms(10);
}
