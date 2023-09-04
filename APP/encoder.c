#include "encoder.h"
#include "app_test.h"
#include "main.h"
#include "lvgl/lvgl.h"
#include "9039M.h"

#define ZUNI 2

extern lv_obj_t *mainlb2,*w2bt1,*w5bt1,*w6bt1,*w6bt2,*blimg,*w1sli2,*w1sli3,*w1lbx1,*w1lbx2,*w1lbx3,*mainsli,*mainlb1,*mainlb5,
*lb1,*alignoj1,*alignoj2,*mainfu;//
extern lv_obj_t *w5lb,*menulb,*menulb1,*w6lbx1,*w6lbx2,*limg,*rimg,*w2bt4,*w2bt3,*w2lb1,*w2lb2;
extern uint8_t flag,soundBflag,sliflag,idflag,levelflag; //标志位
/* 值 */
extern uint8_t backlight,FIR_value; //值
extern int soundB,soundBtemp,soundBRtemp,soundBLtemp,lvbosel;
extern uint16_t lsp,rsp;//加速标志位
extern uint16_t enlock;//旋钮锁
extern uint32_t WrBuff[20],blflag,blflagt;
float samfretemp=0,samfre=0,frenum=0;
int soundBR=0,soundBL=0,bltime=0,blsleep=0;
uint8_t pinsum=0;
//int samChannel[4];
//int sampin[4];
//int dsoChannel[2];
//int dsopin[2];

uint16_t KUP=0;//旋钮编码器是否锁死状态标志位  1为锁死  0为解除
uint16_t cou=0,zunil=0,zunir=0,zuni=ZUNI;//旋钮编码器锁死状态计数
uint16_t coul=0,cour=0;
uint8_t getcode=99;//旋转编码器状态标志位
uint8_t dbflag=1,longprflag=0,alignf=0;
uint32_t flashflag=0;
uint32_t flashflagpcm=0;
uint32_t flashflagbl=0,rmtemp;
extern uint32_t flashflaglvbo;
LV_IMG_DECLARE(BL0)
LV_IMG_DECLARE(BL1)
LV_IMG_DECLARE(BL2)
LV_IMG_DECLARE(BL3)
LV_IMG_DECLARE(BL4)
LV_IMG_DECLARE(BL5)
LV_IMG_DECLARE(BL6)
LV_IMG_DECLARE(BL7)
LV_IMG_DECLARE(BL8)

void ENCODER_INIT(void)//初始化旋转编码器IO的相关口
{
//	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);			//输出， 接LED
//	GPIO_Init(GPIOD, PIN8, 1, 0, 0, 0);			//输出， 接LED
	
	GPIO_Init(GPIOD, PIN11, 0, 1, 0, 0);		//初始化SW  输入，上拉使能，接SW  d11
	GPIO_Init(GPIOM, PIN1, 0, 1, 0, 0);			//初始化KB  输入，上拉使能，接KB M1
	GPIO_Init(GPIOM, PIN0, 0, 1, 0, 0);			//初始化KA  输入，上拉使能，接KA M0
//	SYS->PDWKEN |= (1 << PIN11);						//开启PD11引脚低电平唤醒

}


//编码器状态读取
uint8_t ENCODER_READ(uint8_t levelflag)
{
	
//ENCODER_L=KB    ENCODER_R=KA   以KB为低电平得一瞬间记录KA的值，当KA完全不亮时是逆时针转，当KA有亮时是顺时针转
	uint8_t a=99,b=99;//存放按键的值
	uint8_t kt;	

	if(levelflag!=1){//主界面不打开扫描的左右

	if(ENCODER_L)KUP=0;	//判断旋钮是否解除锁死

	if((ENCODER_L)==0&&KUP==0) //判断是否旋转旋钮，同时判断是否有旋钮锁死
		{
			swm_delay_ms(1);
			kt=ENCODER_R;	//把旋钮另一端电平状态记录
			swm_delay_ms(1); //延时
			if(!ENCODER_L)//去抖
			{ 
				if(kt==0)//用另一端判断左或右旋转
				{ 
					zunir++;zunil=0;
					if(zunir==zuni){
					a=2;//顺时针转
					zunir=0;
					}
				}else
				{
					zunil++;zunir=0;
					if(zunil==zuni){
					a=1;//逆时针转
					zunil=0;
					}
				}
				cou=0; //初始锁死判断计数器
				while(ENCODER_L==0&&cou<200)//等待放开旋钮，同时累加判断锁死
				{ 
					cou++;KUP=1;
					swm_delay_ms(3); //					
				}
				if(cou>=200)a=99;
				
				blflag=0;
				if(blflagt){
				PWM_SetHDuty(PWM1, PWM_CH_B,WrBuff[5]*100*10);//设置PWM电平时间
				blflagt=0;//暗屏标志
				enlock=0;//旋钮放开
				a=99;
				}
			}
   		}
	}

	if(a==99)
		b=check_wkup();//判断旋钮键值，双击返回2，没按返回0，长按返回4
	if(b!=0){	
		
			if(b==1)
				a=3;//单击
			else if(b==2)
			{
					//soundBtemp=soundBtemp+1;
					//lv_label_set_text_fmt(lb1,"%d touch",soundBtemp);//设置DB数值显示
					//while(ENCODER_SW==0){}//等待按键放开		
				a=4;//双击
			}
			else if(b==4)
				a=5;//长按
				blflag=0;
				if(blflagt){
				PWM_SetHDuty(PWM1, PWM_CH_B,WrBuff[5]*100*10);//设置PWM电平时间
				blflagt=0;//暗屏标志
				enlock=0;//旋钮放开
				a=99;
				}
	  }
	return a;
}  




