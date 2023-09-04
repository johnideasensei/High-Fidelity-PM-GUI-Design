#include "main.h"
#include "lvgl/examples/porting/lv_port_disp.h"
#include "lvgl/examples/porting/lv_port_indev.h"
//#include "lvgl/examples/porting/lv_port_fs.h"

#include "lvgl/lvgl.h"
//#include "lvgl/demos/lv_demos.h"
#include "app_test.h"
#include "encoder.h"
#include "9039M.h"
#include "CS8422.h"
#include "soft_i2c.h"
#include "remote.h"
void dac_gpio_init(void);
//void TurnOffUSBPower(void);
void adc_init(void);
uint32_t WrBuff[20] = {22};//存入flash的dB值

extern uint32_t flag,soundBflag,flashflag,flashflaginsel,flashflaglvbo,flashflagpcm,flashflagbl,rm;
extern lv_obj_t *mainlba,*mainlb4,*menulb8,*menulb,*w8switch1,*w8switch2,*w3switch2,*lb1,*mainfu,*mainshi,*mainge,*mainmute;
extern int soundB,soundBRtemp,soundBLtemp;
extern uint16_t coul,cour;
extern uint8_t backlight,sleepflag,sleeptime,muteon,mutetime;//休眠旗帜
extern uint8_t idflag,levelflag,outgpio;

extern uint8_t valuetemp,debug_read,value;
uint8_t numcount=0,ayflag=0,ayflagtemp=0,numflag=0,muteflag=0;
uint16_t lsp=0,rsp=0,enlock=0;//旋钮锁
uint32_t i=0,chn,blflag=0,blflagt=0;
int start=1;//复位标志
int adcv=0;
int cnt_mute_power=0;//长时间不放歌就关机计数器
int cnt_5=0;//长时间不放歌就关机计数器使用，5次就是1秒
uint8_t music_on = 0;

static int music_singal_detected(void){//1为检测到音乐信号 0为未检测到信号
	int check = 0;
	read_9039m_regs(0XF2,&music_on);
	if(music_on == 0xFF){
		check = 0; 
	} 
	else if(music_on == 0x00){
		check = 1;
	}
	return check;
}

