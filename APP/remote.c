#include "remote.h"
#include "main.h"
#include "lvgl/lvgl.h"
#include "9039M.h"
#include "app_test.h"
#include "encoder.h"
#include "CS8422.h"
extern lv_obj_t *mainlb2,*w1lbx1,*w1lbx2,*w1lbx3,*mainsli,*mainlb1,*mainoj1,*mainlb4,*mainlb5,*mainlb3,*menulb1,*menulb2,*menulb5,*blimg
	,*lb1,*scr,*mainoj,*w1bt1,*w1bt2,*w1bt3,*w1bt4,*alignoj1,*alignoj2;//
extern lv_obj_t *oj1,*oj2,*oj3,*oj4,*oj5,*oj6,*oj7,*oj8,*wbt,*ojenter1img,*ojenter2img,*ojenter3img,*ojenter4img,*ojw1,*ojw2,*ojw3,*ojw4,*ojw5,*ojw6,*ojw7,*ojw8;
extern lv_obj_t *w2bt1,*w2bt2,*w2bt3,*w3bt1,*w3bt2,*w3bt3,*w3bt4,*w3bt5,*w4bt1,*w5bt1,*w5lb,*w6bt1,*w6bt2,*w7bt1,*w8bt1,*w8bt2;
extern lv_obj_t *menulb8,*menulb,*w8switch1,*w8switch2,*mainojimg,*mainfu;
extern uint8_t flag,soundBflag,sliflag; //标志位
/* 值 */
extern uint8_t backlight,FIR_value; //值
extern int soundB,soundBtemp,soundBRtemp,soundBLtemp,lvbosel;
extern int soundBR,soundBL;
extern uint16_t lsp,rsp;//加速标志位
uint8_t sleepon=0,muteon=0,mutetime=0,mutef=1;
extern uint16_t enlock;//旋钮锁
extern uint32_t WrBuff[20],flashflagbl,flashflag,flashflaglvbo,flashflaginsel,blflag,blflagt,nextlock;
uint32_t Remote_Odr=0;    //命令暂存处		 	
uint8_t Remote_Cnt=0;    //按键次数,此次按下键的次数
uint8_t  Remote_Rdy=0;    //红外接收到数据 
uint8_t  remote_key;      //解码后得到的按键码值
extern uint8_t idflag,levelflag;
extern lv_group_t *g,*gmain;
LV_IMG_DECLARE(BL0)
LV_IMG_DECLARE(BL1)
LV_IMG_DECLARE(BL2)
LV_IMG_DECLARE(BL3)
LV_IMG_DECLARE(BL4)
LV_IMG_DECLARE(BL5)
LV_IMG_DECLARE(BL6)
LV_IMG_DECLARE(BL7)
LV_IMG_DECLARE(BL8)

LV_IMG_DECLARE(USB)
LV_IMG_DECLARE(HDMI)
LV_IMG_DECLARE(TOS)
LV_IMG_DECLARE(COA)
#define tiaoshi 0
/*******************红外********************************/
void remote_init(void)//初始化红外遥控IO的相关口
{
	
	GPIO_Init(GPION, PIN1, 0, 0, 1, 0);			//初始化红外接收头IO口，输入，下拉使能，接KEY	
	
	EXTI_Init(GPION, PIN1, EXTI_RISE_EDGE);		//IO口上升沿触发中断	
	NVIC_EnableIRQ(GPION_IRQn);//开启N端口中断 
	EXTI_Open(GPION, PIN1);//开始PN1中断
	
	EXTI_Clear(GPION, PIN1);//清除中断标志位	
//	TIMR_Init(TIMR4, TIMR_MODE_TIMER, CyclesPerUs, 100, 1);	//配置定时器4为到时中断模式  ---TIMR_MODE_COUNTER计数模式
//	TIMR_Start(TIMR4);//开启定时器

}