//判断按键
uint8_t check_wkup(void)
{
	uint16_t sw_cout=0;
	uint8_t ak=0;
	if(longprflag){while(!ENCODER_SW);//长按后产生一个冷却,放走抖动
		longprflag=0;
	}
	if(!ENCODER_SW)//判断旋钮是否按下 
		{
			swm_delay_ms(20);//去抖动
			if(!ENCODER_SW){
			while(!ENCODER_SW&&sw_cout<150){//长按判断的计时
				sw_cout++;
				swm_delay_ms(10);
			}
			if(sw_cout>=50){//长按
//				while(!ENCODER_SW);
				longprflag=1;//长按冷却
				return 4;
			}
			else{//非长按,单双击
			for(sw_cout=0;sw_cout<10;sw_cout++)//检测双击200ms
			{
			swm_delay_ms(20);
				if(!ENCODER_SW)//双击
				{
				while(!ENCODER_SW);
					return 2;
				}
			}
			swm_delay_ms(20);
			return 1;//单击
			}
		}
	}
  return 0;
}





//扫描旋转编码器函数
void ENCODER_LOOP(void)
{	
	int i,debug_read;

	getcode=ENCODER_READ(0);	//读出旋转编码器按键值
	rmtemp=remote_loop();
		if(!enlock)//旋钮锁放开
	{
		if(getcode==2||rmtemp==6)//顺时针转/遥控>
		{
				if(sliflag==1){//调节滑动条1 BL
						flashflagbl=1;
				backlight+=1;
			  if(backlight>8)backlight=8;
				if(backlight==1){
				PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
					lv_img_set_src(blimg, &BL1); /* 设置亮度 */
					}
				if(backlight==2){
				PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
					lv_img_set_src(blimg, &BL2); /* 设置亮度 */
					}
				if(backlight==3){
				PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
					lv_img_set_src(blimg, &BL3); /* 设置亮度 */
					}
				if(backlight==4){
				PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
					lv_img_set_src(blimg, &BL4); /* 设置亮度 */
					}
				if(backlight==5){
				PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
					lv_img_set_src(blimg, &BL5); /* 设置亮度 */
					}
				if(backlight==6){
				PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
					lv_img_set_src(blimg, &BL6); /* 设置亮度 */
					}
				if(backlight==7){
				PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
					lv_img_set_src(blimg, &BL7); /* 设置亮度 */
					}
				if(backlight==8){
					PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
					lv_img_set_src(blimg, &BL8); /* 设置亮度 */
					} 		

				//lv_slider_set_value(w2bt1, backlight*12.5, LV_ANIM_OFF); /* 设置当前值 */
					//lv_img_set_src(blimg, &BL1);
				//lv_label_set_text_fmt(w1lbx1, "%d", backlight);
				}
				else if(sliflag==2){//左平衡 +db
					flashflag=1;//写入flash标志
					soundBLtemp-=1;
					if(soundBL!=soundB)
					soundBL-=1;
					if(soundBL<0)soundBL=0;
					write_lvolume(soundBL);
					if(soundBLtemp<1){soundBLtemp=0;lv_label_set_text_fmt(w6lbx1, "%d", soundBLtemp);}
					else lv_label_set_text_fmt(w6lbx1, "%.1f", -soundBLtemp/2.0);
					lv_obj_align_to(w6lbx1,w6bt1, LV_ALIGN_RIGHT_MID,0,5);
					
						if(soundBLtemp==0)lv_img_set_src(limg, &BL8); 
						if(soundBLtemp==1)lv_img_set_src(limg, &BL7); 
						if(soundBLtemp==2)lv_img_set_src(limg, &BL6); 
						if(soundBLtemp==3)lv_img_set_src(limg, &BL5); 
						if(soundBLtemp==4)lv_img_set_src(limg, &BL4); 
						if(soundBLtemp==5)lv_img_set_src(limg, &BL3); 
						if(soundBLtemp==6)lv_img_set_src(limg, &BL2); 
						if(soundBLtemp==7)lv_img_set_src(limg, &BL1); 
						if(soundBLtemp==8)lv_img_set_src(limg, &BL0); 
					lv_obj_align_to(limg,w6bt1, LV_ALIGN_CENTER,0,0);

				}
				else if(sliflag==3){//右平衡 +db
					flashflag=1;//写入flash标志
					soundBRtemp-=1;
					if(soundBR!=soundB)
					soundBR-=1;
					if(soundBR<0)soundBR=0;
					write_rvolume(soundBR);
					if(soundBRtemp<1){soundBRtemp=0;lv_label_set_text_fmt(w6lbx2, "%d", soundBRtemp);}
					else lv_label_set_text_fmt(w6lbx2, "%.1f", -soundBRtemp/2.0);
					lv_obj_align_to(w6lbx2,w6bt2, LV_ALIGN_RIGHT_MID,0,5);
						if(soundBRtemp==0)lv_img_set_src(rimg, &BL8); 
						if(soundBRtemp==1)lv_img_set_src(rimg, &BL7); 
						if(soundBRtemp==2)lv_img_set_src(rimg, &BL6); 
						if(soundBRtemp==3)lv_img_set_src(rimg, &BL5); 
						if(soundBRtemp==4)lv_img_set_src(rimg, &BL4); 
						if(soundBRtemp==5)lv_img_set_src(rimg, &BL3); 
						if(soundBRtemp==6)lv_img_set_src(rimg, &BL2); 
						if(soundBRtemp==7)lv_img_set_src(rimg, &BL1); 
						if(soundBRtemp==8)lv_img_set_src(rimg, &BL0); 
					lv_obj_align_to(rimg,w6bt2, LV_ALIGN_CENTER,0,0);
				}
				else if(sliflag==4){//滑动条4 滤波器选择
					//flashflaglvbo=1;
					if(WrBuff[4]==0)lvbosel=1;
					else if(WrBuff[4]==2)lvbosel=3;
					else if(WrBuff[4]==1)lvbosel=2;
					else lvbosel=WrBuff[4];
					lvbosel++;
			
				  	if(lvbosel>6)lvbosel=7;
					if(lvbosel==1)WrBuff[4]=0;
					else if(lvbosel==2)WrBuff[4]=1;
					else if(lvbosel==3)WrBuff[4]=2;
					else WrBuff[4]=lvbosel;
				
					read_9039m_regs(FIR_Shape_Set,&FIR_value);//先读出寄存器值	
					FIR_value &=(~0x07);//把0-2Bit位清0，其他位不变
					FIR_value |=WrBuff[4];//把选择的寄存器值写入寄存器
					write_9039m_regs(FIR_Shape_Set, FIR_value);
//					write_9039m_regs(FIR_Shape_Set, 0x60+WrBuff[4]);
					if(WrBuff[4]==7){
					lv_label_set_text_fmt(w5lb, "最小相位慢降降低色散 7/%d",7);
					}
					else if(WrBuff[4]==6)lv_label_set_text_fmt(w5lb, "最小相位慢降 7/%d",6);
					else if(WrBuff[4]==5)lv_label_set_text_fmt(w5lb, "最小相位快降 7/%d",5);
					else if(WrBuff[4]==4)lv_label_set_text_fmt(w5lb, "线性相位快降 7/%d",4);
					else if(WrBuff[4]==2)lv_label_set_text_fmt(w5lb, "线性相位慢降 7/%d",3);
					else if(WrBuff[4]==1)lv_label_set_text_fmt(w5lb, "apodizing快降 7/%d",2);
					else lv_label_set_text_fmt(w5lb, "最小相位 7/%d",1);
					lv_obj_align_to(w5lb,menulb, LV_ALIGN_RIGHT_MID,15,0);
					
//								read_9039m_regs(FIR_Shape_Set,&FIR_value);//先读出寄存器值	
//								lv_label_set_text_fmt(w5lb, "%x ",FIR_value);
		}
			else if(sliflag==5){
					//flashflaglvbo=1;//写入flash标志
					WrBuff[15]++;if(WrBuff[15]>60)WrBuff[15]=60;
					lv_label_set_text_fmt(w2lb1, "%dS",WrBuff[15]);
					lv_obj_align_to(w2lb1,menulb, LV_ALIGN_RIGHT_MID,20,0);
				}
			else if(sliflag==6){
					//flashflaglvbo=1;//写入flash标志
				WrBuff[16]++;if(WrBuff[16]>8)WrBuff[16]=8;
				if(WrBuff[16]>WrBuff[5])WrBuff[16]=WrBuff[5]-1;
				lv_label_set_text_fmt(w2lb2, "%d",WrBuff[16]);
				//lv_obj_align_to(w2lb2,menulb1, LV_ALIGN_RIGHT_MID,20,0);
				}

	}				
		else if(getcode==1||rmtemp==7)//逆时针转/遥控<
		{
				if(sliflag==1){//调节滑动条1 BL
						flashflagbl=1;
						backlight-=1;
						if(backlight>8||backlight<1)backlight=1;
						if(backlight==1){
						PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							lv_img_set_src(blimg, &BL1); /* 设置亮度 */
							}
						if(backlight==2){
						PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							lv_img_set_src(blimg, &BL2); /* 设置亮度 */
							}
						if(backlight==3){
						PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							lv_img_set_src(blimg, &BL3); /* 设置亮度 */
							}
						if(backlight==4){
						PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							lv_img_set_src(blimg, &BL4); /* 设置亮度 */
							}
						if(backlight==5){
						PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							lv_img_set_src(blimg, &BL5); /* 设置亮度 */
							}
						if(backlight==6){
						PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							lv_img_set_src(blimg, &BL6); /* 设置亮度 */
							}
						if(backlight==7){
						PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							lv_img_set_src(blimg, &BL7); /* 设置亮度 */
							}
						if(backlight==8){
							PWM_SetHDuty(PWM1, PWM_CH_B,backlight*100*10);//设置PWM电平时间
							lv_img_set_src(blimg, &BL8); /* 设置亮度 */
							} 	
					//lv_slider_set_value(w2bt1, backlight*12.5, LV_ANIM_OFF); /* 设置当前值 */
						//lv_img_set_src(blimg, &BL1);
						//lv_label_set_text_fmt(w1lbx1, "%d", backlight);
				}
				else if(sliflag==2){//滑动条5 左平衡
					flashflag=1;//写入flash标志
					soundBLtemp+=1;
					if(soundBLtemp>8)soundBLtemp=8;
					soundBL=soundB+soundBLtemp;
					if(soundBL>255){//超出修正
					soundBL=255;
					//soundBLtemp=255-soundB;	
					}
//					printf("soundBL=%d soundBLtemp=%d\n",soundBL,soundBLtemp);
					write_lvolume(soundBL);
					lv_label_set_text_fmt(w6lbx1, "%.1f", -soundBLtemp/2.0);
					lv_obj_align_to(w6lbx1,w6bt1, LV_ALIGN_RIGHT_MID,0,5);	

						if(soundBLtemp==0)lv_img_set_src(limg, &BL8); 
						if(soundBLtemp==1)lv_img_set_src(limg, &BL7); 
						if(soundBLtemp==2)lv_img_set_src(limg, &BL6); 
						if(soundBLtemp==3)lv_img_set_src(limg, &BL5); 
						if(soundBLtemp==4)lv_img_set_src(limg, &BL4); 
						if(soundBLtemp==5)lv_img_set_src(limg, &BL3); 
						if(soundBLtemp==6)lv_img_set_src(limg, &BL2); 
						if(soundBLtemp==7)lv_img_set_src(limg, &BL1); 
						if(soundBLtemp==8)lv_img_set_src(limg, &BL0); 	
					lv_obj_align_to(limg,w6bt1, LV_ALIGN_CENTER,0,0);					
				}
				else if(sliflag==3){//滑动条6 右平衡
					flashflag=1;//写入flash标志
					soundBRtemp+=1;
					if(soundBRtemp>8)soundBRtemp=8;
					soundBR=soundB+soundBRtemp;
					if(soundBR>255){//超出修正
					soundBR=255;
					//soundBRtemp=255-soundB;	
					}
//					printf("soundBR=%d soundBRtemp=%d\n",soundBR,soundBRtemp);
					write_rvolume(soundBR);
					lv_label_set_text_fmt(w6lbx2, "%.1f", -soundBRtemp/2.0);
					lv_obj_align_to(w6lbx2,w6bt2, LV_ALIGN_RIGHT_MID,0,5);
					
						if(soundBRtemp==0)lv_img_set_src(rimg, &BL8); 
						if(soundBRtemp==1)lv_img_set_src(rimg, &BL7); 
						if(soundBRtemp==2)lv_img_set_src(rimg, &BL6); 
						if(soundBRtemp==3)lv_img_set_src(rimg, &BL5); 
						if(soundBRtemp==4)lv_img_set_src(rimg, &BL4); 
						if(soundBRtemp==5)lv_img_set_src(rimg, &BL3); 
						if(soundBRtemp==6)lv_img_set_src(rimg, &BL2); 
						if(soundBRtemp==7)lv_img_set_src(rimg, &BL1); 
						if(soundBRtemp==8)lv_img_set_src(rimg, &BL0); 
					lv_obj_align_to(rimg,w6bt2, LV_ALIGN_CENTER,0,0);
				}
				else if(sliflag==4){//滑动条4 滤波器
					//flashflaglvbo=1;
					if(WrBuff[4]==0)lvbosel=1;
					else if(WrBuff[4]==2)lvbosel=3;
					else if(WrBuff[4]==1)lvbosel=2;
					else lvbosel=WrBuff[4];
					lvbosel--;
				  	if(lvbosel<1)lvbosel=1;
					if(lvbosel==1)WrBuff[4]=0;
					else if(lvbosel==2)WrBuff[4]=1;
					else if(lvbosel==3)WrBuff[4]=2;
					else WrBuff[4]=lvbosel;
//					read_9039m_regs(FIR_Shape_Set,&FIR_value);//先读出寄存器值	
//					FIR_value &=(~0x07);//把0-2Bit位清0，其他位不变
//					FIR_value |=WrBuff[4];//把选择的寄存器值写入寄存器
//					write_9039m_regs(FIR_Shape_Set, FIR_value);
					if(WrBuff[4]==7){
					lv_label_set_text_fmt(w5lb, "最小相位慢降降低色散 7/%d",7);
					}
					else if(WrBuff[4]==6)lv_label_set_text_fmt(w5lb, "最小相位慢降 7/%d",6);
					else if(WrBuff[4]==5)lv_label_set_text_fmt(w5lb, "最小相位快降 7/%d",5);
					else if(WrBuff[4]==4)lv_label_set_text_fmt(w5lb, "线性相位快降 7/%d",4);
					else if(WrBuff[4]==2)lv_label_set_text_fmt(w5lb, "线性相位慢降 7/%d",3);
					else if(WrBuff[4]==1)lv_label_set_text_fmt(w5lb, "apodizing快降 7/%d",2);
					else lv_label_set_text_fmt(w5lb, "最小相位 7/%d",1);
								
								
//					read_9039m_regs(FIR_Shape_Set,&FIR_value);//先读出寄存器值	
//					lv_label_set_text_fmt(w5lb, "%x ",FIR_value);
					lv_obj_align_to(w5lb,menulb, LV_ALIGN_RIGHT_MID,15,0);
		}
			else if(sliflag==5){
					//flashflaglvbo=1;//写入flash标志
					WrBuff[15]--;if(WrBuff[15]<3)WrBuff[15]=3;
					lv_label_set_text_fmt(w2lb1, "%dS",WrBuff[15]);
					lv_obj_align_to(w2lb1,menulb, LV_ALIGN_RIGHT_MID,20,0);
				}
			else if(sliflag==6){
				WrBuff[16]--;if(WrBuff[16]<1)WrBuff[16]=1;
				lv_label_set_text_fmt(w2lb2, "%d",WrBuff[16]);
				//lv_obj_align_to(w2lb2,menulb1, LV_ALIGN_RIGHT_MID,20,0);
			}
	}

		else if(getcode==3||rmtemp==1){//确认
			//确认滑动条1,退出编辑模式
			if(sliflag==1){
				lv_obj_clear_state(w2bt1,LV_STATE_EDITED);
				lv_group_focus_obj(w2bt1);	
			}
			else if(sliflag==2){
        		lv_obj_clear_state(w6bt1,LV_STATE_EDITED);
				lv_group_focus_obj(w6bt1);	
			} 
			else if(sliflag==3){
				lv_obj_clear_state(w6bt2,LV_STATE_EDITED);
				lv_group_focus_obj(w6bt2);	
			}
			else if(sliflag==4){
				lv_obj_clear_state(w5bt1,LV_STATE_EDITED);
				lv_group_focus_obj(w5bt1);	
					
				write_9039m_regs(0x56, 0xFF);//静音所有通道
				GPIO_ClrBit(GPIOM, PIN5);
				GPIO_ClrBit(GPION, PIN0);
				swm_delay_ms(100);
				
				read_9039m_regs(FIR_Shape_Set,&FIR_value);//先读出寄存器值	
				FIR_value &= (~0x07);//把0-2Bit位清0，其他位不变
				FIR_value |= WrBuff[4];//把选择的寄存器值写入寄存器
				write_9039m_regs(FIR_Shape_Set, FIR_value);
//				WrBuff[16]=*((volatile uint32_t *)(EEPROM_ADDR + 16*4));
//				if(WrBuff[18]==-1)WrBuff[16]=6;
//				if(WrBuff[4]!=WrBuff[16]){
				WrBuff[18]=0x32;
//				WrBuff[16]=WrBuff[4];
//				FLASH_Erase(EEPROM_ADDR);
//				FLASH_Write(EEPROM_ADDR, WrBuff, 20);
				flashflaglvbo=1;
				swm_delay_ms(300);
				GPIO_ClrBit(GPIOB, PIN4);//复位单片机
				
//				}
				
			}
			else if(sliflag==5){//滑动条5 
				lv_obj_clear_state(w2bt3,LV_STATE_EDITED);
				lv_group_focus_obj(w2bt3);	
				flashflaglvbo=1;
			}
			else if(sliflag==6){//滑动条6 
				lv_obj_clear_state(w2bt4,LV_STATE_EDITED);
				lv_group_focus_obj(w2bt4);	
				flashflaglvbo=1;
			}
//																					for(i=0x4A;i<0x52;i++){
//			if(read_9039m_regs(i,&debug_read)==1)//40
//				printf( "9039=0x%x %d\n", i,debug_read);//把读出的寄存器值写在屏幕上
//			}				
			flag=0;
			sliflag=0;
		}
	}
}