int main(void)
{

    	SystemInit();
    	systick_init();
//    uart_init();
//		ENCODER_INIT();//初始化旋转编码器IO的相关口
	
		GPIO_Init(GPIOM, PIN3, 1, 1, 0, 0);//SLEEP/POWER引脚初始化
		GPIO_Init(GPION, PIN0, 1, 1, 0, 0);//静音引脚初始化
		GPIO_Init(GPIOM, PIN5, 1, 1, 0, 0);//静音引脚2初始化
		GPIO_Init(GPIOM, PIN2, 1, 1, 0, 0);//脉冲引脚初始化
	
		GPIO_Init(GPIOB, PIN4, 1, 1, 0, 0);
		GPIO_SetBit(GPIOB, PIN4);
	
		swm_delay_ms(100);	
		GPIO_ClrBit(GPION, PIN0);//物理静音  1是开启声音 GPIO_SetBit  0是声音关 GPIO_ClrBit
		GPIO_ClrBit(GPIOM, PIN5);//物理静音2  1是开启声音 GPIO_SetBit  0是声音关 GPIO_ClrBit
		GPIO_SetBit(GPIOM, PIN3);//电源光耦    1是通电 GPIO_SetBit	     0是不通电 GPIO_ClrBit
		swm_delay_ms(100);	
		remote_init();
		lv_init();
		lv_port_disp_init();
		lv_port_indev_init();
	
		IIC_Init();
		sam_fre_init();//初始化fgpa口
		dac_gpio_init();//CS8422/ES9039

		swm_delay_ms(20);
		cs8422_init();
		es9039_init();
		
		
		adc_init();
	//	TurnOffUSBPower();
		my_gui();

		TIMR_Init(TIMR1, TIMR_MODE_TIMER, CyclesPerUs, 5000, 1);
		
		TIMR_Init(BTIMR4, TIMR_MODE_TIMER, CyclesPerUs, 200000, 1);	//每1秒钟触发一次中断 1000000
		TIMR_Start(BTIMR4);
		
		TIMR_Init(TIMR2, TIMR_MODE_TIMER, CyclesPerUs, 160000, 1);	//实时刷新定时器
	  	TIMR_Start(TIMR2);
		
//		GPIO_SetBit(GPION, PIN0);//物理静音  1是开启声音 GPIO_SetBit  0是声音关 GPIO_ClrBit
//		GPIO_SetBit(GPIOM, PIN5);//物理静音2  1是开启声音 GPIO_SetBit  0是声音关 GPIO_ClrBit
//	write_9039m_regs(0x56, 0x01);//静音通道--0位对应1通道   写1为静音2	
			
//		FLASH_Write(EEPROM_ADDR, WrBuff, 4);
//	lv_label_set_text_fmt(mainlb2, "#FF0000 %d",*((volatile uint32_t *)(EEPROM_ADDR + 0*4)));//设置旋转显示
//	FLASH_Erase(EEPROM_ADDR);
//	WrBuff[0]=4;
//	FLASH_Write(EEPROM_ADDR, WrBuff, 4);
//	lv_label_set_text_fmt(mainlba, "%d",*((volatile uint32_t *)(EEPROM_ADDR + 0*4)));//设置旋转显示

//FLASH_Erase(EEPROM_ADDR);
//	WrBuff[0]=6;
//	FLASH_Write(EEPROM_ADDR, WrBuff, 4);
//	lv_label_set_text_fmt(mainlb3, "%d",*((volatile uint32_t *)(EEPROM_ADDR + 0*4)));//设置旋转显示
  swm_delay_ms(50);
	/* XMOS脉冲 */
   for(i=0;i<10;i++){
		GPIO_SetBit(GPIOM, PIN2);
		 swm_delay_ms(10);
	 	GPIO_ClrBit(GPIOM, PIN2);
		 swm_delay_ms(10);
		GPIO_SetBit(GPIOM, PIN2);
		 swm_delay_ms(10);
	 }
	 

    while (1)
    {

    if(adcv<=3200&&adcv>100){

    	GPIO_ClrBit(GPION, PIN0);//物理静音  1是开启声音 GPIO_SetBit  0是声音关 GPIO_ClrBit
		GPIO_ClrBit(GPIOM, PIN5);//物理静音2  1是开启声音 GPIO_SetBit  0是声音关 GPIO_ClrBit

    }
			
   // lv_label_set_text_fmt(mainlba,"%.2fv", adcv/4096.0*3.3);
   
   
//i=check_wkup();
//if(i==1)printf( "1\n");
//else if(i==2)printf( "2\n");
//else if(i==4)printf( "4\n");
//  i=ENCODER_READ();
//   if(i==3)printf( "1\n");
//   if(i==4)printf( "2\n");
//   if(i==5)printf( "4\n");
			
	if(levelflag==1){//旋转db调节音量大小标志
		ENCODER_DBLOOP();
		TIMR_Start(TIMR1);
   }else {
	 	TIMR_Stop(TIMR1);
	 }
	if(flag){//编辑模式
		ENCODER_LOOP();//扫描旋转编码器
   }
	//模拟休眠模式
	if(sleeptime){
	if(sleepflag){
		write_9039m_regs(0x56, 0xFF);//静音所有通道
		PWM_SetHDuty(PWM1, PWM_CH_B,0);//息屏
		enlock=1;//旋钮锁死
		swm_delay_ms(100);
		GPIO_ClrBit(GPION, PIN0);//物理静音  1是开启声音 GPIO_SetBit  0是声音关 GPIO_ClrBit
		GPIO_ClrBit(GPIOM, PIN5);//物理静音2  1是开启声音 GPIO_SetBit  0是声音关 GPIO_ClrBit
//				swm_delay_ms(100);
//				GPIO_ClrBit(GPIOM, PIN6);//引脚RESET 高电平正常工作
//				GPIO_ClrBit(GPIOD, PIN15);//引脚RESET 高电平正常工作
		swm_delay_ms(200);
		GPIO_ClrBit(GPIOM, PIN3);//电源光耦    1是通电 GPIO_SetBit	     0是不通电 GPIO_ClrBit
		sleeptime=0;//只做一次
		}
			//模拟唤醒模式
			else if(!sleepflag){
				GPIO_ClrBit(GPIOB, PIN4);//复位单片机
					
//				GPIO_SetBit(GPIOM, PIN3);//电源光耦    1是通电 GPIO_SetBit	     0是不通电 GPIO_ClrBit
////				GPIO_ClrBit(GPION, PIN0);
////				GPIO_ClrBit(GPIOM, PIN5);
//				swm_delay_ms(500);
//				cs8422_init();
//				es9039_init();
//				start_recovery();//重启运行,恢复数据
//				PWM_SetHDuty(PWM1, PWM_CH_B,WrBuff[5]*100*10);//亮屏
//				write_9039m_regs(0x56, 0x00);//静音所有通道
//				GPIO_SetBit(GPION, PIN0);//物理静音  1是开启声音 GPIO_SetBit  0是声音关 GPIO_ClrBit				
//				GPIO_SetBit(GPIOM, PIN5);//物理静音2  1是开启声音 GPIO_SetBit  0是声音关 GPIO_ClrBit	
//				
//			/* XMOS脉冲 */
//   for(i=0;i<10;i++){
//		GPIO_SetBit(GPIOM, PIN2);
//		 swm_delay_ms(2);
//	 	GPIO_ClrBit(GPIOM, PIN2);
//		 swm_delay_ms(2);
//		GPIO_SetBit(GPIOM, PIN2);
//		 swm_delay_ms(2);
//	 }
//			enlock=0;//旋钮解锁
//			sleeptime=0;//只做一次
			}
		}
		//静音模式
  if(mutetime){
			outgpio=2*GPIO_GetBit(GPIOM, PIN5)+GPIO_GetBit(GPION, PIN0);
		 if(outgpio==0)WrBuff[12]=3;//0 0
		 else if(outgpio==1)WrBuff[12]=2;//0 1
		 else if(outgpio==2)WrBuff[12]=1;//1 0
		 else if(outgpio==3)WrBuff[12]=0;//1 1
		
	if(muteon){
		if(levelflag==1){
		lv_obj_set_style_img_opa(mainmute,LV_OPA_100,0);
		lv_obj_set_style_img_opa(mainfu,LV_OPA_0,0);
		lv_obj_set_style_img_opa(mainge,LV_OPA_0,0);
		lv_obj_set_style_img_opa(mainshi,LV_OPA_0,0);
		lv_obj_set_style_text_opa(lb1,LV_OPA_0,0);
		}
		
		//write_9039m_regs(0x56, 0xFF);//静音所有通道
		if(WrBuff[12]==3)muteflag=0;//0 0
		 else if(WrBuff[12]==2){muteflag=1;WrBuff[12]=3;}//0 1
		 else if(WrBuff[12]==1){muteflag=2;WrBuff[12]=3;}//1 0
		 else if(WrBuff[12]==0){muteflag=3;WrBuff[12]=3;}//1 1
		 
		 GPIO_ClrBit(GPIOM, PIN5);GPIO_ClrBit(GPION, PIN0);
		 			
	}else{//解除静音
		
		if(levelflag==1){
		lv_obj_set_style_img_opa(mainmute,LV_OPA_0,0);
		lv_obj_set_style_img_opa(mainfu,LV_OPA_100,0);
		lv_obj_set_style_img_opa(mainge,LV_OPA_100,0);
		lv_obj_set_style_img_opa(mainshi,LV_OPA_100,0);
		lv_obj_set_style_text_opa(lb1,LV_OPA_100,0);
		}
		
		//write_9039m_regs(0x56, 0x00);//静音所有通道
		if(muteflag==1){WrBuff[12]=2;GPIO_ClrBit(GPIOM, PIN5);GPIO_SetBit(GPION, PIN0);}
		else if(muteflag==2){WrBuff[12]=1;GPIO_SetBit(GPIOM, PIN5);GPIO_ClrBit(GPION, PIN0);}
		else if(muteflag==3){WrBuff[12]=0;GPIO_SetBit(GPIOM, PIN5);GPIO_SetBit(GPION, PIN0);}
		else if(muteflag==0){WrBuff[12]=3;GPIO_ClrBit(GPIOM, PIN5);GPIO_ClrBit(GPION, PIN0);}	
	}

	mutetime=0;
	}
	
	
      lv_timer_handler();

    }
}