//扫描红外遥控
uint8_t remote_loop(void)
{
	uint8_t i;
		remote_key=255;
		if(Remote_Rdy)//红外接收到数据
		{	

			remote_key=Remote_Process();
			switch(remote_key)
			{
				case 24: //按键<
#if tiaoshi
printf("<\n");
#endif		
					if(!flag){//未改动之前亮度标志位 idflag==6
						flashflagbl=1;
						backlight-=1;
						if(backlight>8||backlight<1)backlight=1;
						if(backlight==1){
						PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							if(levelflag==3&&idflag==6)lv_img_set_src(blimg, &BL1); /* 设置亮度 */
							}
						if(backlight==2){
						PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							if(levelflag==3&&idflag==6)lv_img_set_src(blimg, &BL2); /* 设置亮度 */
							}
						if(backlight==3){
						PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							if(levelflag==3&&idflag==6)lv_img_set_src(blimg, &BL3); /* 设置亮度 */
							}
						if(backlight==4){
						PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							if(levelflag==3&&idflag==6)lv_img_set_src(blimg, &BL4); /* 设置亮度 */
							}
						if(backlight==5){
						PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							if(levelflag==3&&idflag==6)lv_img_set_src(blimg, &BL5); /* 设置亮度 */
							}
						if(backlight==6){
						PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							if(levelflag==3&&idflag==6)lv_img_set_src(blimg, &BL6); /* 设置亮度 */
							}
						if(backlight==7){
						PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							if(levelflag==3&&idflag==6)lv_img_set_src(blimg, &BL7); /* 设置亮度 */
							}
						if(backlight==8){
							PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							if(levelflag==3&&idflag==6)lv_img_set_src(blimg, &BL8); /* 设置亮度 */
							} 	
						}
				if(levelflag==2)lv_label_set_text_fmt(menulb2, "8/%d",backlight);
								remote_key=0;			
									break; 	 
				
				case 152://按键ENTER
#if tiaoshi
 printf("ENTER\n");
#endif	
								remote_key=1;		
									break;  
				
				case 88://按键>
#if tiaoshi
printf(">\n"); 
#endif
				
						if(!flag){
						flashflagbl=1;
						backlight+=1;
						if(backlight>8)backlight=8;
						if(backlight==1){
						PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							if(levelflag==3&&idflag==6)lv_img_set_src(blimg, &BL1); /* 设置亮度 */
							}
						if(backlight==2){
						PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							if(levelflag==3&&idflag==6)lv_img_set_src(blimg, &BL2); /* 设置亮度 */
							}
						if(backlight==3){
						PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							if(levelflag==3&&idflag==6)lv_img_set_src(blimg, &BL3); /* 设置亮度 */
							}
						if(backlight==4){
						PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							if(levelflag==3&&idflag==6)lv_img_set_src(blimg, &BL4); /* 设置亮度 */
							}
						if(backlight==5){
						PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							if(levelflag==3&&idflag==6)lv_img_set_src(blimg, &BL5); /* 设置亮度 */
							}
						if(backlight==6){
						PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							if(levelflag==3&&idflag==6)lv_img_set_src(blimg, &BL6); /* 设置亮度 */
							}
						if(backlight==7){
						PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							if(levelflag==3&&idflag==6)lv_img_set_src(blimg, &BL7); /* 设置亮度 */
							}
						if(backlight==8){
							PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							if(levelflag==3&&idflag==6)lv_img_set_src(blimg, &BL8); /* 设置亮度 */
							}  	
						}
						if(levelflag==2)lv_label_set_text_fmt(menulb2, "8/%d",backlight);
								remote_key=2;					
									break;				
									
				case 104://按键BACK
#if tiaoshi
printf("BACK\n"); 
#endif
				
	if ( levelflag==2)  
 {
	 idflag=0;
	 
				
        create_main_win();
				
        //lv_scr_load(mainoj);
		

 }
 else if ( levelflag==3) 
 {
		if(idflag==1){lv_obj_del_async(ojw1);}
		if(idflag==2){lv_obj_del_async(ojw2);}
		if(idflag==3){lv_obj_del_async(ojw3);}
		if(idflag==4){lv_obj_del_async(ojw4);}
		if(idflag==5){lv_obj_del_async(ojw5);}
		if(idflag==6){lv_obj_del_async(ojw6);}
		if(idflag==7){lv_obj_del_async(ojw7);}
		if(idflag==8){lv_obj_del_async(ojw8);}
		if(nextlock==1)nextlock=0;
		if(sliflag>=1&&sliflag<=7){
			flag=0;
			sliflag=0;
		}
		create_menu_win();

		 //lv_scr_load(scr);
 }
								remote_key=3;					
									break; 
				
				case 232://按键+db
#if tiaoshi
printf("+db\n"); 
#endif
			if(!enlock){//旋钮锁放开
				flashflag=1;
				if(soundBtemp>26){
				if(rsp==1)soundBtemp-=6;
				else if(rsp==2)soundBtemp-=8;
				else if(rsp==3)soundBtemp-=10;
				else soundBtemp-=1;
				}
				else 
				soundBtemp-=1;
					if(soundBtemp<0){soundBtemp=0;write_volume(0,0);}
					if(soundBtemp>=90&&soundBtemp<=98)soundB=(90+(soundBtemp%89)*4)*2;
					else soundB=soundBtemp*2;
				if(soundBflag)
				showdblb(soundB);		
				soundBL=soundB+soundBLtemp,soundBR=soundB+soundBRtemp;
				if(soundBL>255)soundBL=255;if(soundBR>255)soundBR=255;
				write_volume(soundBL,soundBR);//写入9039寄存器，控制音量
			}	

									break;	
			
				case 216://按键-db
#if tiaoshi
printf("-db\n"); 
#endif
			if(!enlock){//旋钮锁放开
				flashflag=1;
				if(lsp==1)soundBtemp+=6;
				else if(lsp==2)soundBtemp+=8;
				else if(lsp==3)soundBtemp+=10;
				else 
				soundBtemp+=1;
					if(soundBtemp>98){soundBtemp=99;soundB=255;}
					else if(soundBtemp>=90&&soundBtemp<=98)soundB=(90+(soundBtemp%89)*4)*2;
					else soundB=soundBtemp*2;
				if(soundBflag)
				showdblb(soundB);
				soundBL=soundB+soundBLtemp,soundBR=soundB+soundBRtemp;
				if(soundBL>255)soundBL=255;if(soundBR>255)soundBR=255;
				write_volume(soundBL,soundBR);//写入9039寄存器，控制音量
				
			}
									break;
				
				case 184://UP
#if tiaoshi
printf("UP\n"); 
#endif
	
			remote_key=6;
									break;
				case 168://DOWN
#if tiaoshi
printf("DOWN\n"); 
#endif

			remote_key=7;
									break;								
				
				case 248://睡眠/关机
#if tiaoshi
printf("SLEEP\n"); // 设置标签文字
#endif
				if(sleepon==1){
				remote_key=5;	
					}
									break;
					
					
				case 200://静音
#if tiaoshi
printf("MUTE\n"); // 设置标签文字
#endif
				if(!enlock){//旋钮锁放开
				if(mutef){
					muteon=!muteon;
					mutetime=1;

				remote_key=8;	
				}
			}
									break;
				case 40://输入选择
#if tiaoshi
printf("INPUTINSEL\n"); // 设置标签文字
#endif
				if(!enlock){//旋钮锁放开
						WrBuff[1]=*((volatile uint32_t *)(EEPROM_ADDR + 1*4));
						WrBuff[2]=*((volatile uint32_t *)(EEPROM_ADDR + 2*4));
						WrBuff[3]=*((volatile uint32_t *)(EEPROM_ADDR + 3*4));
						if(WrBuff[1]==-1)WrBuff[1]=0;
						if(WrBuff[2]==-1)WrBuff[2]=0;
						if(WrBuff[1]==0&&WrBuff[2]==0){//USB > COA
						HIN0;LIN1;//10 cs8422
						WrBuff[1]=GET_IN0,WrBuff[2]=GET_IN1;
						write_CS8422_regs(0x03,0xC4);//8422同轴输入
						WrBuff[3]=8;
							if(levelflag==1){
							lv_img_set_src(mainojimg, &COA); /* 设置图片源 */
							lv_label_set_text_fmt(mainlb3, "%s","COA");
							}else if(levelflag==2)lv_label_set_text_fmt(menulb1, "%s","同轴");
								else if(levelflag==3&&idflag==1){
								lv_obj_set_style_img_opa(ojenter2img,LV_OPA_100,0);
								lv_obj_set_style_img_opa(ojenter1img,LV_OPA_0,0);
								lv_obj_set_style_img_opa(ojenter3img,LV_OPA_0,0);
								lv_obj_set_style_img_opa(ojenter4img,LV_OPA_0,0);
								lv_group_focus_obj(w1bt2);//聚焦
								}
							}	
						else if(WrBuff[1]==1&&WrBuff[2]==0){//COA > TOS
							if(WrBuff[3]==8){
						HIN0;LIN1;//10 cs8422
						WrBuff[1]=GET_IN0,WrBuff[2]=GET_IN1;
						write_CS8422_regs(0x03,0x84);//8422光纤输入 
						WrBuff[3]=16;
								if(levelflag==1){
								lv_img_set_src(mainojimg, &TOS); /* 设置图片源 */
								lv_label_set_text_fmt(mainlb3, "%s","TOS");
								}else if(levelflag==2)lv_label_set_text_fmt(menulb1, "%s","光纤");
									else if(levelflag==3&&idflag==1){
									lv_obj_set_style_img_opa(ojenter3img,LV_OPA_100,0);
									lv_obj_set_style_img_opa(ojenter1img,LV_OPA_0,0);
									lv_obj_set_style_img_opa(ojenter2img,LV_OPA_0,0);
									lv_obj_set_style_img_opa(ojenter4img,LV_OPA_0,0);
										lv_group_focus_obj(w1bt3);//聚焦
								}
						}
							else if(WrBuff[3]==16){//TOS > HDMI
						 LIN0;HIN1;//01 hdmi
						 WrBuff[1]=GET_IN0,WrBuff[2]=GET_IN1;
								
								if(levelflag==1){
								lv_img_set_src(mainojimg, &HDMI); /* 设置图片源 */
								lv_label_set_text_fmt(mainlb3, "%s","HDMI");
								}else if(levelflag==2)lv_label_set_text_fmt(menulb1, "%s","HDMI");
									else if(levelflag==3&&idflag==1){
									lv_obj_set_style_img_opa(ojenter4img,LV_OPA_100,0);
									lv_obj_set_style_img_opa(ojenter1img,LV_OPA_0,0);
									lv_obj_set_style_img_opa(ojenter2img,LV_OPA_0,0);
									lv_obj_set_style_img_opa(ojenter3img,LV_OPA_0,0);
										lv_group_focus_obj(w1bt4);//聚焦
								}
							}
						}
						else if((WrBuff[1]==0&&WrBuff[2]==1)){//HDMI > USB
						LIN0;LIN1;//00 usb(xmos)
						WrBuff[1]=GET_IN0,WrBuff[2]=GET_IN1;
						//WrBuff[3]=1;	
								if(levelflag==1){
								lv_img_set_src(mainojimg, &USB); /* 设置图片源 */
								lv_label_set_text_fmt(mainlb3, "%s","USB");
								}else if(levelflag==2)lv_label_set_text_fmt(menulb1, "%s","USB");
									else if(levelflag==3&&idflag==1){
									lv_obj_set_style_img_opa(ojenter1img,LV_OPA_100,0);
									lv_obj_set_style_img_opa(ojenter2img,LV_OPA_0,0);
									lv_obj_set_style_img_opa(ojenter2img,LV_OPA_0,0);
									lv_obj_set_style_img_opa(ojenter3img,LV_OPA_0,0);
										lv_group_focus_obj(w1bt1);//聚焦
								}
						}
						
						if(levelflag==2)lv_obj_align_to(menulb1,lb1, LV_ALIGN_RIGHT_MID,15,0);
						if(levelflag==1){
						lv_obj_align_to(mainojimg, mainoj1,LV_ALIGN_CENTER, -335, -60);
						lv_obj_align_to(mainlb3, mainojimg, LV_ALIGN_CENTER, 0, 80);}
				flashflaginsel=1;
					}			
									break;
							
				case 56://滤波器
#if tiaoshi
printf("SHAPE\n"); // 设置标签文字
#endif
				if(!enlock){//旋钮锁放开
				write_9039m_regs(0x56, 0xFF);//静音所有通道
//				GPIO_ClrBit(GPIOM, PIN5);GPIO_ClrBit(GPION, PIN0);
				swm_delay_ms(100);
				
					WrBuff[4]=*((volatile uint32_t *)(EEPROM_ADDR + 4*4));
					if(WrBuff[4]==-1)WrBuff[4]=6;
							if(WrBuff[4]==7){WrBuff[4]=0;}
							else if(WrBuff[4]==6){WrBuff[4]=7;}
							else if(WrBuff[4]==5){WrBuff[4]=6;}
							else if(WrBuff[4]==4){WrBuff[4]=5;}
							else if(WrBuff[4]==2){WrBuff[4]=4;}
							else if(WrBuff[4]==1){WrBuff[4]=2;}
							else if(WrBuff[4]==0){WrBuff[4]=1;}
							else WrBuff[4]=0;
								
							if(WrBuff[4]==0)lvbosel=1;
							else if(WrBuff[4]==1)lvbosel=2;
							else if(WrBuff[4]==2)lvbosel=3;
							else lvbosel=WrBuff[4];
								
							if(levelflag==1){
							lv_label_set_text_fmt(mainlb4, "FIR > #FFCC33 %d#",lvbosel);
							}else if(levelflag==2)lv_label_set_text_fmt(menulb5, "7/%d",lvbosel);
								else if(levelflag==3&&idflag==2){//未改前滤波器标志位 idflag==5
							if(WrBuff[4]==7){
							lv_label_set_text_fmt(w5lb, "最小相位慢降降低色散 7/%d",7);
							}else if(WrBuff[4]==6)lv_label_set_text_fmt(w5lb, "最小相位慢降 7/%d",6);
							else if(WrBuff[4]==5)lv_label_set_text_fmt(w5lb, "最小相位快降 7/%d",5);
							else if(WrBuff[4]==4)lv_label_set_text_fmt(w5lb, "线性相位快降 7/%d",4);
							else if(WrBuff[4]==2)lv_label_set_text_fmt(w5lb, "线性相位慢降 7/%d",3);
							else if(WrBuff[4]==1)lv_label_set_text_fmt(w5lb, "apodizing快降 7/%d",2);
							else lv_label_set_text_fmt(w5lb, "最小相位 7/%d",1);
								lv_obj_align_to(w5lb,menulb, LV_ALIGN_RIGHT_MID,15,0);
								}

							read_9039m_regs(FIR_Shape_Set,&FIR_value);//先读出寄存器值	
							FIR_value &=(~0x07);//把0-2Bit位清0，其他位不变
							FIR_value |=WrBuff[4];//把选择的寄存器值写入寄存器
							write_9039m_regs(FIR_Shape_Set, FIR_value);
//									write_9039m_regs(FIR_Shape_Set, 0x60+WrBuff[4]);
							
							WrBuff[18]=levelflag<<4;
							if(levelflag==3)
							WrBuff[18]+=idflag;	
								
							FLASH_Erase(EEPROM_ADDR);
							FLASH_Write(EEPROM_ADDR, WrBuff, 20);

									
				GPIO_ClrBit(GPIOB, PIN4);//复位单片机
	 
									
								}
								break;
							
                	default:
										
									break;
			}
			
		}
		if(remote_key!=255&&remote_key!=248&&remote_key!=5)
		{
				blflag=0;
				if(blflagt){
				PWM_SetHDuty(PWM1, PWM_CH_B,WrBuff[5]*100*10);//设置PWM电平时间
				blflagt=0;//暗屏标志
				enlock=0;//旋钮放开
				remote_key=255;
				}
		}
		return remote_key;
}


