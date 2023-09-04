#ifndef __LCD_TL040WVS03_H__
#define __LCD_TL040WVS03_H__

//----------------------------ͷ�ļ�����----------------------------//
//#include "dev_lcd.h"
#include "SWM341.h"

//#include "version.h"


#define LCD_GPIO_RST 				GPIOD
#define LCD_PIN_RST 				PIN1

#define LCD_GPIO_BL 				GPIOD
#define LCD_PIN_BL 					PIN9

#define LCD_PORT_BL 				PORTD
#define LCD_PORT_FUNC_BL			PORTD_PIN9_PWM1B
#define LCD_PORT_PWMN_BL			PWM1
#define LCD_PORT_PWMN_MSK_BL		PWM1_MSK







//----------------------------��������----------------------------//
void lcd_spi_init_TL040WVS03(void);

#endif 