void BTIMR4_Handler(void)
{
	bool musicSignalDetected = 0;
	TIMR_INTClr(BTIMR4);
		//lv_label_set_text_fmt(mainlba, "%d", i++);
	//没有播放音乐一段时间以后自动关机计数器
	if(cnt_5 >= 0 && cnt_5 < 5)
	{
		cnt_5++;
	}
	if(cnt_mute_power>=0 && cnt_5 == 5)
	{
	//				read_9039m_regs(0XF2,&tiaoshibuf);
		// lv_label_set_text_fmt(tiaoshi5566, "%d",cnt_mute_power);
		cnt_mute_power += 1;
		musicSignalDetected = music_singal_detected();
		
		if (musicSignalDetected) {//检测musicSignalDetected,1为有音乐信号，0为无音乐信号
			cnt_mute_power = 0;
		}
		// 如果计数器达到10分钟的计数值（10分钟 = 600 秒 / 1.0 秒 = 600），进入休眠
		else if (musicSignalDetected == 0 && cnt_mute_power >= 600) {
			sleeptime = 1;
			sleepflag = 1;
		}
		cnt_5=0;	
	}
//lv_label_set_text_fmt(mainlba, "%d", i++);
	if(flashflag){
		FLASH_Erase(EEPROM_ADDR);
		WrBuff[0]=soundB;
		WrBuff[6]=soundBLtemp;
		WrBuff[7]=soundBRtemp;
		FLASH_Write(EEPROM_ADDR, WrBuff, 20);
		//lv_label_set_text_fmt(mainlb2, "#FF0000 -%.1f#",(float)(soundB)/2);
	  //lv_label_set_text_fmt(mainlb2, "#FF0000 %d",*((volatile uint32_t *)(EEPROM_ADDR + 0*4)));//设置旋转显示
		flashflag=0;
	}
if(flashflaginsel){
	FLASH_Erase(EEPROM_ADDR);
	FLASH_Write(EEPROM_ADDR, WrBuff, 20);
//	lv_label_set_text_fmt(mainlb3, "#FF0000 %d",*((volatile uint32_t *)(EEPROM_ADDR + 1*4)));//设置旋转显示
//	lv_label_set_text_fmt(mainlb4, "#FF0000 %d",*((volatile uint32_t *)(EEPROM_ADDR + 2*4)));//设置旋转显示
	flashflaginsel=0;
}
if(flashflaglvbo){
	FLASH_Erase(EEPROM_ADDR);
	FLASH_Write(EEPROM_ADDR, WrBuff, 20);
	flashflaglvbo=0;
}
if(flashflagbl){
	WrBuff[5]=backlight;
	FLASH_Erase(EEPROM_ADDR);
	FLASH_Write(EEPROM_ADDR, WrBuff, 20);
	flashflagbl=0;
}
if(WrBuff[14]==1){
if(ENCODER_SW!=0&&rm==255)blflag++;
else blflag=0;
if(blflag>=WrBuff[15]*5){
	blflagt=1;//暗屏标志
	enlock=1;//旋钮锁死
	PWM_SetHDuty(PWM1, PWM_CH_B,WrBuff[16]*100*10);//设置PWM电平时间
}}

}