uint8_t Remote_Process(void)//红外解码
{               
    uint8_t t1;
	uint8_t t2; 

    t1 = ((Remote_Odr >> 8)&(0xff)); 	//得到控制码
    t2 = (Remote_Odr >> 0)&0xff;		//得到控制反码 
    Remote_Rdy = 0;			//清除标记 

//关掉遥控识别码，你也可以打开	
//    if(t1==(u8)~t2&&t1==REMOTE_ID)	//检验遥控识别码(ID)及地址 
//    { 
//        t1=Remote_Odr>>8;
//        t2=Remote_Odr; 	
//    }    
	
//    if(t1==(u8)~t2)
	return t1; 		//处理键值 
//	  return 0xff; //否则返回处理错误
}


void GPION_Handler(void)//外部中断M服务函数
{
	uint16_t	res=0; 
	uint8_t OK=0; 
	uint8_t RODATA=0;
	
	if(EXTI_State(GPION, PIN1))//如果中断源是PM3
	{
				
			//按键消抖,请最好不要用sys_delay_ms函数,如果一定要使用的话,
			//请确保  定时器的中断优先级  比  外部中断的优先级  高
			while(1)
			{ 	
					if(RDATA==1)//有高脉冲出现
					{
							//清空定时器值    
							res=Pulse_Width_Check();		//获得此次高脉冲宽度(高电平时间)

							if(res>=55)break;			//非有用信号
							if(res>=40&&res<50){		OK=1;}  	//获得前导位(4.5ms)
							else if(res>=20&&res<30)   	//按键次数加一,2500为标准值2.5ms
							{  							    		 
									Remote_Rdy=1;			//接受到数据
									Remote_Cnt++;			//按键次数增加
								if(Remote_Cnt==10)
									sleepon=1;
								else sleepon=0;
								if(Remote_Cnt>18)lsp=3,rsp=3;
								else if(Remote_Cnt>15)lsp=2,rsp=2;
								else if(Remote_Cnt>12)lsp=1,rsp=1;
								if(Remote_Cnt>1)
									mutef=0;
									break;
							}
							else if(res>=11&&res<20)RODATA=1;//1680为标准值,1680us
							else if(res>=3&&res<9)RODATA=0;//560为标准值,560us	  
							if(OK)
							{
									Remote_Odr<<=1;							
									Remote_Odr+=RODATA; 	//这里得到的是一个32位的码值，	//地址码、地址反码、控制码、控制反码
									Remote_Cnt=0; 		//按键次数清零
									lsp=0,rsp=0;
									sleepon=0;
									mutef=1;
							} 				
					}			 					          
			}		
EXTI_Clear(GPION, PIN1);//清除中断标志位			
	}
}