void sam_fre_init(void){

	GPIO_Init(GPIOD, PIN14, 0, 0, 1, 0);		//初始化DSD  0输入，01下拉使能
	GPIO_Init(GPIOM, PIN15, 0, 0, 1, 0);		//初始化DOP  输入，下拉使能
	GPIO_Init(GPIOD, PIN13, 0, 0, 1, 0);		//初始化F1  输入，下拉使能
	GPIO_Init(GPIOD, PIN12, 0, 0, 1, 0);		//初始化F2  输入，下拉使能 
	GPIO_Init(GPIOM, PIN7, 0, 0, 1, 0);			//初始化F3  输入，下拉使能
	GPIO_Init(GPIOM, PIN4, 0, 0, 1, 0);			//初始化F4  输入，下拉使能
	GPIO_Init(GPIOM, PIN9, 1, 0, 1, 0);			//初始化IN0  输出，下拉使能
	GPIO_Init(GPIOM, PIN10, 1, 0, 1, 0);		//初始化IN1  输出，下拉使能
}
//PCM，DSD显示
void sam_fre_chose(void){ 
// sampin[4]={GETF1,GETF2,GETF3,GETF4};
// dsopin[2]={GETF3,GETF4};
	flashflagpcm=1;
	pinsum=GETF3*2+GETF4*1;
	if(GETDSD&&!GETDOP&&!GETF1){
		if(pinsum==0)samfretemp=64;
		if(pinsum==1)samfretemp=128;
		if(pinsum==2)samfretemp=256;
		if(pinsum==3)samfretemp=512;

//lv_label_set_text_fmt(mainlba, "DSD");
		if(samfre==samfretemp&&frenum<2){
			frenum++;
			lv_label_set_text_fmt(mainlb1, "%d",(int)samfretemp);
			lv_label_set_text_fmt(mainlb5, "#FFCC33 DSD#");
		}else if(samfre!=samfretemp||frenum>4)frenum=0;
//		else{
//				lv_label_set_text_fmt(mainlb5,"DSD");
//				lv_label_set_text_fmt(mainlb1, "64");
//		}
		samfre=samfretemp;
//		write_9039m_regs(0x01, 0xB1);//DSD解码
//		write_9039m_regs(INSEL_SET, 0x42);//输入选择DSD
	}
	else if(GETDOP&&!GETDSD&&!GETF1){
		if(pinsum==0)samfretemp=64;
		if(pinsum==1)samfretemp=128;
		if(pinsum==2)samfretemp=256;
		
//lv_label_set_text_fmt(mainlba, "DOP");
			if(samfre==samfretemp&&frenum<2){
				frenum++;
				lv_label_set_text_fmt(mainlb1, "%d",(int)samfretemp);
				lv_label_set_text_fmt(mainlb5, "#FFCC33 DSD#");
			}else if(samfre!=samfretemp||frenum>4)frenum=0;
//			else{
//					lv_label_set_text_fmt(mainlb5,"DSD");
//					lv_label_set_text_fmt(mainlb1, "64");
//			}
			samfre=samfretemp;
//		write_9039m_regs(0x01, 0xB1);//DSD解码
//		write_9039m_regs(INSEL_SET, 0x42);//输入选择DSD
	}
	else{
			pinsum=GETF2*4+GETF3*2+GETF4*1;
		if(!GETF1){
		if(pinsum==0)samfretemp=44.1;
		if(pinsum==1)samfretemp=88.2;
		if(pinsum==2)samfretemp=176.4;
		if(pinsum==3)samfretemp=352.8;
		if(pinsum==4)samfretemp=705.6;
			if(samfre==samfretemp&&frenum<2){
				frenum++;
				lv_label_set_text_fmt(mainlb1, "%.1fK",samfretemp);
			}else if(samfre!=samfretemp)frenum=0;
			else{
				lv_label_set_text_fmt(mainlb5,"PCM");
				lv_label_set_text_fmt(mainlb1, "44.1K");
		}
			samfre=samfretemp;
		}
		else if(GETF1){
		if(pinsum==0)samfretemp=48;
		if(pinsum==1)samfretemp=96;
		if(pinsum==2)samfretemp=192;
		if(pinsum==3)samfretemp=384;
		if(pinsum==4)samfretemp=768;
			if(samfre==samfretemp&&frenum<2){
				frenum++;
				lv_label_set_text_fmt(mainlb1, "%dK",(int)samfretemp);
			}else if(samfre!=samfretemp||frenum>4)frenum=0;
			samfre=samfretemp;
		}else{
				lv_label_set_text_fmt(mainlb5,"PCM");
				lv_label_set_text_fmt(mainlb1, "48K");
		}
		
				if(samfre==samfretemp&&frenum<2){
				frenum++;
				lv_label_set_text_fmt(mainlb5,"PCM");}
				else if(samfre!=samfretemp||frenum>4)frenum=0;
			


	}
	
//	//lv_label_set_text_fmt(mainlb1, "%x",pinsum);
	lv_obj_align_to(mainlb1, alignoj1, LV_ALIGN_CENTER, 0, 0);

}