void TIMR1_Handler(void)
{
	TIMR_INTClr(TIMR1);
		//Adc测试
			 ADC_Start(ADC0, ADC_SEQ0);
			if((ADC0->SEQ[0].SR & ADC_SR_EOC_Msk) == 1){
			 adcv=ADC_Read(ADC0, ADC_SEQ0, &chn);}
			
			numflag++;
	 if(numflag>100){//100*10ms
		 if(cour>2)rsp=1;
		 else rsp=0;
		 if(coul>2)lsp=1;
		 else lsp=0;
		 cour=coul=0;
	 numflag=0;}
}

void TIMR2_Handler(void)
{
	TIMR_INTClr(TIMR2);
	
			ayflagtemp=GETF2*4+GETF3*2+GETF4*1;
			if(ayflag!=ayflagtemp){
			ayflag=ayflagtemp;
			read_9039m_regs(0x01,&numcount);
			numcount &=(~0x40);//把0-2Bit位清0，其他位不变
			if(ayflag==4){
			numcount |=0x40; 
			//WrBuff[8]=1;
			write_9039m_regs(0x00, 0x42);
			}
			else {
			numcount |=0;
			//WrBuff[8]=0;
			if(WrBuff[8])write_9039m_regs(0x00, 0x42);
			else write_9039m_regs(0x00, 0x02);
			}
			write_9039m_regs(0x01, numcount);
			}
	
	if(levelflag==1){
//		read_9039m_regs(FIR_Shape_Set,&debug_read);//先读出寄存器值	
//			lv_label_set_text_fmt(lb1, "0X%x",debug_read);
		sam_fre_chose();//PCM/DSD显示
	}
		
		/* 界面数据同步 */
	else if(levelflag==2)
	{
		if(WrBuff[12]==0){lv_label_set_text_fmt(menulb8, "%s","全开");}
		else if(WrBuff[12]==1){lv_label_set_text_fmt(menulb8, "%s","耳放");}
		else if(WrBuff[12]==2){lv_label_set_text_fmt(menulb8, "%s","RCA-XLR");}
		else if(WrBuff[12]==3){lv_label_set_text_fmt(menulb8, "%s","全关");}
		lv_obj_align_to(menulb8,menulb, LV_ALIGN_RIGHT_MID,15,0);
	}				
	else if(levelflag==3&&idflag==3)//未改动前输出选择标志位 idflag==8
	{
		if(WrBuff[12]==0){lv_obj_add_state(w8switch1, LV_STATE_CHECKED);lv_obj_add_state(w8switch2, LV_STATE_CHECKED);}
		else if(WrBuff[12]==1){lv_obj_add_state(w8switch1, LV_STATE_CHECKED);lv_obj_clear_state(w8switch2, LV_STATE_CHECKED);}
		else if(WrBuff[12]==2){lv_obj_clear_state(w8switch1, LV_STATE_CHECKED);lv_obj_add_state(w8switch2, LV_STATE_CHECKED);}
		else if(WrBuff[12]==3){lv_obj_clear_state(w8switch1, LV_STATE_CHECKED);lv_obj_clear_state(w8switch2, LV_STATE_CHECKED);}
	}	
	else if(levelflag==3&&idflag==5)//未改动前9030MPRO标志位 idflag==3
	{
		if(WrBuff[8]==0)lv_obj_add_state(w3switch2, LV_STATE_CHECKED);
		else lv_obj_clear_state(w3switch2, LV_STATE_CHECKED);
	}

}