/*功能	：检测脉冲宽度*****************************************/
/*说明	：最长脉宽为5ms
//返回值: t代表脉宽为t*20us(T=1~250); *************************/
uint16_t Pulse_Width_Check(void)
{
    uint16_t t=0;	 
		
    while(RDATA)//如果是高电平   我们值测量高电平的长度
    {
				swm_delay_us(100);
				t++;
        if(t>=55)	return t; 			//超时溢出	
    }
		
    return t;
}	



//uint32_t delay_time;      //us级延时
//void TIMR4_Handler(void)//定时器4服务函数
//{
//	TIMR_INTClr(TIMR4);//清空中断标志位
//	
//	if(delay_time){delay_time--;}
//}


////利用定时器4进行精确延时,单位1=100us
//void delay_us(uint32_t us)
//{
//	delay_time = us;
//	while(delay_time);
//}

void showdblb(int soundB){
//lv_obj_set_style_text_decor(mainlb2,LV_TEXT_DECOR_UNDERLINE,0);
						if(soundB>=188&&soundB<=252){switch_num_shi(89+(soundB-180)/8);switch_num_ge(89+(soundB-180)/8);}
						else if(soundB==255){switch_num_shi(99);switch_num_ge(99);}
						else {switch_num_shi(soundB/2);switch_num_ge(soundB/2);}
						if(soundB==0)lv_obj_set_style_img_opa(mainfu,LV_OPA_0,0);
						if(soundB==2)lv_obj_set_style_img_opa(mainfu,LV_OPA_100,0);
//					if((soundB/2)%10==0||(soundB/2)%10==9){lv_obj_align_to(mainlb2, alignoj2, LV_ALIGN_RIGHT_MID, -55, -15);}
//					if(rsp==1||lsp==1){lv_obj_align_to(mainlb2, alignoj2, LV_ALIGN_RIGHT_MID, -55, -15);}

//				if(soundB/2>99)lv_obj_align_to(mainlb2, mainlb1, LV_ALIGN_BOTTOM_RIGHT, 300, 0);
//				else if(soundB/2>9)lv_obj_align_to(mainlb2, mainlb1, LV_ALIGN_BOTTOM_RIGHT, 265, 0);
//				else lv_obj_align_to(mainlb2, mainlb1, LV_ALIGN_BOTTOM_RIGHT, 230, 0);
				lv_slider_set_value(mainsli, 99-soundBtemp, LV_ANIM_OFF);
			
}
	