uint8_t ENCODER_DB(void)
{ 

	uint8_t a=99;//存放按键的值
	uint8_t kt;	

	if(ENCODER_L)KUP=0;	//判断旋钮是否解除锁死
	if((ENCODER_L)==0&&KUP==0) //判断是否旋转旋钮，同时判断是否有旋钮锁死
		{
			swm_delay_ms(1);
			kt=ENCODER_R;	//把旋钮另一端电平状态记录
			swm_delay_ms(3); //延时
			if(!ENCODER_L)//去抖
			{ 
				if(kt==0)//用另一端判断左或右旋转
				{ 
//					zunir++;zunil=0;
//					if(zunir==zuni){
//					a=2;//顺时针转
//					zunir=0;
//					}
					a=2;//顺时针转
				}else
				{
//					zunil++;zunir=0;
//					if(zunil==zuni){
//					a=1;//逆时针转
//					zunil=0;
//					}
					a=1;//逆时针转
				}
				cou=0; //初始锁死判断计数器
				while(ENCODER_L==0&&cou<200)//等待放开旋钮，同时累加判断锁死
				{ 
					cou++;KUP=1;
					swm_delay_ms(3); //					
				}
				if(cou>=200)a=99;
				
				blflag=0;
				if(blflagt){
				PWM_SetHDuty(PWM1, PWM_CH_B,WrBuff[5]*100*10);//设置PWM电平时间
				blflagt=0;//暗屏标志
				enlock=0;//旋钮放开
				a=99;
				}
			}
   	}
	return a;
}  