//8422和9039复位引脚初始化-再拉低
void dac_gpio_init(void){
	GPIO_Init(GPIOM, PIN6, 1, 0, 1, 0);			//输出，下拉使能 RESET M6 CS8416-PIN9
	GPIO_Init(GPIOD, PIN15, 1, 0, 1, 0);			//输出，下拉使能 RESET D15 ES9039-PIN8	
	GPIO_ClrBit(GPIOM, PIN6);//引脚RESET 高电平正常工作
	GPIO_ClrBit(GPIOD, PIN15);//引脚RESET 高电平正常工作
	
}

void adc_init(void)//ADC采集初始化
{
	ADC_InitStructure ADC_initStruct;
	ADC_SEQ_InitStructure ADC_SEQ_initStruct;
	
	PORT_Init(PORTC, PIN5,  PORTC_PIN5_ADC0_CH1,   0);	//PC.5  => ADC0.CH1

	ADC_initStruct.clk_src = ADC_CLKSRC_HRC_DIV8;
	ADC_initStruct.samplAvg = ADC_AVG_SAMPLE1;
	ADC_initStruct.EOC_IEn = 0;	
	ADC_initStruct.HalfIEn = 0;
	ADC_Init(ADC0, &ADC_initStruct);					//配置ADC
	
	ADC_SEQ_initStruct.channels = ADC_CH1;
	ADC_SEQ_initStruct.trig_src = ADC_TRIGGER_SW;
	ADC_SEQ_initStruct.conv_cnt = 1;
	ADC_SEQ_initStruct.samp_tim = ADC_SAMPLE_1CLOCK;
	ADC_SEQ_Init(ADC0, ADC_SEQ0, &ADC_SEQ_initStruct);
	
	ADC_Open(ADC0);										//使能ADC
 	ADC_Calibrate(ADC0);								//校准ADC
	

}

void SysTick_Handler_cb(void)
{
    lv_tick_inc(1);
}

void SysTick_Handler(void)
{
    swm_inctick();
    SysTick_Handler_cb();
}





//void TurnOffUSBPower(void)
//{
//	SYS->USBCR |= 0x07;
//	
//	SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_USB_Pos);
//	USBD->DEVCR = (USBD_SPEED_FS << USBD_DEVCR_SPEED_Pos);
//	
//	SYS->USBPHYCR &= ~SYS_USBPHYCR_PLLEN_Msk;
//}

	