void ENCODER_DBLOOP(void)
{	
	getcode=ENCODER_DB();	//读出旋转编码器按键值
	if(!enlock){//旋钮锁放开
	  
		//FLASH_Write(EEPROM_ADDR, WrBuff1, 4);
	//lv_label_set_text_fmt(mainlb2, "#FF0000 %d",*((volatile uint32_t *)(EEPROM_ADDR + 0*4)));//设置旋转显示
		if(getcode==2)//顺时针转
		{
			
//				if(soundBflag==1){//调节dB
				cour++;coul=0;
			if(rsp==1&&soundBtemp>26){
					if(cour>=5)soundBtemp-=10;
					else if(cour>=4)soundBtemp-=8;
					else if(cour>=3)soundBtemp-=6;
					else soundBtemp-=1;
				}//zuni=1;
				else{ 
					soundBtemp-=1;
				}
				if(soundBtemp<1){soundBtemp=0;write_volume(0,0);}
				if(soundBtemp>=90&&soundBtemp<=98)soundB=(90+(soundBtemp%89)*4)*2;
				else soundB=soundBtemp*2;
				
//				lv_obj_set_style_text_decor(mainlb2,LV_TEXT_DECOR_UNDERLINE,0);

				if(soundB>=188&&soundB<=252){switch_num_shi(89+(soundB-180)/8);switch_num_ge(89+(soundB-180)/8);}
				else {switch_num_shi(soundB/2);switch_num_ge(soundB/2);}
				if(soundB==0)lv_obj_set_style_img_opa(mainfu,LV_OPA_0,0);
				if(soundB==2)lv_obj_set_style_img_opa(mainfu,LV_OPA_100,0);
			

				lv_slider_set_value(mainsli, 99-soundBtemp, LV_ANIM_OFF);
				soundBL=soundB+soundBLtemp,soundBR=soundB+soundBRtemp;
				if(soundBL>255)soundBL=255;if(soundBR>255)soundBR=255;
				write_volume(soundBL,soundBR);//写入9039寄存器，控制音量
				//lv_label_set_text_fmt(mainlb2, "#FF0000 +%d",soundB);//设置旋转显示
//				}	
				
				flashflag=1;//写入flash标志
		} 
		else if(getcode==1)//逆时针转
		{

//				if(soundBflag==1){//调节dB
			  coul++;cour=0;
				if(lsp==1){
					if(coul>=5)soundBtemp+=10;
					else if(coul>=4)soundBtemp+=8;
					else if(coul>=3)soundBtemp+=6;
					else soundBtemp+=1;
				}//zuni=1;
				else{ 
					soundBtemp+=1;}
				if(soundBtemp>98){soundBtemp=99;soundB=255;}
					else if(soundBtemp>=90&&soundBtemp<=98)soundB=(90+(soundBtemp%89)*4)*2;
					else soundB=soundBtemp*2;
//				lv_obj_set_style_text_decor(mainlb2,LV_TEXT_DECOR_UNDERLINE,0);
						if(soundB>=188&&soundB<=252){switch_num_shi(89+(soundB-180)/8);switch_num_ge(89+(soundB-180)/8);}
						else if(soundB==255){switch_num_shi(99);switch_num_ge(99);}
						else {switch_num_shi(soundB/2);switch_num_ge(soundB/2);}
						if(soundB==0)lv_obj_set_style_img_opa(mainfu,LV_OPA_0,0);
						if(soundB==2)lv_obj_set_style_img_opa(mainfu,LV_OPA_100,0);


					lv_slider_set_value(mainsli, 99-soundBtemp, LV_ANIM_OFF);
				soundBL=soundB+soundBLtemp,soundBR=soundB+soundBRtemp;
				if(soundBL>255)soundBL=255;if(soundBR>255)soundBR=255;
				write_volume(soundBL,soundBR);//写入9039寄存器，控制音量
				
					flashflag=1;//写入flash标志
				}
//		}
		else if(getcode==3){
//		soundBtemp++;
//			lv_label_set_text_fmt(mainlb2,"%d touch",soundBtemp);//设置DB数值显示
		soundBflag=0;
			
		}
	}
}

