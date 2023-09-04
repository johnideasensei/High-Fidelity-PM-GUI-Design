#include "app_test.h"
#include "lvgl/lvgl.h"
#include "9039M.h"
//#include "CS8416.h"
#include "CS8422.h"
#include "encoder.h"
extern lv_indev_t * indev_keypad;
extern lv_indev_t * indev_encoder;

//uint8_t get_code=99;

/* LVGL */
uint32_t flag=0,nextlock=0,prelock=0;//flag为编码器扫描标识,(导航0/编辑模式1)
uint32_t soundBflag=0;//音量扫描标识
uint32_t sliflag=0;//滑条扫描标识
uint32_t flashflaginsel=0;
uint32_t flashflaglvbo=0;
/* 值 */
extern int start;
extern float samfretemp,frenum;
extern uint32_t WrBuff[20];
extern int soundBR,soundBL,soundBt;
uint8_t backlight=0,idflag=0,posi=0,levelflag;
int soundB=0,soundBtemp=0,soundBRtemp=0,soundBLtemp=0,lvbosel=0;
uint8_t FIR_value,x5A=0,x01=0,x0A=0,outgpio=0;//读取滤波器变量
uint8_t valuetemp=0,debug_read=0,value=0;
uint32_t insel0,insel1,insel;//输入选择
#define FONT &lv_font_wode_ziti_35

/* 获取当前活动屏幕的宽高 */
#define scr_act_width() lv_obj_get_width(lv_scr_act())
#define scr_act_height() lv_obj_get_height(lv_scr_act())

lv_obj_t *mainoj,*mainoj1,*mainlb1,*mainlb2,*mainlba,*mainlb3,*mainlb4,*mainlb5,*mainsli,*mainsliimg,*alignoj1,*alignoj2,*mainojimg,*mainge,*mainshi,*mainfu,*mainmute;

lv_obj_t *oj,*oj1,*oj2,*oj3,*oj4,*oj5,*oj6,*oj7,*oj8,*oj9,*lb,*menuoj,*menulb,*menulb1,*menulb2,*menulb3,*menulb4,*menulb5,*menulb6,*menulb7,*menulb8;
lv_obj_t *ojimg,*ojnextimg,
*oj1img,*oj2img,*oj3img,*oj4img;
lv_obj_t *lb1,*lb2;
lv_obj_t *scr,*line,*wbt;

lv_obj_t *ojw1,*w1lb,*w1bt1,*w1bt2,*w1bt3,*w1bt4,*wlbback,*w1img,*ojenter1img,*ojenter2img,*ojenter3img,*ojenter4img,
*wlbok,*w1sli,*w1lbx1,*w1sli2,*w1sli3,*w1lbx2,*w1lbx3,*w1lbx4,*w1lbx5,*w1oj;
lv_obj_t *wlb1,*wlb2;
lv_obj_t *ojw2,*w2bt1,*w2bt2,*w2bt3,*w2bt4,*w2bt5,*w2bt6,*blmaximg,*blimg,*w2switch,*w2lb1,*w2lb2,*w2black;
lv_obj_t *ojw3,*w3bt1,*w3bt2,*w3bt3,*w3bt4,*w3bt5,*w3bt6,*w3switch1,*w3switch2,*w3switch3,*w3switch4,*w3switch5,*w3switch6;
lv_obj_t *ojw4,*w4bt1,*w4switch1;
lv_obj_t *ojw5,*w5bt1,*w5lb;
lv_obj_t *ojw6,*w6bt1,*w6bt2,*w6lbx1,*w6lbx2,*limg,*rimg;
lv_obj_t *ojw7,*w7bt1,*w7switch1;
lv_obj_t *ojw8,*w8bt1,*w8bt2,*w8switch1,*w8switch2;

lv_group_t *g,*gmain,*g1,*g2,*g3,*g4,*g5,*g6,*g7,*g8;

/* 声明字体 */
//LV_FONT_DECLARE(lv_font_wode_ziti_18)   
//LV_FONT_DECLARE(lv_font_wode_ziti_35)   
//LV_FONT_DECLARE(lv_font_wode_ziti_70_num)  
LV_FONT_DECLARE(lv_font_palatino_68_num)   
LV_FONT_DECLARE(lv_font_palatino_40_num)   
LV_FONT_DECLARE(lv_font_palatino_39_num)  
LV_FONT_DECLARE(lv_font_palatino_29_num)   
LV_FONT_DECLARE(lv_font_palatino_27_num)
LV_FONT_DECLARE(lv_font_palatino_25_num)   
LV_FONT_DECLARE(lv_font_simhei_25_num)  
//LV_FONT_DECLARE(lv_font_msyh_20_num) 
LV_FONT_DECLARE(lv_font_msyh_23_num)  


LV_IMG_DECLARE(USB)
LV_IMG_DECLARE(HDMI)
LV_IMG_DECLARE(TOS)
LV_IMG_DECLARE(COA)
LV_IMG_DECLARE(XIEBIAN) 
LV_IMG_DECLARE(back)
LV_IMG_DECLARE(back_p)
LV_IMG_DECLARE(next)
LV_IMG_DECLARE(next_p)


LV_IMG_DECLARE(USB3)
LV_IMG_DECLARE(TOS3)
LV_IMG_DECLARE(COA3)
LV_IMG_DECLARE(HDMI3)
LV_IMG_DECLARE(ENTER3)

LV_IMG_DECLARE(blmax)
LV_IMG_DECLARE(blmin)
LV_IMG_DECLARE(BL0)
LV_IMG_DECLARE(BL1)
LV_IMG_DECLARE(BL2)
LV_IMG_DECLARE(BL3)
LV_IMG_DECLARE(BL4)
LV_IMG_DECLARE(BL5)
LV_IMG_DECLARE(BL6)
LV_IMG_DECLARE(BL7)
LV_IMG_DECLARE(BL8)

LV_IMG_DECLARE(jfu)
LV_IMG_DECLARE(j0)
LV_IMG_DECLARE(j1)
LV_IMG_DECLARE(j2)
LV_IMG_DECLARE(j3)
LV_IMG_DECLARE(j4)
LV_IMG_DECLARE(j5)
LV_IMG_DECLARE(j6)
LV_IMG_DECLARE(j7)
LV_IMG_DECLARE(j8)
LV_IMG_DECLARE(j9)
LV_IMG_DECLARE(mute1)

static lv_style_t style,style2,style3,stylebk,stylesli,stylesli2,stylesli3,stylemenu,stylemenu_p,styletext,styletext_p,styletext2,styletext2_p,scrollstyle;   /* 定义样式变量 */


static void clk_event_cb( lv_event_t *e ){

  lv_obj_t  *target = lv_event_get_target(e);   /* 第一步：获取触发事件的部件 */
	lv_event_code_t  code = lv_event_get_code(e);  /* 第一步：获取事件类型 */
	
    if ( target == mainoj1 && code == LV_EVENT_CLICKED)
     {
		  // lv_group_del(gmain); 
			create_menu_win();
     //create_main_group();
		 //lv_obj_del_async(mainoj);
			
		 //lv_scr_load(scr);
			 
		flag=0;
		soundBflag=0;
		sliflag=0;
     }
		 
}


static void clk1_event_cb( lv_event_t *e )
{
 lv_obj_t  *target = lv_event_get_target(e);
//  lv_event_code_t  code = lv_event_get_code(e);   /* 第一步：获取触发事件的部件 */
//     if(lv_group_get_obj_count(g1)){
//                    lv_group_del(g1);
//                //lv_group_remove_all_objs(g1);
//            }
//        g1 = lv_group_create();//创建组
//        lv_group_set_default(g1);//如果部件已加入默认组，使用这种配置 设置默认组

//        lv_indev_set_group(indev_keypad, g1);//关联按键和组
	nextlock=0,prelock=0;
     if ( target == oj1)  /* 第二步：判断触发事件的部件 */
     {
         show_oj_win1();
         //lv_scr_load(ojw1);
         //lv_scr_load_anim(ojw1,LV_SCR_LOAD_ANIM_NONE,0,0, TRUE);
				  lv_obj_del_async(scr);
     }
     else if ( target == oj2)
     {
            show_oj_win2();
//         lv_scr_load(ojw2);
			 		//lv_obj_del_async(scr);
     }
     else if ( target == oj3){
            show_oj_win3();
//            lv_scr_load(ojw3);
			 		//lv_obj_del_async(scr);
     }
		 else if ( target == oj4){
            show_oj_win4();
//            lv_scr_load(ojw4);
			 		lv_obj_del_async(scr);
     }
		 else if ( target == oj5){
            show_oj_win5();
//            lv_scr_load(ojw5);
			 		lv_obj_del_async(scr);			 
     }
		 else if ( target == oj6){
            show_oj_win6();
//            lv_scr_load(ojw6);
			 		lv_obj_del_async(scr);			 
     }
		 else if ( target == oj7){
            show_oj_win7();
//            lv_scr_load(ojw7);
			 		lv_obj_del_async(scr);			 
     }
		 else if ( target == oj8){
            show_oj_win8();
//            lv_scr_load(ojw8);
			 		lv_obj_del_async(scr);			 
     }
}


static void focus_event_cb( lv_event_t *e )
{
 lv_obj_t  *target = lv_event_get_target(e);   /* 第一步：获取触发事件的部件 */
 lv_event_code_t  code = lv_event_get_code(e);

 if (target == oj1){
		prelock=0;//到头解锁
	  posi=1;
        lv_obj_scroll_to_y(menuoj,0,0);
        lv_img_set_src(ojimg, &back); /* 设置图片源 */
        lv_obj_set_style_text_color(wlbback, lv_color_hex(0x999999), LV_STATE_DEFAULT );
 }
  else if (target == oj2){
		posi=2;
        lv_obj_scroll_to_view(oj2, LV_ANIM_OFF);
 }
  else if (target == oj3){
		posi=3;
        lv_obj_scroll_to_view(oj3, LV_ANIM_OFF);
 }
  else if (target == oj4){
		posi=4;
        lv_obj_scroll_to_view(oj4, LV_ANIM_OFF);
 }
  else if (target == oj5){
		posi=5;
        lv_obj_scroll_to_view(oj5, LV_ANIM_OFF);
 }
  else if (target == oj6){
		posi=6;
        lv_obj_scroll_to_view(oj6, LV_ANIM_OFF);
 }
 else if (target == oj7){
	  posi=7;
			  lv_obj_scroll_to_view(oj7, LV_ANIM_OFF);
 }
  else if (target == oj8){
		nextlock=0;//到底解锁
		posi=8;
        lv_obj_scroll_to_view(oj8, LV_ANIM_OFF);
        lv_img_set_src(ojimg, &back); /* 设置图片源 */
        lv_obj_set_style_text_color(wlbback, lv_color_hex(0x999999), LV_STATE_DEFAULT );
 }
 else if (target == oj9 && code == LV_EVENT_FOCUSED){
	  nextlock=1;//到底锁死
	  posi=9;
			  lv_obj_scroll_to_y(menuoj,355,0);
        lv_img_set_src(ojimg, &back); /* 设置图片源 */
        lv_obj_set_style_text_color(wlbback, lv_color_hex(0x999999), LV_STATE_DEFAULT );
 }
 //一键恢复出厂设置
 if ( target == oj9 && code == LV_EVENT_CLICKED){
  for(value=0;value<17;value++){
    if(value==0)WrBuff[value]=60;
		else if(value==4)WrBuff[value]=6;
    else if(value==5)WrBuff[value]=7;
    else if(value==14)WrBuff[value]=1;
    else if(value==15)WrBuff[value]=12;
    else if(value==16)WrBuff[value]=2;
    else WrBuff[value]=0;}
    flashflaginsel=1;
    posi=0;
    idflag=0;
    PWM_SetHDuty(PWM1, PWM_CH_B,0);//设置PWM电平时间
    swm_delay_ms(1000);
    GPIO_ClrBit(GPIOB, PIN4);//复位单片机
//			start_recovery();
//        create_main_win();
//        lv_scr_load(mainoj);
//			GPIO_SetBit(GPIOM, PIN5);GPIO_SetBit(GPION, PIN0);
     }
}

//返回事件回调
static void event_cbback( lv_event_t *e )
{
 lv_event_code_t  code = lv_event_get_code(e);  /* 第一步：获取事件类型 */
 lv_obj_t  *target = lv_event_get_target(e);   /* 第一步：获取触发事件的部件 */


       // lv_obj_set_size(lv_obj_get_child(target, 0),200,100);
 if ( target == wbt && code == LV_EVENT_FOCUSED ){
				prelock=1;//到头锁死
				nextlock=0;//到底解锁
        posi=0;
        lv_img_set_src(ojimg, &back_p); /* 设置图片源 */
        lv_obj_set_style_text_color(wlbback, lv_color_hex(0x00A0E9), LV_STATE_DEFAULT );
 }
 
 if ( target == wbt && code == LV_EVENT_CLICKED && levelflag==2)  /* 第二步：判断事件类型 */
 {
				
				posi=0;
				idflag=0;
				//lv_group_del(g);
				//lv_obj_del_async(scr);
        create_main_win();
	 			lv_obj_del_async(scr);
				
        //lv_scr_load(mainoj);
	 			//lv_obj_del(scr);
				
	 //lv_scr_load_anim(scr,LV_SCR_LOAD_ANIM_FADE_ON,0,0, true);
 }
 if ( target == wbt && code == LV_EVENT_CLICKED && levelflag==3)  /* 第二步：判断事件类型 */
 {
	 	 if(idflag==1){lv_obj_del_async(ojw1);}
		 else if(idflag==2){lv_obj_del_async(ojw2);}
		 else if(idflag==3){lv_obj_del_async(ojw3);}
		 else if(idflag==4){lv_obj_del_async(ojw4);}
		 else if(idflag==5){lv_obj_del_async(ojw5);}
		 else if(idflag==6){lv_obj_del_async(ojw6);}
		 else if(idflag==7){lv_obj_del_async(ojw7);}
		 else if(idflag==8){lv_obj_del_async(ojw8);}
     create_menu_win();
     //create_main_group();
		 //lv_scr_load(scr);

 }

}


//w 输入选择事件
static void w1_event_cb( lv_event_t *e )
{

 lv_event_code_t  code = lv_event_get_code(e);  /* 第一步：获取事件类型 */
 lv_obj_t  *target = lv_event_get_target(e);   /* 第一步：获取触发事件的部件 */

  if ( target == w1bt1 && code == LV_EVENT_FOCUSED ) {
          prelock=0;//到头解锁
          posi=1;
          lv_obj_scroll_to_y(menuoj,0,0);
          lv_img_set_src(ojimg, &back); /* 设置图片源 */
          lv_obj_set_style_text_color(wlbback, lv_color_hex(0x999999), LV_STATE_DEFAULT );
  }

  if ( target == w1bt2 && code == LV_EVENT_FOCUSED ) {
          posi=2;
          lv_obj_scroll_to_view(w1bt2, LV_ANIM_OFF);
  }
  if ( target == w1bt3 && code == LV_EVENT_FOCUSED ) {
          nextlock=0;//到底解锁
          posi=3;
          lv_obj_scroll_to_view(w1bt3, LV_ANIM_OFF);
  }
  if ( target == w1bt4 && code == LV_EVENT_FOCUSED ) {
          nextlock=1;//到底锁死
          posi=4;
          lv_obj_scroll_to_view(w1bt4, LV_ANIM_OFF);
          lv_img_set_src(ojimg, &back); /* 设置图片源 */
          lv_obj_set_style_text_color(wlbback, lv_color_hex(0x999999), LV_STATE_DEFAULT );
  }
  if(code == LV_EVENT_CLICKED){
			flashflaginsel=1;
//USB输入
  if ( target == w1bt1){
  lv_obj_set_style_img_opa(ojenter1img,LV_OPA_100,0);
  lv_obj_set_style_img_opa(ojenter2img,LV_OPA_0,0);
  lv_obj_set_style_img_opa(ojenter3img,LV_OPA_0,0);
  lv_obj_set_style_img_opa(ojenter4img,LV_OPA_0,0);
	LIN0;LIN1;//00 usb(xmos)
	WrBuff[1]=GET_IN0,WrBuff[2]=GET_IN1;
	insel=1;
	WrBuff[3]=insel;		

 }
 //同轴输入
  if ( target == w1bt2)
 {
  lv_obj_set_style_img_opa(ojenter2img,LV_OPA_100,0);
  lv_obj_set_style_img_opa(ojenter1img,LV_OPA_0,0);
  lv_obj_set_style_img_opa(ojenter3img,LV_OPA_0,0);
  lv_obj_set_style_img_opa(ojenter4img,LV_OPA_0,0);
	HIN0;LIN1;//10 cs8422
	WrBuff[1]=GET_IN0,WrBuff[2]=GET_IN1;
  write_CS8422_regs(0x03,0xC4);//8422同轴输入
//	 	if(read_CS8422_regs(0x03,&debug_read)==1)
//		lv_label_set_text_fmt(lb,"40x%02x\n", debug_read);//把读出的寄存器值写在屏幕上
	insel=8;
	WrBuff[3]=insel;


 }

//光纤输入
  if ( target == w1bt3)
 {
  lv_obj_set_style_img_opa(ojenter3img,LV_OPA_100,0);
  lv_obj_set_style_img_opa(ojenter2img,LV_OPA_0,0);
  lv_obj_set_style_img_opa(ojenter1img,LV_OPA_0,0);
  lv_obj_set_style_img_opa(ojenter4img,LV_OPA_0,0);
	HIN0;LIN1;//10 cs8422
	WrBuff[1]=GET_IN0,WrBuff[2]=GET_IN1;
  write_CS8422_regs(0x03,0x84);//8422光纤输入
//		if(read_CS8422_regs(0x03,&debug_read)==1)
//		lv_label_set_text_fmt(lb,"40x%02x\n", debug_read);//把读出的寄存器值写在屏幕上	 
	insel=16;
	WrBuff[3]=insel;


 }
//HDMI输入
  if ( target == w1bt4)
    {
    lv_obj_set_style_img_opa(ojenter4img,LV_OPA_100,0);
    lv_obj_set_style_img_opa(ojenter1img,LV_OPA_0,0);
    lv_obj_set_style_img_opa(ojenter3img,LV_OPA_0,0);
    lv_obj_set_style_img_opa(ojenter2img,LV_OPA_0,0);
      LIN0;HIN1;//01 hdmi
      WrBuff[1]=GET_IN0,WrBuff[2]=GET_IN1;


    }

  }
}

//w2 亮度与主题时间事件回调
static void w2_event_cb( lv_event_t *e )
{

 lv_event_code_t  code = lv_event_get_code(e);  /* 第一步：获取事件类型 */
 lv_obj_t  *target = lv_event_get_target(e);   /* 第一步：获取触发事件的部件 */
     if(target== w2bt1&&code == LV_EVENT_CLICKED)
    {
        lv_obj_clear_state(w2bt1,LV_STATE_ANY);
        lv_obj_add_state(w2bt1,LV_STATE_EDITED);
				//lv_obj_add_state(w2bt1,LV_STATE_CHECKED);
		}

		if(lv_obj_get_state(w2bt1)==LV_STATE_EDITED){
                flag=1;
			          sliflag=1;
			}
	  if ( target == w2bt2 && code == LV_EVENT_CLICKED )
 {
     if(lv_obj_has_state(w2switch, LV_STATE_CHECKED)){
			 WrBuff[14]=0;
     lv_obj_clear_state(w2switch, LV_STATE_CHECKED);
		 
		 }
     else {
			 WrBuff[14]=1;
			 lv_obj_add_state(w2switch, LV_STATE_CHECKED);
		 
		 }
		 flashflaginsel=1;

 }
	  if(target== w2bt3&&code == LV_EVENT_CLICKED)
    {
        lv_obj_clear_state(w2bt3,LV_STATE_ANY);
        lv_obj_add_state(w2bt3,LV_STATE_EDITED);
				//lv_obj_add_state(w2bt3,LV_STATE_CHECKED);
		}

		if(lv_obj_get_state(w2bt3)==LV_STATE_EDITED){
                flag=1;
			          sliflag=5;
			}
		     if(target== w2bt4&&code == LV_EVENT_CLICKED)
    {
        lv_obj_clear_state(w2bt4,LV_STATE_ANY);
        lv_obj_add_state(w2bt4,LV_STATE_EDITED);
				//lv_obj_add_state(w2bt5,LV_STATE_CHECKED);
		}

		if(lv_obj_get_state(w2bt4)==LV_STATE_EDITED){
                flag=1;
			          sliflag=6;
			}
    if ( target == w2bt1 && code == LV_EVENT_FOCUSED ) {
			prelock=0;//到头解锁
            posi=1;
            lv_obj_scroll_to_y(oj,0,0);
            lv_img_set_src(ojimg, &back); /* 设置图片源 */
            lv_obj_set_style_text_color(wlbback, lv_color_hex(0x999999), LV_STATE_DEFAULT );
    }

    if ( target == w2bt2 && code == LV_EVENT_FOCUSED ) {
            posi=2;
            lv_obj_scroll_to_y(oj,193,0);
    }

    if ( target == w2bt3 && code == LV_EVENT_FOCUSED ) {
			nextlock=0;//到底解锁
            posi=3;
            lv_obj_scroll_to_y(oj,218,0);

    }
		if ( target == w2bt4 && code == LV_EVENT_FOCUSED ) {
			nextlock=1;//到底锁死
			lv_img_set_src(ojimg, &back); /* 设置图片源 */
			lv_obj_set_style_text_color(wlbback, lv_color_hex(0x999999), LV_STATE_DEFAULT );
            posi=4;
            lv_obj_scroll_to_y(oj,228,0);

    }
//    if ( target == w2bt5 && code == LV_EVENT_FOCUSED ) {
//						nextlock=0;//到底解锁
//            posi=5;
//            lv_obj_scroll_to_y(oj,460,0);

//    }
//    if ( target == w2bt6 && code == LV_EVENT_FOCUSED ) {
//						nextlock=1;//到底锁死
//            posi=6;
//            lv_obj_scroll_to_y(oj,460,0);
//            lv_img_set_src(ojimg, &back); /* 设置图片源 */
//            lv_obj_set_style_text_color(wlbback, lv_color_hex(0x999999), LV_STATE_DEFAULT );

//    }
}

//w3 9039MPRO设置/本地全局飞秒时钟事件/左右输出呼唤
static void w3_event_cb( lv_event_t *e )
{
 lv_event_code_t  code = lv_event_get_code(e);  /* 第一步：获取事件类型 */
 lv_obj_t  *target = lv_event_get_target(e);   /* 第一步：获取触发事件的部件 */

if ( code == LV_EVENT_FOCUSED ){
//    if ( target == w3bt1  ) {
//			prelock=0;//到头解锁
//            posi=1;
//            lv_obj_scroll_to_y(oj,0,0);
//            lv_img_set_src(ojimg, &back); /* 设置图片源 */
//            lv_obj_set_style_text_color(wlbback, lv_color_hex(0x999999), LV_STATE_DEFAULT );
//    }
//	 	if(read_9039m_regs(0x01,&debug_read)==1)
//		lv_label_set_text_fmt(lb,"%02x\n", debug_read);//把读出的寄存器值写在屏幕上
		//lv_label_set_text_fmt(wlb1,"%d-%d-%d", WrBuff[9],WrBuff[10],WrBuff[11]);//把读出的寄存器值写在屏幕上	
    if ( target == w3bt2 ) {
				  prelock=0;//到头解锁
            posi=2;
            lv_obj_scroll_to_y(oj,0,0);
            lv_img_set_src(ojimg, &back); /* 设置图片源 */
            lv_obj_set_style_text_color(wlbback, lv_color_hex(0x999999), LV_STATE_DEFAULT );
    }
    if ( target == w3bt3 ) {
            posi=3;
            lv_obj_scroll_to_y(oj,188,0);
    }
    if ( target == w3bt4 ) {
            posi=4;
            lv_obj_scroll_to_y(oj,384,0);
    }
    if ( target == w3bt5 ) {
						nextlock=0;//到底解锁
            posi=5;
            lv_obj_scroll_to_y(oj,384,0);
            lv_img_set_src(ojimg, &back); /* 设置图片源 */
            lv_obj_set_style_text_color(wlbback, lv_color_hex(0x999999), LV_STATE_DEFAULT );
    }
		if ( target == w3bt6 ) {
						nextlock=1;//到底锁死
						posi=6;
						lv_obj_scroll_to_y(oj,572,0);
						lv_img_set_src(ojimg, &back); /* 设置图片源 */
						lv_obj_set_style_text_color(wlbback, lv_color_hex(0x999999), LV_STATE_DEFAULT );

    }
}	


if ( code == LV_EVENT_CLICKED ){
	
//	 if ( target == w3bt1 )
//	 {
//			 if(lv_obj_has_state(w3switch1, LV_STATE_CHECKED)){
//			 lv_obj_clear_state(w3switch1, LV_STATE_CHECKED);
//			 }
//			 else 
//			 {
//				lv_obj_add_state(w3switch1, LV_STATE_CHECKED);
//			 }
//	 }
  //ASYNC模式设置、设置9039MPRO是否启用异步模式
	 if ( target == w3bt2 )
	 {
      if(lv_obj_has_state(w3switch2, LV_STATE_CHECKED)){
        lv_obj_clear_state(w3switch2, LV_STATE_CHECKED);
        read_9039m_regs(0x01,&FIR_value);
        FIR_value &=(~0x40);//把0-2Bit位清0，其他位不变
        FIR_value |=0x40; 
        WrBuff[8]=1;
        write_9039m_regs(0x01, FIR_value);
			 }
      else {
        lv_obj_add_state(w3switch2, LV_STATE_CHECKED);
        read_9039m_regs(0x01,&FIR_value);
        FIR_value &=(~0x40);//把0-2Bit位清0，其他位不变
        FIR_value |=0;
        WrBuff[8]=0;
        write_9039m_regs(0x01, FIR_value);
			 }
	 }
  //ASYNC模式设置、设置9039MPRO是否启用16xIIR滤波器
	 if ( target == w3bt3 )
	 {
      if(lv_obj_has_state(w3switch3, LV_STATE_CHECKED)){
        lv_obj_clear_state(w3switch3, LV_STATE_CHECKED);
        read_9039m_regs(0x5A,&FIR_value);
        FIR_value &=(~0x04);//把2Bit位清0，其他位不变
        FIR_value |=4;
        write_9039m_regs(0x5A, FIR_value);
			 }
      else {
        lv_obj_add_state(w3switch3, LV_STATE_CHECKED);
        read_9039m_regs(0x5A,&FIR_value);
        FIR_value &=(~0x04);//把2Bit位清0，其他位不变
        FIR_value |=0;
        write_9039m_regs(0x5A, FIR_value);
			 }
	 }
  //SOF超采样滤波器、2xOSF
	 if ( target == w3bt4 )
	 {
			 if(lv_obj_has_state(w3switch4, LV_STATE_CHECKED)){
			 lv_obj_clear_state(w3switch4, LV_STATE_CHECKED);
					read_9039m_regs(0x5A,&FIR_value);
					FIR_value &=(~0x01);//把0Bit位清0，其他位不变
					FIR_value |=1;
					write_9039m_regs(0x5A, FIR_value);			 
			 }
			 else {
				 lv_obj_add_state(w3switch4, LV_STATE_CHECKED);
					read_9039m_regs(0x5A,&FIR_value);
					FIR_value &=(~0x01);//把0Bit位清0，其他位不变
					FIR_value |=0; 
					write_9039m_regs(0x5A, FIR_value);
			 }

	 }
  //SOF超采样滤波器、4xOSF
	 if ( target == w3bt5 )
	 {
			 if(lv_obj_has_state(w3switch5, LV_STATE_CHECKED)){
			 lv_obj_clear_state(w3switch5, LV_STATE_CHECKED);
					read_9039m_regs(0x5A,&FIR_value);
					FIR_value &=(~0x02);//把1Bit位清0，其他位不变
					FIR_value |=2; 
					write_9039m_regs(0x5A, FIR_value);
			 }
			 else {
				 lv_obj_add_state(w3switch5, LV_STATE_CHECKED);
					read_9039m_regs(0x5A,&FIR_value);
					FIR_value &=(~0x02);//把1Bit位清0，其他位不变
					FIR_value |=0;
				 write_9039m_regs(0x5A, FIR_value);
			 }

	 }
   //相位反转开关、设置9039MPRO是否启用相位反转
	  if ( target == w3bt6 )
 {
     if(lv_obj_has_state(w3switch6, LV_STATE_CHECKED)){
     lv_obj_clear_state(w3switch6, LV_STATE_CHECKED);
				WrBuff[13]=0;
				write_9039m_regs(0x57, 0x00);//反转所有声道输出
		 }
     else {
			lv_obj_add_state(w3switch6, LV_STATE_CHECKED);
				WrBuff[13]=1;
				write_9039m_regs(0x57, 0xFF);//反转所有声道输出
		 }

 }
	 
				read_9039m_regs(0x5A,&x5A);
				swm_delay_ms(5);
				WrBuff[9]=x5A;
				flashflaglvbo=1;
}

 /* w4 本地全局飞秒时钟 */
     if ( target == w4bt1 && code == LV_EVENT_FOCUSED ) {
			 prelock=0;//到头解锁
			 nextlock=1;//到底锁死
            posi=1;
            lv_img_set_src(ojimg, &back); /* 设置图片源 */
            lv_obj_set_style_text_color(wlbback, lv_color_hex(0x999999), LV_STATE_DEFAULT );
    }
     if ( target == w4bt1 && code == LV_EVENT_CLICKED )
 {
     if(lv_obj_has_state(w4switch1, LV_STATE_CHECKED)){
			 lv_obj_clear_state(w4switch1, LV_STATE_CHECKED);
			 write_CS8422_regs(0x0A, 0x52);
			 WrBuff[10]=1;
		 }
     else {
			 lv_obj_add_state(w4switch1, LV_STATE_CHECKED);
			 write_CS8422_regs(0x0A, 0x02);
			 WrBuff[10]=0;
		 }
		 
			flashflaglvbo=1;
 }

 /* w7 左右输出互换 */
     if ( target == w7bt1 && code == LV_EVENT_FOCUSED ) {
			 prelock=0;//到头解锁
			 nextlock=1;//到底锁死
            posi=1;
            lv_img_set_src(ojimg, &back); /* 设置图片源 */
            lv_obj_set_style_text_color(wlbback, lv_color_hex(0x999999), LV_STATE_DEFAULT );
    }
     if ( target == w7bt1 && code == LV_EVENT_CLICKED ){
     if(lv_obj_has_state(w7switch1, LV_STATE_CHECKED)){
      lv_obj_clear_state(w7switch1, LV_STATE_CHECKED);
			WrBuff[11]=0;
			 
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
		 }
     else {
			 lv_obj_add_state(w7switch1, LV_STATE_CHECKED);
			 WrBuff[11]=1;
			 
			  write_9039m_regs(0x40, 0x01);//TDM CH1配置,选择TDM中的L声道
				swm_delay_ms(5);
				write_9039m_regs(0x41, 0x00);//TDM CH2配置,选择TDM中的R声道
				swm_delay_ms(5);
				write_9039m_regs(0x42, 0x01);//TDM CH3配置,选择TDM中的L声道
				swm_delay_ms(5);
				write_9039m_regs(0x43, 0x00);//TDM CH4配置,选择TDM中的R声道
				swm_delay_ms(5);
				write_9039m_regs(0x44, 0x01);//TDM CH5配置,选择TDM中的L声道
				swm_delay_ms(5);
				write_9039m_regs(0x45, 0x00);//TDM CH6配置,选择TDM中的R声道
				swm_delay_ms(5);
				write_9039m_regs(0x46, 0x01);//TDM CH7配置,选择TDM中的L声道
				swm_delay_ms(5);
				write_9039m_regs(0x47, 0x00);//TDM CH8配置,选择TDM中的R声道
				swm_delay_ms(5);
		 }
flashflaglvbo=1;
 }
}

//W5 滤波事件回调
static void w5_event_cb( lv_event_t *e ){
	  lv_event_code_t  code = lv_event_get_code(e);  /* 第一步：获取事件类型 */
		lv_obj_t  *target = lv_event_get_target(e);   /* 第一步：获取触发事件的部件 */
    if ( target == w5bt1 && code == LV_EVENT_FOCUSED ) {
        prelock=0;//到头解锁
        nextlock=1;//到底锁死
        posi=1;
        //lv_img_set_src(ojimg, &back); /* 设置图片源 */
        lv_obj_set_style_text_color(wlbback, lv_color_hex(0x999999), LV_STATE_DEFAULT );
    }
    if ( target == w5bt1 && code == LV_EVENT_CLICKED ){
        lv_obj_clear_state(w5bt1,LV_STATE_ANY);
        lv_obj_add_state(w5bt1,LV_STATE_EDITED);
    }
 		if ( lv_obj_get_state(w5bt1) == LV_STATE_EDITED){
        flag=1;
        sliflag=4;
		}
}

//W6 平衡调节
static void w6_event_cb( lv_event_t *e )
{
    lv_event_code_t  code = lv_event_get_code(e);  /* 第一步：获取事件类型 */
     lv_obj_t  *target = lv_event_get_target(e);   /* 第一步：获取触发事件的部件 */
	
		if ( target == w6bt1 && code == LV_EVENT_FOCUSED ) {
		prelock=0;//到头解锁
		nextlock=0;//到底解锁
            posi=1;
            lv_img_set_src(ojimg, &back); /* 设置图片源 */
            lv_obj_set_style_text_color(wlbback, lv_color_hex(0x999999), LV_STATE_DEFAULT );

    }
        if ( target == w6bt2 && code == LV_EVENT_FOCUSED ) {
					nextlock=1;//到底锁死
            posi=2;
            lv_img_set_src(ojimg, &back); /* 设置图片源 */
            lv_obj_set_style_text_color(wlbback, lv_color_hex(0x999999), LV_STATE_DEFAULT );

    }
	
       if(code == LV_EVENT_CLICKED)
    {
			if(target== w6bt1){//左平衡
        lv_obj_clear_state(w6bt1,LV_STATE_ANY);
        lv_obj_add_state(w6bt1,LV_STATE_EDITED);
			}	
      if(target== w6bt2){//右平衡
        lv_obj_clear_state(w6bt2,LV_STATE_ANY);
        lv_obj_add_state(w6bt2,LV_STATE_EDITED);
			}
				//lv_obj_add_state(w2bt1,LV_STATE_CHECKED);
		}

		if(lv_obj_get_state(w6bt1)==LV_STATE_EDITED){
                flag=1;
			          sliflag=2;
			}  


		if(lv_obj_get_state(w6bt2)==LV_STATE_EDITED){
                flag=1;
			          sliflag=3;
			}  
	

}
//W8 输出选择事件回调
static void w8_event_cb( lv_event_t *e )
{
    lv_event_code_t  code = lv_event_get_code(e);  /* 第一步：获取事件类型 */
     lv_obj_t  *target = lv_event_get_target(e);   /* 第一步：获取触发事件的部件 */
    if ( target == w8bt1 && code == LV_EVENT_FOCUSED ) {
					  prelock=0;//到头解锁
					  nextlock=0;//到底解锁
            posi=1;
            lv_img_set_src(ojimg, &back); /* 设置图片源 */
            lv_obj_set_style_text_color(wlbback, lv_color_hex(0x999999), LV_STATE_DEFAULT );

    }
    if ( target == w8bt2 && code == LV_EVENT_FOCUSED ) {
						nextlock=1;//到底锁死
            posi=2;
            lv_img_set_src(ojimg, &back); /* 设置图片源 */
            lv_obj_set_style_text_color(wlbback, lv_color_hex(0x999999), LV_STATE_DEFAULT );

    }
    if(code == LV_EVENT_CLICKED){//耳放输出
      if ( target == w8bt1 )
      {
      if(lv_obj_has_state(w8switch1, LV_STATE_CHECKED)){
        lv_obj_clear_state(w8switch1, LV_STATE_CHECKED);
        GPIO_ClrBit(GPIOM, PIN5);//物理静音2  1是开启声音 GPIO_SetBit  0是声音关 GPIO_ClrBit
      }
      else {
        lv_obj_add_state(w8switch1, LV_STATE_CHECKED);
        GPIO_SetBit(GPIOM, PIN5);//物理静音2  1是开启声音 GPIO_SetBit  0是声音关 GPIO_ClrBit
      }
      }

     if ( target == w8bt2)//RCA-XLR输出
     {
      if(lv_obj_has_state(w8switch2, LV_STATE_CHECKED)){
        lv_obj_clear_state(w8switch2, LV_STATE_CHECKED);
        GPIO_ClrBit(GPION, PIN0);//物理静音  1是开启声音 GPIO_SetBit  0是声音关 GPIO_ClrBit
      }
      else {
        lv_obj_add_state(w8switch2, LV_STATE_CHECKED);
        GPIO_SetBit(GPION, PIN0);//物理静音  1是开启声音 GPIO_SetBit  0是声音关 GPIO_ClrBit
      }
     }

      outgpio=2*GPIO_GetBit(GPIOM, PIN5)+GPIO_GetBit(GPION, PIN0);
      if(outgpio==0)WrBuff[12]=3;//0 0
      else if(outgpio==1)WrBuff[12]=2;//0 1
      else if(outgpio==2)WrBuff[12]=1;//1 0
      else if(outgpio==3)WrBuff[12]=0;//1 1
      flashflaglvbo=1;
    }
}
/**
 * @brief  LVGL演示
 * @param  无
 * @return 无
 */

void my_gui(void)
{

/**********       样式                           */


    lv_style_init(&style);  /* 按钮样式 */
    lv_style_set_bg_opa(&style,LV_OPA_0);
    lv_style_set_border_opa(&style,LV_OPA_0);

    lv_style_init(&style2);  /* 按钮聚焦样式 */
    lv_style_set_bg_color(&style2, lv_color_hex(0x00A0E9));  /* 设置背景颜色 */
    lv_style_set_border_color(&style2,lv_color_hex(0x00A0E9)); //边框颜色
    //lv_style_set_shadow_opa(&style2,LV_OPA_0);
		lv_style_set_border_opa(&style2,LV_OPA_100);

    lv_style_init(&style3);  /* 按钮点击样式 */
    lv_style_set_bg_color(&style3, lv_color_hex(0xFF6633));  /* 设置背景颜色 */
    //lv_style_set_border_side(&style3, LV_BORDER_SIDE_BOTTOM);
    lv_style_set_border_color(&style3,lv_color_hex(0xFF6633)); //边框颜色
    lv_style_set_shadow_opa(&style3,LV_OPA_0);

		lv_style_init(&styletext);/* 标题样式 */
    lv_style_set_text_color(&styletext,lv_color_hex(0xFFFFFF));

    lv_style_init(&styletext2);/* 二级标题样式 */
    lv_style_set_text_color(&styletext2,lv_color_hex(0x999999));
	
	
    lv_style_init(&stylebk);  /* 背景样式 */
    lv_style_set_bg_color(&stylebk, lv_color_hex(0x000000));  /* 设置背景颜色 */
    lv_style_set_border_color(&stylebk,lv_color_hex(0x000000)); //边框颜色
    	 lv_obj_add_style(lv_scr_act(), & stylebk, LV_STATE_DEFAULT);
			 
    lv_style_init(&scrollstyle);/* 滚动条样式 */
    lv_style_set_bg_color(&scrollstyle, lv_color_hex(0xFFFFFF));
    lv_style_set_bg_opa(&scrollstyle,LV_OPA_80);
		lv_style_set_width(&scrollstyle, 12);
		
    lv_style_init(&stylesli2);  /* 滑块聚焦样式 */
    lv_style_set_border_color(&stylesli2,lv_color_hex(0x00A0E9)); //边框颜色0x99CC00

    lv_style_init(&stylesli3);  /* 滑块编辑样式 */
    lv_style_set_border_color(&stylesli3,lv_color_hex(0x99CC00)); //边框颜色0x99CC00
		lv_style_set_border_opa(&stylesli3,LV_OPA_100);

    lv_style_init(&stylemenu); /* 菜单样式 */
    lv_style_set_border_color(&stylemenu, lv_color_hex(0x000000));
    lv_style_set_bg_opa(&stylemenu, LV_OPA_0);

    lv_style_init(&stylemenu_p); /* 菜单点击样式 */
    lv_style_set_bg_color(&stylemenu_p, lv_color_hex(0x00CCFF));
    lv_style_set_border_color(&stylemenu_p, lv_color_hex(0x000000));
    lv_style_set_bg_opa(&stylemenu_p, LV_OPA_100);

    WrBuff[18]=*((volatile uint32_t *)(EEPROM_ADDR + 18*4));
    if(WrBuff[18]==-1)WrBuff[18]=0x10;

/**********       主屏幕（1）                           */
if(WrBuff[18]==0x10)//levelflag = 1 idflag = 0
  create_main_win();
//开机重启运行		
if(start){
  start_recovery();					
  start=0;//上电做一次		
}
if(WrBuff[18]==0x20)//levelflag = 2 idflag = 0
  create_menu_win();
else if(WrBuff[18]>0x30){//levelflag = 3 idflag = 0
if(WrBuff[18]==0x31)show_oj_win1();//levelflag = 3 idflag = 1
else if(WrBuff[18]==0x32)show_oj_win2();//levelflag = 3 idflag = 2
else if(WrBuff[18]==0x33)show_oj_win3();//levelflag = 3 idflag = 3
else if(WrBuff[18]==0x34)show_oj_win4();//levelflag = 3 idflag = 4
else if(WrBuff[18]==0x35)show_oj_win5();//levelflag = 3 idflag = 5
else if(WrBuff[18]==0x36)show_oj_win6();//levelflag = 3 idflag = 6
else if(WrBuff[18]==0x37)show_oj_win7();//levelflag = 3 idflag = 7
else if(WrBuff[18]==0x38)show_oj_win8();//levelflag = 3 idflag = 8
	
	WrBuff[18]=0x10;
	FLASH_Erase(EEPROM_ADDR);
	FLASH_Write(EEPROM_ADDR, WrBuff, 20);
}
else create_main_win();

    /**********       菜单（2）                           */
    //create_menu_win();

        /**********       附屏幕（3）                           */
        //show_oj_win1();
        //show_oj_win2();
        //show_oj_win3();
        //show_oj_win4();
        //show_oj_win5();


/**********       组                           */
//create_main_group();
}









/**********       创建主屏幕（1）                           */
void create_main_win(){
						nextlock=0,prelock=0,levelflag=1;
						WrBuff[18]=0x10;
						frenum=0;
//						printf("%d\n",levelflag);
            mainoj = lv_obj_create(NULL);
            lv_scr_load(mainoj);
            //lv_obj_add_style(mainoj, & stylebk, LV_STATE_DEFAULT);

	
            /*大按键*/
            mainoj1= lv_obj_create(mainoj);
            lv_obj_set_style_radius(mainoj1,0,0);//设置容器圆角为直角
            lv_obj_set_size(mainoj1,scr_act_width(), scr_act_height());//在主屏幕下创建一个等大的对象屏幕
            lv_obj_add_style(mainoj1, & stylebk, LV_STATE_DEFAULT);
            lv_obj_center(mainoj1);
            lv_obj_add_event_cb(mainoj1, clk_event_cb, LV_EVENT_ALL, NULL);//添加事件
            //lv_obj_set_style_bg_color(mainoj1, lv_color_hex(0xf4b183),LV_STATE_FOCUSED);
					
//						imgtest=lv_img_create(mainoj1);
//	          lv_obj_set_size(imgtest,800, 268);
//						lv_obj_center(imgtest);
//						lv_img_set_src(imgtest, &imgtest1); /* 设置图片源 */
            /* 线条 */
            line=lv_obj_create(mainoj1);
            lv_obj_set_size(line,scr_act_width(),2);
            lv_obj_align_to(line, mainoj1,LV_ALIGN_CENTER, 0, 65);
						lv_obj_set_style_opa(line,LV_OPA_40,LV_STATE_DEFAULT);
            lv_obj_set_style_radius(line, 0,0);

            /* 对齐OJ1 */
            alignoj1=lv_obj_create(mainoj1);
            lv_obj_set_size(alignoj1,10,10);
            lv_obj_set_style_radius(alignoj1, 1,0);
            lv_obj_align_to(alignoj1, mainoj1,LV_ALIGN_CENTER, -115, -10);
            lv_obj_add_style(alignoj1,&stylemenu,0);//透明对齐点

            /* PCM/DSD */
            mainlb5 = lv_label_create(mainoj1);
            lv_obj_set_style_text_font(mainlb5,&lv_font_palatino_39_num,0);
            lv_obj_set_style_text_color(mainlb5, lv_color_hex(0xFFFFFF), LV_STATE_DEFAULT );
            lv_obj_set_style_text_opa(mainlb5,LV_OPA_100,LV_STATE_DEFAULT);
            lv_label_set_recolor(mainlb5, true );  /* 开启重新着色功能 */
            

            /* KHZ */
            mainlb1 = lv_label_create(mainoj1);
            lv_obj_set_style_text_font(mainlb1,&lv_font_palatino_68_num,0);
            lv_obj_set_style_text_color(mainlb1, lv_color_hex(0xFFFFFF), LV_STATE_DEFAULT );
            lv_obj_set_style_text_opa(mainlb1,LV_OPA_100,LV_STATE_DEFAULT);
            //lv_label_set_recolor(mainlb1, true );  /* 开启重新着色功能 */
            //lv_label_set_text_fmt(mainlb1, "#3366FF %.1f#",samfre);
            lv_obj_align_to(mainlb1, alignoj1, LV_ALIGN_CENTER, 0, 0);
            
//            mainlba = lv_label_create(mainoj1);
//            lv_obj_set_style_text_font(mainlba,FONT,0);
//            lv_obj_set_style_text_color(mainlba, lv_color_hex(0xFFFFFF), LV_STATE_DEFAULT );
            //lv_label_set_text_fmt(mainlba, "KHZ");
							sam_fre_chose();//显示mainlb1,mainlb5
//						lv_obj_align_to(mainlba, mainlb1, LV_ALIGN_BOTTOM_RIGHT,  110, 10);//KHZ对齐

						 /* 主界面db音量部件 */
            alignoj2=lv_obj_create(mainoj1);
            lv_obj_set_size(alignoj2,10,10);
            lv_obj_set_style_radius(alignoj2, 1,0);
            lv_obj_align_to(alignoj2, alignoj1,LV_ALIGN_CENTER, 450, 0);
            lv_obj_add_style(alignoj2,&stylemenu,0);//透明对齐点
            /* dB */
            /* 个位图片 */
            mainge= lv_img_create(mainoj1);
            //lv_img_set_src(mainge, &j9); /* 设置图片源 */
            lv_obj_align_to(mainge, alignoj2, LV_ALIGN_RIGHT_MID, -100, -70);
            /* 十位图片 */
            mainshi= lv_img_create(mainoj1);
            //lv_img_set_src(mainshi, &j9); /* 设置图片源 */
            lv_obj_align_to(mainshi, alignoj2, LV_ALIGN_RIGHT_MID, -170, -70);
            /* 负号图片 */
            mainfu= lv_img_create(mainoj1);
            lv_img_set_src(mainfu, &jfu); /* 设置图片源 */
            lv_obj_align_to(mainfu, alignoj2, LV_ALIGN_RIGHT_MID, -165, -10);

//            lv_label_set_recolor(mainlb2, true );  /* 开启重新着色功能 */
						soundB=*((volatile uint32_t *)(EEPROM_ADDR + 0*4));
						soundBLtemp=*((volatile uint32_t *)(EEPROM_ADDR + 6*4));
						soundBRtemp=*((volatile uint32_t *)(EEPROM_ADDR + 7*4));
						if(soundBLtemp==-1)soundBLtemp=0;
            if(soundBRtemp==-1)soundBRtemp=0;
            if(soundB==-1){soundB=0;}
            if(soundB>=188&&soundB<=252){
              switch_num_shi(89+(soundB-180)/8);
              switch_num_ge(89+(soundB-180)/8);
            }
            else if(soundB==255){
              switch_num_shi(99);
              switch_num_ge(99);
            }
            else {
              switch_num_shi(soundB/2);
              switch_num_ge(soundB/2);
            }
            if(soundB==0)lv_obj_set_style_img_opa(mainfu,LV_OPA_0,0);
            if(soundB==2)lv_obj_set_style_img_opa(mainfu,LV_OPA_100,0);
							
						
						soundBL=soundB+soundBLtemp,soundBR=soundB+soundBRtemp;
						if(soundBL>255){soundBL=255;}//超出矫正soundBLtemp=255-soundB;
						if(soundBR>255){soundBR=255;}//超出矫正soundBRtemp=255-soundB;
            WrBuff[0]=soundB;//存 写入flash数组 的数据，进入主界面的数据包
						WrBuff[6]=soundBLtemp,WrBuff[7]=soundBRtemp;

            lb1 = lv_label_create(mainoj1);
            lv_obj_set_style_text_font(lb1,&lv_font_palatino_40_num,0);
            lv_obj_set_style_text_color(lb1, lv_color_hex(0xFFFFFF), LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(lb1,LV_OPA_100,LV_STATE_DEFAULT);
            lv_label_set_text_fmt(lb1, "dB");
            lv_obj_align_to(lb1, alignoj2, LV_ALIGN_RIGHT_MID,  25, 15);

						/* 静音标志 */
            mainmute=lv_img_create(mainoj1);
            lv_img_set_src(mainmute, &mute1); /* 设置图片源 */
            lv_obj_align_to(mainmute, alignoj2, LV_ALIGN_RIGHT_MID, 30, -10);
						if(!GPIO_GetBit(GPIOM, PIN5)&&!GPIO_GetBit(GPION, PIN0))
						{
							lv_obj_set_style_img_opa(mainmute,LV_OPA_100,0);
							lv_obj_set_style_img_opa(mainfu,LV_OPA_0,0);
							lv_obj_set_style_img_opa(mainge,LV_OPA_0,0);
							lv_obj_set_style_img_opa(mainshi,LV_OPA_0,0);
							lv_obj_set_style_text_opa(lb1,LV_OPA_0,0);
						}
						else{
						lv_obj_set_style_img_opa(mainmute,LV_OPA_0,0);
						if(soundB!=0)
						lv_obj_set_style_img_opa(mainfu,LV_OPA_100,0);
						lv_obj_set_style_img_opa(mainge,LV_OPA_100,0);
						lv_obj_set_style_img_opa(mainshi,LV_OPA_100,0);
						lv_obj_set_style_text_opa(lb1,LV_OPA_100,0);
						}
						
            /* dB滑块 */
            mainsli = lv_slider_create(mainoj1);                             /* 创建滑块 */
            lv_obj_set_size(mainsli, 230,25);                               /* 设置大小 */
						lv_obj_set_style_radius(mainsli, 1,0);
            lv_obj_set_style_radius(mainsli, 1,LV_PART_MAIN|LV_PART_INDICATOR);
            /* 本地样式 */
            lv_obj_set_style_bg_opa(mainsli,LV_OPA_0,LV_PART_KNOB);
            lv_obj_set_style_bg_color(mainsli,lv_color_hex(0xFFFFFF), LV_STATE_DEFAULT|LV_PART_INDICATOR);//指示器
            lv_obj_set_style_bg_color(mainsli,lv_color_hex(0xFFFFFF), LV_PART_MAIN);//主体
						
						lv_obj_align_to(mainsli, alignoj2, LV_ALIGN_BOTTOM_RIGHT, 25, 139);           /* 设置位置 */
						lv_obj_clear_flag(mainsli,LV_OBJ_FLAG_CHECKABLE);
						lv_obj_clear_state(mainsli,LV_STATE_ANY);
						lv_slider_set_range(mainsli, 0, 99);
						if(soundB>=188&&soundB<=252)soundBtemp=89+(soundB-180)/8;
						else if(soundB==255)soundBtemp=99;
						else soundBtemp=soundB/2;

            lv_slider_set_value(mainsli, 99-soundBtemp, LV_ANIM_OFF); /* 设置dB值 */
						
						/* 滑块图片 */
            mainsliimg= lv_img_create(mainsli);
            //lv_obj_set_size(mainsliimg,230,23);
            lv_img_set_src(mainsliimg, &XIEBIAN); /* 设置图片源 */
            lv_obj_align_to(mainsliimg, alignoj2, LV_ALIGN_BOTTOM_RIGHT, 24, 139);           /* 设置位置 */
						
						/* 输入选择图片 */
            mainojimg= lv_img_create(mainoj1);
            //lv_obj_set_size(oj4img,153,153);
            lv_obj_align_to(mainojimg, mainoj1,LV_ALIGN_CENTER, -335, -70);
						
            /* IN OUT */
            mainlb3 = lv_label_create(mainoj1);
            lv_obj_set_style_text_font(mainlb3,&lv_font_palatino_29_num,0);
            lv_obj_set_style_text_color(mainlb3, lv_color_hex(0xFFFFFF), LV_STATE_DEFAULT );
            lv_obj_set_style_text_opa(mainlb3,LV_OPA_100,LV_STATE_DEFAULT);
            lv_label_set_recolor(mainlb3, true );  /* 开启重新着色功能 */
            lv_obj_align_to(mainlb3, mainojimg, LV_ALIGN_CENTER, 0, 80);
						
						lb2 = lv_label_create(mainoj1);
            lv_obj_set_style_text_font(lb2,&lv_font_palatino_25_num,0);
            lv_obj_set_style_text_color(lb2, lv_color_hex(0xFFFFFF), LV_STATE_DEFAULT );
            lv_obj_set_style_text_opa(lb2,LV_OPA_100,LV_STATE_DEFAULT);
            lv_label_set_recolor(lb2, true );  /* 开启重新着色功能 */
            lv_label_set_text_fmt(lb2, "INPUT");
            lv_obj_align_to(lb2, mainlb3, LV_ALIGN_CENTER, 0, 87);
						
						
						insel0=*((volatile uint32_t *)(EEPROM_ADDR + 1*4));
						insel1=*((volatile uint32_t *)(EEPROM_ADDR + 2*4));
						insel=*((volatile uint32_t *)(EEPROM_ADDR + 3*4));
						if(insel0==-1)insel0=0;
						if(insel1==-1)insel1=0;
						WrBuff[1]=insel0;
						WrBuff[2]=insel1;
						WrBuff[3]=insel;
            
						WrBuff[4]=*((volatile uint32_t *)(EEPROM_ADDR + 4*4));//滤波器选择数据包
						if(WrBuff[4]==-1)WrBuff[4]=6;
						if(WrBuff[4]==0)lvbosel=1;
						else if(WrBuff[4]==2)lvbosel=3;
						else if(WrBuff[4]==1)lvbosel=2;
						else lvbosel=WrBuff[4];
						/* FILTER */
            mainlb4 = lv_label_create(mainoj1);
            lv_obj_set_style_text_font(mainlb4,&lv_font_palatino_27_num,0);
            lv_obj_set_style_text_color(mainlb4, lv_color_hex(0xFFFFFF), LV_STATE_DEFAULT );
            lv_obj_set_style_text_opa(mainlb4,LV_OPA_100,LV_STATE_DEFAULT);
            lv_label_set_recolor(mainlb4, true );  /* 开启重新着色功能 */
            lv_label_set_text_fmt(mainlb4, "FIR > #FFCC33 %d#",lvbosel);
            lv_obj_align_to(mainlb4, mainlb1, LV_ALIGN_CENTER, 0,112);
						lv_obj_align_to(mainlb5, mainlb4, LV_ALIGN_CENTER, 0,-188);

						
						if(insel0==0&&insel1==0){
							lv_img_set_src(mainojimg, &USB); /* 设置图片源 */
						lv_label_set_text_fmt(mainlb3, "%s","USB");
							LIN0;LIN1;//00 usb(xmos) or DSD
							if(insel==1){
//								lv_label_set_text_fmt(mainlb3, "IN:#3399FF %s#                                   OUT:#3399FF %s#","USB","RCA");
								}
							else if(insel==2){
//								write_9039m_regs(0x01, 0XB2);//DSD解码
//								write_9039m_regs(INSEL_SET, 0x42);//DSD输入
//								lv_label_set_text_fmt(mainlb3, "IN:#3399FF %s#                                   OUT:#3399FF %s#","DSD","RCA");
								}
							}	
						else if(insel0==1&&insel1==0){
//							HIN0;LIN1;//10 CS8416
							if(insel==8){
								lv_img_set_src(mainojimg, &COA); /* 设置图片源 */
								lv_label_set_text_fmt(mainlb3, "%s","COA");
            
//								write_CS8416_regs(Control4_Set, 0x91);
//								lv_label_set_text_fmt(mainlb3, "IN:#3399FF %s#                                   OUT:#3399FF %s#","同轴","RCA");
								}
							else if(insel==16){
								lv_img_set_src(mainojimg, &TOS); /* 设置图片源 */
								lv_label_set_text_fmt(mainlb3, "%s","TOS");
//								write_CS8416_regs(Control4_Set, 0x89);
//								lv_label_set_text_fmt(mainlb3, "IN:#3399FF %s#                                   OUT:#3399FF %s#","光纤","RCA");
								}
						}
						else if((insel0==0&&insel1==1)){
							lv_img_set_src(mainojimg, &HDMI); /* 设置图片源 */
							lv_label_set_text_fmt(mainlb3, "%s","HDMI");
//						LIN0;HIN1;//01 hdmi
//						lv_label_set_text_fmt(mainlb3, "IN:#3399FF %s#                                   OUT:#3399FF %s#","HDMI","RCA");
							}
						else{
						LIN0;LIN1;//00 usb(xmos)
						insel0=0,insel1=0;
							lv_img_set_src(mainojimg, &USB); /* 设置图片源 */
							lv_label_set_text_fmt(mainlb3, "%s","USB");
//            lv_label_set_text_fmt(mainlb3, "IN:#3399FF %s#                                   OUT:#3399FF %s#","USB","RCA");
							}
						//lv_obj_set_style_transform_zoom(mainojimg,320,0);
						lv_obj_align_to(mainojimg, mainoj1,LV_ALIGN_CENTER, -335, -60);
						lv_obj_align_to(mainlb3, mainojimg, LV_ALIGN_CENTER, 0, 80);
            /* 大按键加入组*/
            gmain = lv_group_create();//创建组
            //lv_group_set_default(gmain);//如果部件已加入默认组，使用这种配置 设置默认组
            lv_group_add_obj(gmain,mainoj1);//如果部件未加入默认组，使用这种配置 添加部件到组
            lv_indev_set_group(indev_keypad, gmain);//关联按键和组

						soundBflag=1;//开启旋钮控制音量						
						flashflaginsel=1;
//lv_label_set_text_fmt(mainlb4,"%d-%d-%d", WrBuff[9],WrBuff[10],WrBuff[11]);//把读出的寄存器值写在屏幕上	

}


        /**********       创建菜单（2）                           */
    void create_menu_win(){
				//levelflag=2;
//				WrBuff[18]=0x20;
			WrBuff[18]=0x10;
//			  printf("%d\n",levelflag);
        scr = lv_obj_create(NULL);
        lv_scr_load(scr);
        oj = lv_obj_create(scr);
        lv_obj_set_size(oj,scr_act_width(), scr_act_height());//在主屏幕下创建一个等大的对象屏幕
        lv_obj_set_style_radius(oj,0,0);//设置容器圆角为直角
        lv_obj_align(oj, LV_ALIGN_CENTER, 0, 0);
        lv_obj_add_style(oj, & stylebk, LV_STATE_DEFAULT);
        lv_obj_add_style(scr, & stylebk, LV_STATE_DEFAULT);
        /* 菜单面板 */
        menuoj = lv_obj_create(scr);
        lv_obj_set_size(menuoj,750, 195);

        lv_obj_set_scroll_dir(menuoj, LV_DIR_VER);
        lv_obj_set_style_border_color(menuoj, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
        lv_obj_set_style_bg_color(menuoj, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
				lv_obj_add_style(menuoj, &scrollstyle, LV_PART_SCROLLBAR);
				lv_obj_set_style_radius(menuoj,0,0);//设置容器圆角为直角
        /* 大标题标签 */
        lb=lv_label_create(scr);
        lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
        lv_label_set_text(lb, "设置");
        lv_label_set_recolor(lb,true);//重新着色
        lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
        lv_obj_align_to(lb,scr,LV_ALIGN_CENTER,0,-105);
        lv_obj_align_to(menuoj,lb,LV_ALIGN_CENTER,0,150);
        /* 返回按钮 */
        wbt = lv_obj_create(scr);//menuback
        lv_obj_set_size(wbt,150,50);
        lv_obj_align(wbt, LV_ALIGN_TOP_LEFT,  10, 125);
        lv_obj_add_style(wbt, &style, 0);//透明化
        lv_obj_add_style(wbt, &style2, LV_STATE_FOCUSED);
				lv_obj_set_style_radius(wbt,0,0);//设置容器圆角为直角
        lv_obj_add_event_cb(wbt, event_cbback, LV_EVENT_CLICKED, NULL);//添加事件
        lv_obj_add_event_cb(wbt, event_cbback, LV_EVENT_FOCUSED, NULL);//添加事件
        lv_obj_clear_flag(wbt, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
        /* 返回图标 */
        ojimg =lv_img_create(wbt);
        lv_img_set_src(ojimg, &back); /* 设置图片源 */
        lv_obj_align_to(ojimg,wbt,LV_ALIGN_CENTER,-50,0);
        /* 返回标签 */
        wlbback=lv_label_create(wbt);//按钮中的标签
        lv_obj_set_style_text_font(wlbback,&lv_font_msyh_23_num,0);
        lv_label_set_text(wlbback, "主界面");
        lv_label_set_recolor(wlbback,true);//重新着色
        lv_obj_add_style(wlbback, &styletext2, LV_STATE_DEFAULT);
        lv_obj_align_to(wlbback,wbt, LV_ALIGN_CENTER,10,0);

        int a,b=0;
        /* 线条 */
        for(a=1;a<9;a++){
        line=lv_obj_create(menuoj);
        lv_obj_set_style_bg_color(line, lv_color_hex(0x2A2829), LV_STATE_DEFAULT );
				lv_obj_set_style_border_color(line, lv_color_hex(0x2A2829), LV_STATE_DEFAULT);
        lv_obj_set_size(line,700,1);
        if(a==1)
        lv_obj_align_to(line, menuoj,LV_ALIGN_TOP_MID, 5, 40);
        else lv_obj_align_to(line, menuoj,LV_ALIGN_TOP_MID, 5, 40+b);
        b+=60;
        }

            /**********       oj1                           */
            oj1 = lv_obj_create(menuoj);
            lv_obj_set_size(oj1,710,50);
            lv_obj_align_to(oj1, menuoj, LV_ALIGN_TOP_LEFT, -5, -15);
            lv_obj_add_style(oj1, &style, 0);
            lv_obj_add_style(oj1, &style2, LV_STATE_FOCUSED);
            lv_obj_clear_flag(oj1, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
						lv_obj_set_style_radius(oj1,0,0);//设置容器圆角为直角
				
            lb=lv_label_create(oj1);//标题1标签
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "输入选择");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_add_event_cb(oj1, focus_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
				    lv_obj_add_event_cb(oj1, clk1_event_cb, LV_EVENT_CLICKED, NULL);
            lv_obj_align_to(lb,oj1, LV_ALIGN_TOP_LEFT,-5,-10);

            /* 前进图标 */
            ojnextimg =lv_img_create(oj1);
            lv_img_set_src(ojnextimg, &next); /* 设置图片源 */
            lv_obj_align_to(ojnextimg,oj1, LV_ALIGN_TOP_LEFT,660,-5);

            /* 文本框1 */
            lb1 =lv_obj_create(oj1);
            lv_obj_set_size(lb1,80,30);
            lv_obj_align_to(lb1,ojnextimg , LV_ALIGN_LEFT_MID,-90,0);
            lv_obj_add_style(lb1, &style, 0);//透明化
            lv_obj_clear_flag(lb1, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
                menulb1=lv_label_create(oj1);//输入选择标签
                lv_obj_set_style_text_font(menulb1,&lv_font_msyh_23_num,0);
                //lv_label_set_text_fmt(menulb1, "%s","光纤");
                lv_label_set_recolor(menulb1,true);//重新着色
                lv_obj_add_style(menulb1, &styletext2, LV_STATE_DEFAULT);
                lv_obj_align_to(menulb1,lb1, LV_ALIGN_RIGHT_MID,15,0);

            /**********       oj6                           */
            oj6 = lv_obj_create(menuoj);
            lv_obj_set_size(oj6,710,50);
            lv_obj_align_to(oj6, menuoj, LV_ALIGN_TOP_LEFT, -5, 285);
            lv_obj_add_style(oj6, &style, 0);
            lv_obj_add_style(oj6, &style2, LV_STATE_FOCUSED);
            lv_obj_clear_flag(oj6, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
						lv_obj_set_style_radius(oj6,0,0);//设置容器圆角为直角

            lb=lv_label_create(oj6);//标题1标签
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "亮度与主题");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_add_event_cb(oj6, focus_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
						lv_obj_add_event_cb(oj6, clk1_event_cb, LV_EVENT_CLICKED, NULL);
            lv_obj_align_to(lb,oj6, LV_ALIGN_TOP_LEFT,-5,-10);

            /* 前进图标 */
            ojnextimg =lv_img_create(oj6);
            lv_img_set_src(ojnextimg, &next); /* 设置图片源 */
            lv_obj_align_to(ojnextimg,oj6, LV_ALIGN_TOP_LEFT,660,-5);

            /* 文本框2 */
            menulb =lv_obj_create(oj6);
            lv_obj_set_size(menulb,80,30);
            lv_obj_align_to(menulb,ojnextimg , LV_ALIGN_LEFT_MID,-90,0);
            lv_obj_add_style(menulb, &style, 0);//透明化
            lv_obj_clear_flag(menulb, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
                menulb2=lv_label_create(oj6);
                lv_obj_set_style_text_font(menulb2,&lv_font_msyh_23_num,0);
                //lv_label_set_text_fmt(menulb2, "8/%d",5);
                lv_label_set_recolor(menulb2,true);//重新着色
                lv_obj_add_style(menulb2, &styletext2, LV_STATE_DEFAULT);
                lv_obj_align_to(menulb2,menulb, LV_ALIGN_RIGHT_MID,22,0);

            /**********       oj5                           */
            oj5 = lv_obj_create(menuoj);
            lv_obj_set_size(oj5,710,50);
            lv_obj_align_to(oj5, menuoj, LV_ALIGN_TOP_LEFT, -5, 225);
            lv_obj_add_style(oj5, &style, 0);
            lv_obj_add_style(oj5, &style2, LV_STATE_FOCUSED);
            lv_obj_clear_flag(oj5, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
						lv_obj_set_style_radius(oj5,0,0);//设置容器圆角为直角

            lb=lv_label_create(oj5);//标题1标签
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "9039MPRO设置");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_add_event_cb(oj5, focus_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
						lv_obj_add_event_cb(oj5, clk1_event_cb, LV_EVENT_CLICKED, NULL);
            lv_obj_align_to(lb,oj5, LV_ALIGN_TOP_LEFT,-5,-10);

            /* 前进图标 */
            ojnextimg =lv_img_create(oj5);
            lv_img_set_src(ojnextimg, &next); /* 设置图片源 */
            lv_obj_align_to(ojnextimg,oj5, LV_ALIGN_TOP_LEFT,660,-5);

            /* 文本框2 */
            menulb =lv_obj_create(oj5);
            lv_obj_set_size(menulb,80,30);
            lv_obj_align_to(menulb,ojnextimg , LV_ALIGN_LEFT_MID,-90,0);
            lv_obj_add_style(menulb, &style, 0);//透明化
            lv_obj_clear_flag(menulb, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
                menulb3=lv_label_create(oj5);
                lv_obj_set_style_text_font(menulb3,&lv_font_msyh_23_num,0);
                lv_label_set_text_fmt(menulb3, "SET");
                lv_label_set_recolor(menulb3,true);//重新着色
                lv_obj_add_style(menulb3, &styletext2, LV_STATE_DEFAULT);
                lv_obj_align_to(menulb3,menulb, LV_ALIGN_RIGHT_MID,15,0);

            /**********       oj4                           */
            oj4 = lv_obj_create(menuoj);
            lv_obj_set_size(oj4,710,50);
            lv_obj_align_to(oj4, menuoj, LV_ALIGN_TOP_LEFT, -5, 165);
            lv_obj_add_style(oj4, &style, 0);
            lv_obj_add_style(oj4, &style2, LV_STATE_FOCUSED);
            lv_obj_clear_flag(oj4, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
						lv_obj_set_style_radius(oj4,0,0);//设置容器圆角为直角

            lb=lv_label_create(oj4);//标题1标签
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "本地全局飞秒时钟");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_add_event_cb(oj4, focus_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
						lv_obj_add_event_cb(oj4, clk1_event_cb, LV_EVENT_CLICKED, NULL);
            lv_obj_align_to(lb,oj4, LV_ALIGN_TOP_LEFT,-5,-10);

            /* 前进图标 */
            ojnextimg =lv_img_create(oj4);
            lv_img_set_src(ojnextimg, &next); /* 设置图片源 */
            lv_obj_align_to(ojnextimg,oj4, LV_ALIGN_TOP_LEFT,660,-5);

            /* 文本框2 */
            menulb =lv_obj_create(oj4);
            lv_obj_set_size(menulb,80,30);
            lv_obj_align_to(menulb,ojnextimg , LV_ALIGN_LEFT_MID,-90,0);
            lv_obj_add_style(menulb, &style, 0);//透明化
            lv_obj_clear_flag(menulb, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
            menulb4=lv_label_create(oj4);//输入选择标签
            lv_obj_set_style_text_font(menulb4,&lv_font_msyh_23_num,0);
            if(WrBuff[10]==1)lv_label_set_text_fmt(menulb4, "%s","关");
            else lv_label_set_text_fmt(menulb4, "%s","开");
            lv_label_set_recolor(menulb4,true);//重新着色
            lv_obj_add_style(menulb4, &styletext2, LV_STATE_DEFAULT);
            lv_obj_align_to(menulb4,menulb, LV_ALIGN_RIGHT_MID,15,0);

            /**********       oj2                           */
            oj2 = lv_obj_create(menuoj);
            lv_obj_set_size(oj2,710,50);
            lv_obj_align_to(oj2, menuoj, LV_ALIGN_TOP_LEFT, -5, 45);
            lv_obj_add_style(oj2, &style, 0);
            lv_obj_add_style(oj2, &style2, LV_STATE_FOCUSED);
            lv_obj_clear_flag(oj2, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
						lv_obj_set_style_radius(oj2,0,0);//设置容器圆角为直角

            lb=lv_label_create(oj2);//标题1标签
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "滤波器选择");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_add_event_cb(oj2, focus_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
						lv_obj_add_event_cb(oj2, clk1_event_cb, LV_EVENT_CLICKED, NULL);
            lv_obj_align_to(lb,oj2, LV_ALIGN_TOP_LEFT,-5,-10);

            /* 前进图标 ">" */
            ojnextimg =lv_img_create(oj2);
            lv_img_set_src(ojnextimg, &next); /* 设置图片源 */
            lv_obj_align_to(ojnextimg,oj2, LV_ALIGN_TOP_LEFT,660,-5);

            /* 文本框2 */
            menulb =lv_obj_create(oj2);
            lv_obj_set_size(menulb,80,30);
            lv_obj_align_to(menulb,ojnextimg , LV_ALIGN_LEFT_MID,-90,0);
            lv_obj_add_style(menulb, &style, 0);//透明化
            lv_obj_clear_flag(menulb, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
            menulb5=lv_label_create(oj2);//输入选择标签
            lv_obj_set_style_text_font(menulb5,&lv_font_msyh_23_num,0);
            lv_label_set_recolor(menulb5,true);//重新着色
            //lv_label_set_text_fmt(menulb5, "7/%d",4);
						if(WrBuff[4]==7){
            lv_label_set_text_fmt(menulb5, "7/%d",7);
						}
            else if(WrBuff[4]==6)lv_label_set_text_fmt(menulb5, "7/%d",6);
						else if(WrBuff[4]==5)lv_label_set_text_fmt(menulb5, "7/%d",5);
						else if(WrBuff[4]==4)lv_label_set_text_fmt(menulb5, "7/%d",4);
						else if(WrBuff[4]==2)lv_label_set_text_fmt(menulb5, "7/%d",3);
						else if(WrBuff[4]==1)lv_label_set_text_fmt(menulb5, "7/%d",2);
						else lv_label_set_text_fmt(menulb5, "7/%d",1);
                
            lv_obj_add_style(menulb5, &styletext2, LV_STATE_DEFAULT);
            lv_obj_align_to(menulb5,menulb, LV_ALIGN_RIGHT_MID,15,0);

            /**********       oj7                           */
            oj7 = lv_obj_create(menuoj);
            lv_obj_set_size(oj7,710,50);
            lv_obj_align_to(oj7, menuoj, LV_ALIGN_TOP_LEFT, -5, 345);
            lv_obj_add_style(oj7, &style, 0);
            lv_obj_add_style(oj7, &style2, LV_STATE_FOCUSED);
            lv_obj_clear_flag(oj7, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
						lv_obj_set_style_radius(oj7,0,0);//设置容器圆角为直角
						
            lb=lv_label_create(oj7);//标题1标签
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "平衡调节");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_add_event_cb(oj7, focus_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
						lv_obj_add_event_cb(oj7, clk1_event_cb, LV_EVENT_CLICKED, NULL);
            lv_obj_align_to(lb,oj7, LV_ALIGN_TOP_LEFT,-5,-10);

            /* 前进图标 */
            ojnextimg =lv_img_create(oj7);
            lv_img_set_src(ojnextimg, &next); /* 设置图片源 */
            lv_obj_align_to(ojnextimg,oj7, LV_ALIGN_TOP_LEFT,660,-5);

            /* 文本框2 */
            menulb =lv_obj_create(oj7);
            lv_obj_set_size(menulb,80,30);
            lv_obj_align_to(menulb,ojnextimg , LV_ALIGN_LEFT_MID,-90,0);
            lv_obj_add_style(menulb, &style, 0);//透明化
            lv_obj_clear_flag(menulb, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
                menulb6=lv_label_create(oj7);//输入选择标签
                lv_obj_set_style_text_font(menulb6,&lv_font_msyh_23_num,0);
								soundBLtemp=WrBuff[6],soundBRtemp=WrBuff[7];
								if( ((soundBLtemp/2.0)-soundBLtemp/2>0) &&((soundBRtemp/2.0)-soundBRtemp/2>0) )lv_label_set_text_fmt(menulb6, "L %.1f / R %.1f",soundBLtemp/2.0,soundBRtemp/2.0);
								else if( ((soundBLtemp/2.0)-soundBLtemp/2>0) &&((soundBRtemp/2.0)-soundBRtemp/2==0)) lv_label_set_text_fmt(menulb6, "L %.1f / R %d",soundBLtemp/2.0,soundBRtemp/2);
								else if( ((soundBLtemp/2.0)-soundBLtemp/2==0) &&((soundBRtemp/2.0)-soundBRtemp/2>0)) lv_label_set_text_fmt(menulb6, "L %d / R %.1f",soundBLtemp/2,soundBRtemp/2.0);
								else if( ((soundBLtemp/2.0)-soundBLtemp/2==0) &&((soundBRtemp/2.0)-soundBRtemp/2==0)) lv_label_set_text_fmt(menulb6, "L %d / R %d",soundBLtemp/2,soundBRtemp/2);
								else lv_label_set_text_fmt(menulb6, "L %d / R %d",0,0);
                lv_label_set_recolor(menulb6,true);//重新着色
                lv_obj_add_style(menulb6, &styletext2, LV_STATE_DEFAULT);
                lv_obj_align_to(menulb6,menulb, LV_ALIGN_RIGHT_MID,15,0);

            /**********       oj8                           */
            oj8 = lv_obj_create(menuoj);
            lv_obj_set_size(oj8,710,50);
            lv_obj_align_to(oj8, menuoj, LV_ALIGN_TOP_LEFT, -5, 405);
            lv_obj_add_style(oj8, &style, 0);
            lv_obj_add_style(oj8, &style2, LV_STATE_FOCUSED);
            lv_obj_clear_flag(oj8, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
						lv_obj_set_style_radius(oj8,0,0);//设置容器圆角为直角
								
            lb=lv_label_create(oj8);//标题1标签
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "左右输出互换");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_add_event_cb(oj8, focus_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
						lv_obj_add_event_cb(oj8, clk1_event_cb, LV_EVENT_CLICKED, NULL);
            lv_obj_align_to(lb,oj8, LV_ALIGN_TOP_LEFT,-5,-10);

            /* 前进图标 */
            ojnextimg =lv_img_create(oj8);
            lv_img_set_src(ojnextimg, &next); /* 设置图片源 */
            lv_obj_align_to(ojnextimg,oj8, LV_ALIGN_TOP_LEFT,660,-5);

            /* 文本框2 */
            menulb =lv_obj_create(oj8);
            lv_obj_set_size(menulb,80,30);
            lv_obj_align_to(menulb,ojnextimg , LV_ALIGN_LEFT_MID,-90,0);
            lv_obj_add_style(menulb, &style, 0);//透明化
            lv_obj_clear_flag(menulb, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
                menulb7=lv_label_create(oj8);//输入选择标签
                lv_obj_set_style_text_font(menulb7,&lv_font_msyh_23_num,0);
								if(WrBuff[11]==1)lv_label_set_text_fmt(menulb7, "%s","开");
								else lv_label_set_text_fmt(menulb7, "%s","关");
                lv_label_set_recolor(menulb7,true);//重新着色
                lv_obj_add_style(menulb7, &styletext2, LV_STATE_DEFAULT);
                lv_obj_align_to(menulb7,menulb, LV_ALIGN_RIGHT_MID,15,0);

            /**********       oj3                           */
            oj3 = lv_obj_create(menuoj);
            lv_obj_set_size(oj3,710,50);
            lv_obj_align_to(oj3, menuoj, LV_ALIGN_TOP_LEFT, -5, 105);
            lv_obj_add_style(oj3, &style, 0);
            lv_obj_add_style(oj3, &style2, LV_STATE_FOCUSED);
            lv_obj_clear_flag(oj3, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
						lv_obj_set_style_radius(oj3,0,0);//设置容器圆角为直角

            lb=lv_label_create(oj3);//标题1标签
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "输出选择");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_add_event_cb(oj3, focus_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
            lv_obj_add_event_cb(oj3, clk1_event_cb, LV_EVENT_CLICKED, NULL);
            lv_obj_align_to(lb,oj3, LV_ALIGN_TOP_LEFT,-5,-10);

            /* 前进图标 */
            ojnextimg =lv_img_create(oj3);
            lv_img_set_src(ojnextimg, &next); /* 设置图片源 */
            lv_obj_align_to(ojnextimg,oj3, LV_ALIGN_TOP_LEFT,660,-5);

            /* 文本框2 */
            menulb =lv_obj_create(oj3);
            lv_obj_set_size(menulb,80,30);
            lv_obj_align_to(menulb,ojnextimg , LV_ALIGN_LEFT_MID,-90,0);
            lv_obj_add_style(menulb, &style, 0);//透明化
            lv_obj_clear_flag(menulb, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
                menulb8=lv_label_create(oj3);//输入选择标签
                lv_obj_set_style_text_font(menulb8,&lv_font_msyh_23_num,0);
                //lv_label_set_text_fmt(menulb8, "%s","全开");
                lv_label_set_recolor(menulb8,true);//重新着色
                lv_obj_add_style(menulb8, &styletext2, LV_STATE_DEFAULT);
                lv_obj_align_to(menulb8,menulb, LV_ALIGN_RIGHT_MID,15,0);
								
						/**********       oj9                           */
            oj9 = lv_obj_create(menuoj);
            lv_obj_set_size(oj9,710,50);
            lv_obj_align_to(oj9, menuoj, LV_ALIGN_TOP_LEFT, -5, 465);
            lv_obj_add_style(oj9, &style, 0);
            lv_obj_add_style(oj9, &style2, LV_STATE_FOCUSED);
            lv_obj_clear_flag(oj9, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
						lv_obj_set_style_radius(oj9,0,0);//设置容器圆角为直角

            lb=lv_label_create(oj9);//标题1标签
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "恢复出厂设置");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_add_event_cb(oj9, focus_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
            lv_obj_add_event_cb(oj9, focus_event_cb, LV_EVENT_CLICKED, NULL);
            lv_obj_align_to(lb,oj9, LV_ALIGN_TOP_LEFT,-5,-10);
								
        /**********       加入组                           */
            create_main_group();
          
            if(idflag==1)lv_group_focus_obj(oj1);
            if(idflag==2)lv_group_focus_obj(oj2);
            if(idflag==3)lv_group_focus_obj(oj3);
            if(idflag==4)lv_group_focus_obj(oj4);
            if(idflag==5)lv_group_focus_obj(oj5);
            if(idflag==6)lv_group_focus_obj(oj6);
            if(idflag==7)lv_group_focus_obj(oj7);
            if(idflag==8)lv_group_focus_obj(oj8);
            if(idflag==9)lv_group_focus_obj(oj9);
            
				/**********      二级菜单显示数据                          */
						insel0=*((volatile uint32_t *)(EEPROM_ADDR + 1*4));
						insel1=*((volatile uint32_t *)(EEPROM_ADDR + 2*4));
						insel=*((volatile uint32_t *)(EEPROM_ADDR + 3*4));
							if(insel0==0&&insel1==0){
						lv_label_set_text_fmt(menulb1, "%s","USB");
							//LIN0;LIN1;//00 usb(xmos) or DSD
							if(insel==1){
								}
							else if(insel==2){
								}
							}	
						else if(insel0==1&&insel1==0){
							if(insel==8)lv_label_set_text_fmt(menulb1, "%s","同轴");				
							else if(insel==16)lv_label_set_text_fmt(menulb1, "%s","光纤");
						}
						else if((insel0==0&&insel1==1)){
						lv_obj_align_to(menulb1,lb1, LV_ALIGN_RIGHT_MID,-5,0);
						lv_label_set_text_fmt(menulb1, "%s","HDMI");

						}
						else
						lv_label_set_text_fmt(menulb1, "%s","USB");
						
						backlight=*((volatile uint32_t *)(EEPROM_ADDR + 5*4));//背光数据包
						if(backlight<0)backlight=0;if(backlight>7)backlight=8;
						lv_label_set_text_fmt(menulb2, "8/%d",backlight);
						
							outgpio=2*GPIO_GetBit(GPIOM, PIN5)+GPIO_GetBit(GPION, PIN0);
							 if(outgpio==0)WrBuff[12]=3;//0 0
							 else if(outgpio==1)WrBuff[12]=2;//0 1
							 else if(outgpio==2)WrBuff[12]=1;//1 0
							 else if(outgpio==3)WrBuff[12]=0;//1 1
							 flashflaglvbo=1;
						if(WrBuff[12]==0){lv_label_set_text_fmt(menulb8, "%s","全开");}
						else if(WrBuff[12]==1){lv_label_set_text_fmt(menulb8, "%s","耳放");}
						else if(WrBuff[12]==2){lv_label_set_text_fmt(menulb8, "%s","RCA-XLR");}
						else if(WrBuff[12]==3){lv_label_set_text_fmt(menulb8, "%s","全关");}
			lv_obj_align_to(menulb8,menulb, LV_ALIGN_RIGHT_MID,15,0);
						
						levelflag=2;
//						flashflaginsel=1;
    }

						/**********       创建附屏幕（3）                           */    
				void show_oj_win1(){//输入选择
							levelflag=3;
              idflag=1;
//							WrBuff[18]=0x31;
							/**********       输入选择                          */
							ojw1 = lv_obj_create(NULL);
							lv_scr_load(ojw1);
							lv_obj_add_style(ojw1, & stylebk, LV_STATE_DEFAULT);

							/********       菜单面板                           */
							menuoj = lv_obj_create(ojw1);
							lv_obj_set_size(menuoj,750, 200);

							lv_obj_set_scroll_dir(menuoj, LV_DIR_VER);
							lv_obj_set_style_border_color(menuoj, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
							lv_obj_set_style_bg_color(menuoj, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
							lv_obj_add_style(menuoj, &scrollstyle, LV_PART_SCROLLBAR);
							lv_obj_set_style_radius(menuoj,0,0);//设置容器圆角为直角
					
							/* 大标题标签 */
							lb=lv_label_create(ojw1);
							lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
							lv_label_set_text(lb, "输入选择");
							lv_label_set_recolor(lb,true);//重新着色
							lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
							lv_obj_align_to(lb,ojw1,LV_ALIGN_CENTER,0,-105);
							lv_obj_align_to(menuoj,lb,LV_ALIGN_CENTER,0,150);
							/* 返回按钮 */
							wbt = lv_obj_create(ojw1);//menuback
							lv_obj_set_size(wbt,150,50);
							lv_obj_align(wbt, LV_ALIGN_TOP_LEFT,  10, 125);
							lv_obj_add_style(wbt, &style, 0);//透明化
							lv_obj_add_style(wbt, &style2, LV_STATE_FOCUSED);
							lv_obj_set_style_radius(wbt,0,0);//设置容器圆角为直角
							lv_obj_add_event_cb(wbt, event_cbback, LV_EVENT_CLICKED, NULL);//添加事件
							lv_obj_add_event_cb(wbt, event_cbback, LV_EVENT_FOCUSED, NULL);//添加事件
							lv_obj_clear_flag(wbt, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
							/* 返回图标 */
							ojimg =lv_img_create(wbt);
							lv_img_set_src(ojimg, &back); /* 设置图片源 */
							lv_obj_align_to(ojimg,wbt,LV_ALIGN_CENTER,-50,0);
							/* 返回标签 */
							wlbback=lv_label_create(wbt);//按钮中的标签
							lv_obj_set_style_text_font(wlbback,&lv_font_msyh_23_num,0);
							lv_label_set_text(wlbback, "设置");
							lv_label_set_recolor(wlbback,true);//重新着色
							lv_obj_add_style(wlbback, &styletext2, LV_STATE_DEFAULT);
							lv_obj_align_to(wlbback,wbt, LV_ALIGN_CENTER,0,0);

							int a,b=0;
							/* 线条 */
							for(a=1;a<=4;a++){
							line=lv_obj_create(menuoj);
							lv_obj_set_style_bg_color(line, lv_color_hex(0x2A2829), LV_STATE_DEFAULT);
							lv_obj_set_style_border_color(line, lv_color_hex(0x2A2829), LV_STATE_DEFAULT);
							lv_obj_set_size(line,700,1);
							if(a==1)
							lv_obj_align_to(line, menuoj,LV_ALIGN_TOP_MID, 5, 40);
							else lv_obj_align_to(line, menuoj,LV_ALIGN_TOP_MID, 5, 40+b);
							b+=60;
							}

									/**********       USB                           */
									w1bt1 = lv_obj_create(menuoj);
									lv_obj_set_size(w1bt1,700,50);
									lv_obj_align_to(w1bt1, menuoj, LV_ALIGN_TOP_LEFT, 5, -15);
									lv_obj_add_style(w1bt1, &style, 0);
									lv_obj_add_style(w1bt1, &style2, LV_STATE_FOCUSED);
									lv_obj_clear_flag(w1bt1, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
									lv_obj_set_style_radius(w1bt1,0,0);//设置容器圆角为直角
							
									/* 输入选择图片 */
									w1img= lv_img_create(w1bt1);
									lv_img_set_src(w1img, &USB3); /* 设置图片源 */
									lv_obj_align_to(w1img, w1bt1,LV_ALIGN_TOP_LEFT,-15,-15);

									lb=lv_label_create(w1bt1);
									lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
									lv_label_set_text(lb, "USB");
									lv_label_set_recolor(lb,true);//重新着色
									lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
									lv_obj_add_event_cb(w1bt1, w1_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
									lv_obj_add_event_cb(w1bt1, w1_event_cb, LV_EVENT_CLICKED, NULL);/* 添加事件 */
									lv_obj_align_to(lb,w1img, LV_ALIGN_CENTER,60,0);

									/* 选中图标 */
									ojenter1img =lv_img_create(w1bt1);
									lv_img_set_src(ojenter1img, &ENTER3); /* 设置图片源 */
									lv_obj_align_to(ojenter1img,w1bt1, LV_ALIGN_TOP_LEFT,625,-5);

									/**********       同轴                           */
									w1bt2 = lv_obj_create(menuoj);
									lv_obj_set_size(w1bt2,700,50);
									lv_obj_align_to(w1bt2, menuoj, LV_ALIGN_TOP_LEFT, 5, 45);
									lv_obj_add_style(w1bt2, &style, 0);
									lv_obj_add_style(w1bt2, &style2, LV_STATE_FOCUSED);
									lv_obj_clear_flag(w1bt2, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
									lv_obj_set_style_radius(w1bt2,0,0);//设置容器圆角为直角

									/* 输入选择图片 */
									w1img= lv_img_create(w1bt2);
									lv_img_set_src(w1img, &COA3); /* 设置图片源 */
									lv_obj_align_to(w1img, w1bt2,LV_ALIGN_TOP_LEFT,-15,-15);

									lb=lv_label_create(w1bt2);
									lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
									lv_label_set_text(lb, "同轴");
									lv_label_set_recolor(lb,true);//重新着色
									lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
									lv_obj_add_event_cb(w1bt2, w1_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
									lv_obj_add_event_cb(w1bt2, w1_event_cb, LV_EVENT_CLICKED, NULL);/* 添加事件 */
									lv_obj_align_to(lb,w1img, LV_ALIGN_CENTER,60,0);

									/* 选中图标 */
									ojenter2img =lv_img_create(w1bt2);
									lv_img_set_src(ojenter2img, &ENTER3); /* 设置图片源 */
									lv_obj_align_to(ojenter2img,w1bt2, LV_ALIGN_TOP_LEFT,625,-5);

									/**********       光纤                           */
									w1bt3 = lv_obj_create(menuoj);
									lv_obj_set_size(w1bt3,700,50);
									lv_obj_align_to(w1bt3, menuoj, LV_ALIGN_TOP_LEFT, 5, 105);
									lv_obj_add_style(w1bt3, &style, 0);
									lv_obj_add_style(w1bt3, &style2, LV_STATE_FOCUSED);
									lv_obj_clear_flag(w1bt3, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
									lv_obj_set_style_radius(w1bt3,0,0);//设置容器圆角为直角

									/* 输入选择图片 */
									w1img= lv_img_create(w1bt3);
									lv_img_set_src(w1img, &TOS3); /* 设置图片源 */
									lv_obj_align_to(w1img, w1bt3,LV_ALIGN_TOP_LEFT,-15,-15);

									lb=lv_label_create(w1bt3);
									lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
									lv_label_set_text(lb, "光纤");
									lv_label_set_recolor(lb,true);//重新着色
									lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
									lv_obj_add_event_cb(w1bt3, w1_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
									lv_obj_add_event_cb(w1bt3, w1_event_cb, LV_EVENT_CLICKED, NULL);/* 添加事件 */
									lv_obj_align_to(lb,w1img, LV_ALIGN_CENTER,60,0);

									/* 选中图标 */
									ojenter3img =lv_img_create(w1bt3);
									lv_img_set_src(ojenter3img, &ENTER3); /* 设置图片源 */
									lv_obj_align_to(ojenter3img,w1bt3, LV_ALIGN_TOP_LEFT,625,-5);

									/**********       HDMI                           */
									w1bt4 = lv_obj_create(menuoj);
									lv_obj_set_size(w1bt4,700,50);
									lv_obj_align_to(w1bt4, menuoj, LV_ALIGN_TOP_LEFT, 5, 165);
									lv_obj_add_style(w1bt4, &style, 0);
									lv_obj_add_style(w1bt4, &style2, LV_STATE_FOCUSED);
									lv_obj_clear_flag(w1bt4, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
									lv_obj_set_style_radius(w1bt4,0,0);//设置容器圆角为直角

									/* 输入选择图片 */
									w1img= lv_img_create(w1bt4);
									lv_img_set_src(w1img, &HDMI3); /* 设置图片源 */
									lv_obj_align_to(w1img, w1bt4,LV_ALIGN_TOP_LEFT,-15,-15);

									lb=lv_label_create(w1bt4);
									lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
									lv_label_set_text(lb, "HDMI");
									lv_label_set_recolor(lb,true);//重新着色
									lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
									lv_obj_add_event_cb(w1bt4, w1_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
									lv_obj_add_event_cb(w1bt4, w1_event_cb, LV_EVENT_CLICKED, NULL);/* 添加事件 */
									lv_obj_align_to(lb,w1img, LV_ALIGN_CENTER,60,0);

									/* 选中图标 */
									ojenter4img =lv_img_create(w1bt4);
									lv_img_set_src(ojenter4img, &ENTER3); /* 设置图片源 */
									lv_obj_align_to(ojenter4img,w1bt4, LV_ALIGN_TOP_LEFT,625,-5);

									lv_obj_set_style_img_opa(ojenter1img,LV_OPA_0,0);
									lv_obj_set_style_img_opa(ojenter2img,LV_OPA_0,0);
									lv_obj_set_style_img_opa(ojenter3img,LV_OPA_0,0);
									lv_obj_set_style_img_opa(ojenter4img,LV_OPA_0,0);

									g1 = lv_group_create();//创建组
									//lv_group_set_default(g1);//如果部件已加入默认组，使用这种配置 设置默认组
									lv_group_add_obj(g1,wbt);
									lv_group_add_obj(g1,w1bt1);
									lv_group_add_obj(g1,w1bt2);
									lv_group_add_obj(g1,w1bt3);
									lv_group_add_obj(g1,w1bt4);

					        lv_indev_set_group(indev_keypad, g1);//关联按键和组

	//        lv_group_focus_obj(wbt);//聚焦
	
							/********** 输入选择显示勾选 */
							if(WrBuff[1]==0&&WrBuff[2]==0){
						lv_obj_set_style_img_opa(ojenter1img,LV_OPA_100,0);
							if(insel==1){
								}
							else if(insel==2){
								}
							}	
						else if(WrBuff[1]==1&&WrBuff[2]==0){
							if(WrBuff[3]==8)lv_obj_set_style_img_opa(ojenter2img,LV_OPA_100,0);			
							else if(WrBuff[3]==16)lv_obj_set_style_img_opa(ojenter3img,LV_OPA_100,0);
						}
						else if((WrBuff[1]==0&&WrBuff[2]==1))
						lv_obj_set_style_img_opa(ojenter4img,LV_OPA_100,0);
						else
						lv_obj_set_style_img_opa(ojenter1img,LV_OPA_100,0);
						
//						flashflaginsel=1;
				}

        void show_oj_win3(){//输出选择

            levelflag = 3;
            idflag=3;					  
//					WrBuff[18]=0x38;
            /**********       输出选择                         */
            ojw3 = lv_obj_create(NULL);
            lv_scr_load(ojw3);
            lv_obj_add_style(ojw3, & stylebk, LV_STATE_DEFAULT);
            lv_obj_clear_flag(ojw3, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
            /* 黑面板 */
            oj = lv_obj_create(ojw3);
            lv_obj_set_size(oj,800, 218);//在主屏幕下创建一个等大的对象屏幕
            lv_obj_set_style_radius(oj,0,0);//设置容器圆角为直角
            lv_obj_align(oj, LV_ALIGN_CENTER, 0, 0);
            lv_obj_add_style(oj, & stylebk, LV_STATE_DEFAULT);
            lv_obj_clear_flag(oj, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
					
            /* 大标题标签 */
            lb=lv_label_create(ojw3);
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "输出选择");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_align_to(lb,ojw3,LV_ALIGN_CENTER,0,-105);
            //lv_obj_align_to(menuoj,lb,LV_ALIGN_CENTER,0,150);
            /* 返回按钮 */
            wbt = lv_obj_create(ojw3);//menuback
            lv_obj_set_size(wbt,150,50);
            lv_obj_align(wbt, LV_ALIGN_TOP_LEFT,  10, 125);
            lv_obj_add_style(wbt, &style, 0);//透明化
						lv_obj_add_style(wbt, &style2, LV_STATE_FOCUSED);
						lv_obj_set_style_radius(wbt,0,0);//设置容器圆角为直角
            lv_obj_add_event_cb(wbt, event_cbback, LV_EVENT_CLICKED, NULL);//添加事件
            lv_obj_add_event_cb(wbt, event_cbback, LV_EVENT_FOCUSED, NULL);//添加事件
            lv_obj_clear_flag(wbt, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
            /* 返回图标 */
            ojimg =lv_img_create(wbt);
            lv_img_set_src(ojimg, &back); /* 设置图片源 */
            lv_obj_align_to(ojimg,wbt,LV_ALIGN_CENTER,-50,0);
            /* 返回标签 */
            wlbback=lv_label_create(wbt);//按钮中的标签
            lv_obj_set_style_text_font(wlbback,&lv_font_msyh_23_num,0);
            lv_label_set_text(wlbback, "设置");
            lv_label_set_recolor(wlbback,true);//重新着色
            lv_obj_add_style(wlbback, &styletext2, LV_STATE_DEFAULT);
            lv_obj_align_to(wlbback,wbt, LV_ALIGN_CENTER,0,0);

            /**********       面板 输出选择                           */
            menuoj = lv_obj_create(oj);
            lv_obj_set_size(menuoj,750,120);
            lv_obj_set_scroll_dir(menuoj, LV_DIR_VER);
            lv_obj_set_style_border_color(menuoj, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
            lv_obj_set_style_bg_color(menuoj, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
            lv_obj_align_to(menuoj,oj,LV_ALIGN_CENTER,0,50);
            lv_obj_clear_flag(menuoj, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
						lv_obj_set_style_radius(menuoj,0,0);//设置容器圆角为直角
						
            int a,b=0;
            /* 线条 */
            for(a=1;a<=1;a++){
            line=lv_obj_create(menuoj);
            lv_obj_set_style_bg_color(line, lv_color_hex(0x2A2829), LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(line, lv_color_hex(0x2A2829), LV_STATE_DEFAULT);
            lv_obj_set_size(line,720,1);
            if(a==1)
            lv_obj_align_to(line, menuoj,LV_ALIGN_TOP_MID, 5, 40);
            else lv_obj_align_to(line, menuoj,LV_ALIGN_TOP_MID, 5, 40+b);
            b+=60;
            }

            /**********       耳放输出界面                         */
            w8bt1 = lv_obj_create(menuoj);
            lv_obj_set_size(w8bt1,740,45);
            lv_obj_align_to(w8bt1, menuoj, LV_ALIGN_TOP_LEFT, -15, -15);
            lv_obj_add_style(w8bt1, &style, 0);
            lv_obj_add_style(w8bt1, &style2, LV_STATE_FOCUSED);
            lv_obj_clear_flag(w8bt1, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条

            //开关1
            w8switch1=lv_switch_create(w8bt1);
            lv_obj_set_size(w8switch1,60, 30);
            lv_obj_set_style_bg_color(w8switch1, lv_color_hex(0x999999), LV_PART_MAIN);
            lv_obj_align_to(w8switch1,w8bt1, LV_ALIGN_LEFT_MID,645,0);
       
            lb=lv_label_create(w8bt1);//标题1标签
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "耳放输出");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_add_event_cb(w8bt1, w8_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
            lv_obj_add_event_cb(w8bt1, w8_event_cb, LV_EVENT_CLICKED, NULL);/* 添加事件 */
            lv_obj_align_to(lb,w8bt1, LV_ALIGN_TOP_LEFT,-5,-10);

            wlb1=lv_label_create(oj);
            lv_obj_set_style_text_font(wlb1,&lv_font_msyh_23_num,0);
            lv_label_set_text(wlb1, "输出选择");
            lv_label_set_recolor(wlb1,true);//重新着色
            lv_obj_add_style(wlb1, &styletext2, LV_STATE_DEFAULT);
            lv_obj_align_to(wlb1,lb,LV_ALIGN_LEFT_MID,0,-50);

            /**********       RCA-XLR输出界面                         */
            w8bt2 = lv_obj_create(menuoj);
            lv_obj_set_size(w8bt2,740,45);
            lv_obj_align_to(w8bt2, menuoj, LV_ALIGN_TOP_LEFT, -15, 45);
            lv_obj_add_style(w8bt2, &style, 0);
            lv_obj_add_style(w8bt2, &style2, LV_STATE_FOCUSED);
            lv_obj_clear_flag(w8bt2, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条

            //开关2
            w8switch2=lv_switch_create(w8bt2);
            lv_obj_set_size(w8switch2,60, 30);
            lv_obj_set_style_bg_color(w8switch2, lv_color_hex(0x999999), LV_PART_MAIN);
            lv_obj_align_to(w8switch2,w8bt2, LV_ALIGN_LEFT_MID,645,0);


            lb=lv_label_create(w8bt2);//标题1标签
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "RCA-XLR输出");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_add_event_cb(w8bt2, w8_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
            lv_obj_add_event_cb(w8bt2, w8_event_cb, LV_EVENT_CLICKED, NULL);/* 添加事件 */
            lv_obj_align_to(lb,w8bt2, LV_ALIGN_TOP_LEFT,-5,-10);

            g8 = lv_group_create();//创建组
            //lv_group_set_default(g8);//如果部件已加入默认组，使用这种配置 设置默认组
            lv_group_add_obj(g8,wbt);
            lv_group_add_obj(g8,w8bt1);
            lv_group_add_obj(g8,w8bt2);


            lv_indev_set_group(indev_keypad, g8);//关联按键和组


            //lv_group_focus_obj(wbt);//聚焦
						outgpio=2*GPIO_GetBit(GPIOM, PIN5)+GPIO_GetBit(GPION, PIN0);
						 if(outgpio==0)WrBuff[12]=3;//0 0
						 else if(outgpio==1)WrBuff[12]=2;//0 1
						 else if(outgpio==2)WrBuff[12]=1;//1 0
						 else if(outgpio==3)WrBuff[12]=0;//1 1
						 flashflaglvbo=1;
						if(WrBuff[12]==0){lv_obj_add_state(w8switch1, LV_STATE_CHECKED);lv_obj_add_state(w8switch2, LV_STATE_CHECKED);}
						else if(WrBuff[12]==1){lv_obj_add_state(w8switch1, LV_STATE_CHECKED);lv_obj_clear_state(w8switch2, LV_STATE_CHECKED);}
						else if(WrBuff[12]==2){lv_obj_clear_state(w8switch1, LV_STATE_CHECKED);lv_obj_add_state(w8switch2, LV_STATE_CHECKED);}
						else if(WrBuff[12]==3){lv_obj_clear_state(w8switch1, LV_STATE_CHECKED);lv_obj_clear_state(w8switch2, LV_STATE_CHECKED);}
						
						

        }

        

        void show_oj_win5(){//9039MPRO设置
            levelflag = 3;
            idflag=5;
//						WrBuff[18]=0x33;
            /**********       9039MPRO设置                         */
            ojw5 = lv_obj_create(NULL);
            lv_scr_load(ojw5);
            lv_obj_add_style(ojw5, & stylebk, LV_STATE_DEFAULT);
            lv_obj_clear_flag(ojw5, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
            /* 黑面板 */
            oj = lv_obj_create(ojw5);
            lv_obj_set_size(oj,770, 218);//在主屏幕下创建一个等大的对象屏幕
            lv_obj_set_style_radius(oj,0,0);//设置容器圆角为直角
            lv_obj_align(oj, LV_ALIGN_CENTER, 0, 40);
            lv_obj_add_style(oj, & stylebk, LV_STATE_DEFAULT);
						lv_obj_add_style(oj, &scrollstyle, LV_PART_SCROLLBAR);
					
            /* 大标题标签 */
            lb=lv_label_create(ojw5);
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "9039MPRO设置");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_align_to(lb,ojw5,LV_ALIGN_CENTER,0,-105);
            //lv_obj_align_to(menuoj,lb,LV_ALIGN_CENTER,0,150);
            /* 返回按钮 */
            wbt = lv_obj_create(ojw5);//menuback
            lv_obj_set_size(wbt,150,50);
            lv_obj_align(wbt, LV_ALIGN_TOP_LEFT,  10, 125);
            lv_obj_add_style(wbt, &style, 0);//透明化
						lv_obj_add_style(wbt, &style2, LV_STATE_FOCUSED);
						lv_obj_set_style_radius(wbt,0,0);//设置容器圆角为直角
            lv_obj_add_event_cb(wbt, event_cbback, LV_EVENT_CLICKED, NULL);//添加事件
            lv_obj_add_event_cb(wbt, event_cbback, LV_EVENT_FOCUSED, NULL);//添加事件
            lv_obj_clear_flag(wbt, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
            /* 返回图标 */
            ojimg =lv_img_create(wbt);
            lv_img_set_src(ojimg, &back); /* 设置图片源 */
            lv_obj_align_to(ojimg,wbt,LV_ALIGN_CENTER,-50,0);
            /* 返回标签 */
            wlbback=lv_label_create(wbt);//按钮中的标签
            lv_obj_set_style_text_font(wlbback,&lv_font_msyh_23_num,0);
            lv_label_set_text(wlbback, "设置");
            lv_label_set_recolor(wlbback,true);//重新着色
            lv_obj_add_style(wlbback, &styletext2, LV_STATE_DEFAULT);
            lv_obj_align_to(wlbback,wbt, LV_ALIGN_CENTER,0,0);

//            /* SRC设置 */
//            w3bt1 = lv_obj_create(oj);
//            lv_obj_set_size(w3bt1,750, 60);
//            lv_obj_set_scroll_dir(w3bt1, LV_DIR_VER);
//            lv_obj_set_style_border_color(w3bt1, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
//            lv_obj_set_style_bg_color(w3bt1, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
//            lv_obj_add_style(w3bt1, &stylesli2, LV_STATE_FOCUSED);
//            lv_obj_add_style(w3bt1, &stylesli3, LV_STATE_PRESSED);
//            lv_obj_add_event_cb(w3bt1, w3_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
//            lv_obj_add_event_cb(w3bt1, w3_event_cb, LV_EVENT_CLICKED, NULL);/* 添加事件 */

//            lv_obj_align_to(w3bt1,oj,LV_ALIGN_CENTER,0,20);
//            lv_obj_clear_flag(w3bt1, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条

//            lb=lv_label_create(w3bt1);//标题1标签
//            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
//            lv_label_set_text(lb, "ASRC");
//            lv_label_set_recolor(lb,true);//重新着色
//            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
//            lv_obj_align_to(lb,w3bt1, LV_ALIGN_LEFT_MID,0,0);

//            wlb1=lv_label_create(oj);
//            lv_obj_set_style_text_font(wlb1,&lv_font_msyh_23_num,0);
//            lv_label_set_text(wlb1, "SRC设置");
//            lv_label_set_recolor(wlb1,true);//重新着色
//            lv_obj_add_style(wlb1, &styletext2, LV_STATE_DEFAULT);
//            lv_obj_align_to(wlb1,lb,LV_ALIGN_LEFT_MID,0,-50);

//            wlb2=lv_label_create(oj);
//            lv_obj_set_style_text_font(wlb2,&lv_font_msyh_23_num,0);
//            lv_label_set_text(wlb2, "设置9039MPRO是否启用SRC采样率转化器");
//            lv_label_set_recolor(wlb2,true);//重新着色
//            lv_obj_add_style(wlb2, &styletext2, LV_STATE_DEFAULT);
//            lv_obj_align_to(wlb2,lb,LV_ALIGN_LEFT_MID,0,50);
//            //开关1
//            w3switch1=lv_switch_create(w3bt1);
//            lv_obj_set_size(w3switch1,60, 30);
//            lv_obj_set_style_bg_color(w3switch1, lv_color_hex(0x999999), LV_PART_MAIN);
//            lv_obj_align_to(w3switch1,w3bt1, LV_ALIGN_LEFT_MID,650,0);
//						lv_obj_add_state(w3switch1, LV_STATE_CHECKED);
						
            /* ASYNC模式设置 */
            w3bt2 = lv_obj_create(oj);
            lv_obj_set_size(w3bt2,715, 60);
            lv_obj_set_scroll_dir(w3bt2, LV_DIR_VER);
            lv_obj_set_style_border_color(w3bt2, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
            lv_obj_set_style_bg_color(w3bt2, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
            lv_obj_add_style(w3bt2, &stylesli2, LV_STATE_FOCUSED);
            lv_obj_add_style(w3bt2, &stylesli3, LV_STATE_PRESSED);
            lv_obj_add_event_cb(w3bt2, w3_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
            lv_obj_add_event_cb(w3bt2, w3_event_cb, LV_EVENT_CLICKED, NULL);/* 添加事件 */

            lv_obj_align_to(w3bt2,oj,LV_ALIGN_CENTER,-5,-20);
            lv_obj_clear_flag(w3bt2, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
						lv_obj_set_style_radius(w3bt2,0,0);//设置容器圆角为直角

            lb=lv_label_create(w3bt2);//标题1标签
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "ASYNC");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_align_to(lb,w3bt2, LV_ALIGN_LEFT_MID,0,0);

            wlb1=lv_label_create(oj);
            lv_obj_set_style_text_font(wlb1,&lv_font_msyh_23_num,0);
            lv_label_set_text(wlb1, "ASYNC模式设置 ");
            lv_label_set_recolor(wlb1,true);//重新着色
            lv_obj_add_style(wlb1, &styletext2, LV_STATE_DEFAULT);
            lv_obj_align_to(wlb1,lb,LV_ALIGN_LEFT_MID,0,-50);

            wlb2=lv_label_create(oj);
            lv_obj_set_style_text_font(wlb2,&lv_font_msyh_23_num,0);
            lv_label_set_text(wlb2, "设置9039MPRO是否启用异步模式");
            lv_label_set_recolor(wlb2,true);//重新着色
            lv_obj_add_style(wlb2, &styletext2, LV_STATE_DEFAULT);
            lv_obj_align_to(wlb2,lb,LV_ALIGN_LEFT_MID,0,60);
            //开关2
            w3switch2=lv_switch_create(w3bt2);
            lv_obj_set_size(w3switch2,60, 30);
            lv_obj_set_style_bg_color(w3switch2, lv_color_hex(0x999999), LV_PART_MAIN);
            lv_obj_align_to(w3switch2,w3bt2, LV_ALIGN_LEFT_MID,620,0);
						if(WrBuff[8]==0)lv_obj_add_state(w3switch2, LV_STATE_CHECKED);
						else lv_obj_clear_state(w3switch2, LV_STATE_CHECKED);
						
            /* ASYNC模式设置 */
            w3bt3 = lv_obj_create(oj);
            lv_obj_set_size(w3bt3,715, 60);
            lv_obj_set_scroll_dir(w3bt3, LV_DIR_VER);
            lv_obj_set_style_border_color(w3bt3, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
            lv_obj_set_style_bg_color(w3bt3, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
            lv_obj_add_style(w3bt3, &stylesli2, LV_STATE_FOCUSED);
            lv_obj_add_style(w3bt3, &stylesli3, LV_STATE_PRESSED);
            lv_obj_add_event_cb(w3bt3, w3_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
            lv_obj_add_event_cb(w3bt3, w3_event_cb, LV_EVENT_CLICKED, NULL);/* 添加事件 */

            lv_obj_align_to(w3bt3,oj,LV_ALIGN_CENTER,-5,168);
            lv_obj_clear_flag(w3bt3, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
						lv_obj_set_style_radius(w3bt3,0,0);//设置容器圆角为直角

            lb=lv_label_create(w3bt3);//标题1标签
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "16xIIR Filter");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_align_to(lb,w3bt3, LV_ALIGN_LEFT_MID,0,0);

            wlb1=lv_label_create(oj);
            lv_obj_set_style_text_font(wlb1,&lv_font_msyh_23_num,0);
            lv_label_set_text(wlb1, "ASYNC模式设置 ");
            lv_label_set_recolor(wlb1,true);//重新着色
            lv_obj_add_style(wlb1, &styletext2, LV_STATE_DEFAULT);
            lv_obj_align_to(wlb1,lb,LV_ALIGN_LEFT_MID,0,-50);

            wlb2=lv_label_create(oj);
            lv_obj_set_style_text_font(wlb2,&lv_font_msyh_23_num,0);
            lv_label_set_text(wlb2, "设置9039MPRO是否启用16xIIR滤波器");
            lv_label_set_recolor(wlb2,true);//重新着色
            lv_obj_add_style(wlb2, &styletext2, LV_STATE_DEFAULT);
            lv_obj_align_to(wlb2,lb,LV_ALIGN_LEFT_MID,0,60);
            //开关3
            w3switch3=lv_switch_create(w3bt3);
            lv_obj_set_size(w3switch3,60, 30);
            lv_obj_set_style_bg_color(w3switch3, lv_color_hex(0x999999), LV_PART_MAIN);
            lv_obj_align_to(w3switch3,w3bt3, LV_ALIGN_LEFT_MID,620,0);

            /**********       面板1 SOF超采样滤波器                           */
            menuoj = lv_obj_create(oj);
            lv_obj_set_size(menuoj,715,120);
            lv_obj_set_scroll_dir(menuoj, LV_DIR_VER);
            lv_obj_set_style_border_color(menuoj, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
            lv_obj_set_style_bg_color(menuoj, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
            lv_obj_align_to(menuoj,oj,LV_ALIGN_CENTER,-5,392);
            lv_obj_clear_flag(menuoj, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
						lv_obj_set_style_radius(menuoj,0,0);//设置容器圆角为直角

            int a,b=0;
            /* 线条 */
            for(a=1;a<=1;a++){
            line=lv_obj_create(menuoj);
            lv_obj_set_style_bg_color(line, lv_color_hex(0x2A2829), LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(line, lv_color_hex(0x2A2829), LV_STATE_DEFAULT);
            lv_obj_set_size(line,690,1);
            if(a==1)
            lv_obj_align_to(line, menuoj,LV_ALIGN_TOP_MID, 5, 40);
            else lv_obj_align_to(line, menuoj,LV_ALIGN_TOP_MID, 5, 40+b);
            b+=60;
            }

                /**********       2xOSF                           */
                w3bt4 = lv_obj_create(menuoj);
                lv_obj_set_size(w3bt4,700,45);
                lv_obj_align_to(w3bt4, menuoj, LV_ALIGN_TOP_LEFT, -15, -15);
                lv_obj_add_style(w3bt4, &style, 0);
                lv_obj_add_style(w3bt4, &style2, LV_STATE_FOCUSED);
                lv_obj_clear_flag(w3bt4, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
								lv_obj_set_style_radius(w3bt4,0,0);//设置容器圆角为直角
						
                //开关4
                w3switch4=lv_switch_create(w3bt4);
                lv_obj_set_size(w3switch4,60, 30);
                lv_obj_set_style_bg_color(w3switch4, lv_color_hex(0x999999), LV_PART_MAIN);
                lv_obj_align_to(w3switch4,w3bt4, LV_ALIGN_LEFT_MID,610,0);

						
                lb=lv_label_create(w3bt4);//标题1标签
                lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
                lv_label_set_text(lb, "2xOSF");
                lv_label_set_recolor(lb,true);//重新着色
                lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
                lv_obj_add_event_cb(w3bt4, w3_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
                lv_obj_add_event_cb(w3bt4, w3_event_cb, LV_EVENT_CLICKED, NULL);/* 添加事件 */
                lv_obj_align_to(lb,w3bt4, LV_ALIGN_TOP_LEFT,-5,-10);

                wlb1=lv_label_create(oj);
                lv_obj_set_style_text_font(wlb1,&lv_font_msyh_23_num,0);
                lv_label_set_text(wlb1, "SOF超采样滤波器");
                lv_label_set_recolor(wlb1,true);//重新着色
                lv_obj_add_style(wlb1, &styletext2, LV_STATE_DEFAULT);
                lv_obj_align_to(wlb1,lb,LV_ALIGN_LEFT_MID,0,-50);

                /**********       4xOSF                           */
                w3bt5 = lv_obj_create(menuoj);
                lv_obj_set_size(w3bt5,700,45);
                lv_obj_align_to(w3bt5, menuoj, LV_ALIGN_TOP_LEFT, -15, 45);
                lv_obj_add_style(w3bt5, &style, 0);
                lv_obj_add_style(w3bt5, &style2, LV_STATE_FOCUSED);
                lv_obj_clear_flag(w3bt5, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
								lv_obj_set_style_radius(w3bt5,0,0);//设置容器圆角为直角

                //开关5
                w3switch5=lv_switch_create(w3bt5);
                lv_obj_set_size(w3switch5,60, 30);
                lv_obj_set_style_bg_color(w3switch5, lv_color_hex(0x999999), LV_PART_MAIN);
                lv_obj_align_to(w3switch5,w3bt5, LV_ALIGN_LEFT_MID,610,0);


                lb=lv_label_create(w3bt5);//标题1标签
                lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
                lv_label_set_text(lb, "4xOSF");
                lv_label_set_recolor(lb,true);//重新着色
                lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
                lv_obj_add_event_cb(w3bt5, w3_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
                lv_obj_add_event_cb(w3bt5, w3_event_cb, LV_EVENT_CLICKED, NULL);/* 添加事件 */
                lv_obj_align_to(lb,w3bt5, LV_ALIGN_TOP_LEFT,-5,-10);

                 
						/* 相位反转 */
						w3bt6 = lv_obj_create(oj);
						lv_obj_set_size(w3bt6,715, 60);
						lv_obj_set_scroll_dir(w3bt6, LV_DIR_VER);
						lv_obj_set_style_border_color(w3bt6, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
						lv_obj_set_style_bg_color(w3bt6, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
						lv_obj_add_style(w3bt6, &stylesli2, LV_STATE_FOCUSED);
						lv_obj_add_style(w3bt6, &stylesli3, LV_STATE_PRESSED);
						lv_obj_add_event_cb(w3bt6, w3_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
						lv_obj_add_event_cb(w3bt6, w3_event_cb, LV_EVENT_CLICKED, NULL);/* 添加事件 */
						lv_obj_set_style_radius(w3bt6,0,0);//设置容器圆角为直角
						
						lv_obj_align_to(w3bt6,oj,LV_ALIGN_CENTER,0,553);
						lv_obj_clear_flag(w3bt6, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条

						lb=lv_label_create(w3bt6);//标题1标签
						lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
						lv_label_set_text(lb, "相位反转");
						lv_label_set_recolor(lb,true);//重新着色
						lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
						lv_obj_align_to(lb,w3bt6, LV_ALIGN_LEFT_MID,0,0);

						wlb1=lv_label_create(oj);
						lv_obj_set_style_text_font(wlb1,&lv_font_msyh_23_num,0);
						lv_label_set_text(wlb1, "相位反转开关");
						lv_label_set_recolor(wlb1,true);//重新着色
						lv_obj_add_style(wlb1, &styletext2, LV_STATE_DEFAULT);
						lv_obj_align_to(wlb1,lb,LV_ALIGN_LEFT_MID,0,-50);

						wlb2=lv_label_create(oj);
						lv_obj_set_style_text_font(wlb2,&lv_font_msyh_23_num,0);
						lv_label_set_text(wlb2, "设置9039MPRO是否启用相位反转");
						lv_label_set_recolor(wlb2,true);//重新着色
						lv_obj_add_style(wlb2, &styletext2, LV_STATE_DEFAULT);
						lv_obj_align_to(wlb2,lb,LV_ALIGN_LEFT_MID,0,60);
						//开关6
						w3switch6=lv_switch_create(w3bt6);
						lv_obj_set_size(w3switch6,60, 30);
						lv_obj_set_style_bg_color(w3switch6, lv_color_hex(0x999999), LV_PART_MAIN);
						lv_obj_align_to(w3switch6,w3bt6, LV_ALIGN_LEFT_MID,620,0);
						//lv_obj_add_state(w3switch6, LV_STATE_CHECKED);
						
					  /* 黑面板 */
            w2black = lv_obj_create(oj);
            lv_obj_set_size(w2black,710,80);
            lv_obj_set_style_radius(w2black,0,0);//设置容器圆角为直角
            lv_obj_align_to(w2black,oj,LV_ALIGN_CENTER,0,838);
            lv_obj_add_style(w2black, & stylebk, LV_STATE_DEFAULT);
						
						if(WrBuff[13]==1)lv_obj_add_state(w3switch6, LV_STATE_CHECKED);
						else lv_obj_clear_state(w3switch6, LV_STATE_CHECKED);

						if(WrBuff[9]==0){	
						lv_obj_add_state(w3switch3, LV_STATE_CHECKED);
						lv_obj_add_state(w3switch4, LV_STATE_CHECKED);
						lv_obj_add_state(w3switch5, LV_STATE_CHECKED);
						}
						else if(WrBuff[9]==1){
						lv_obj_add_state(w3switch3, LV_STATE_CHECKED);
						lv_obj_add_state(w3switch5, LV_STATE_CHECKED);
						lv_obj_clear_state(w3switch4, LV_STATE_CHECKED);
						}
						else if(WrBuff[9]==2){
						lv_obj_add_state(w3switch3, LV_STATE_CHECKED);
						lv_obj_clear_state(w3switch5, LV_STATE_CHECKED);
						lv_obj_add_state(w3switch4, LV_STATE_CHECKED);
						}
						else if(WrBuff[9]==3){
						lv_obj_add_state(w3switch3, LV_STATE_CHECKED);
						lv_obj_clear_state(w3switch5, LV_STATE_CHECKED);
						lv_obj_clear_state(w3switch4, LV_STATE_CHECKED);
						}
						else if(WrBuff[9]==4){
						lv_obj_clear_state(w3switch3, LV_STATE_CHECKED);
						lv_obj_add_state(w3switch5, LV_STATE_CHECKED);
						lv_obj_add_state(w3switch4, LV_STATE_CHECKED);
						}						
						else if(WrBuff[9]==5){
						lv_obj_clear_state(w3switch3, LV_STATE_CHECKED);
						lv_obj_add_state(w3switch5, LV_STATE_CHECKED);
						lv_obj_clear_state(w3switch4, LV_STATE_CHECKED);
						}
						else if(WrBuff[9]==6){
						lv_obj_add_state(w3switch3, LV_STATE_CHECKED);
						lv_obj_clear_state(w3switch5, LV_STATE_CHECKED);
						lv_obj_clear_state(w3switch4, LV_STATE_CHECKED);
						}
						else if(WrBuff[9]==7){
						lv_obj_clear_state(w3switch3, LV_STATE_CHECKED);
						lv_obj_clear_state(w3switch5, LV_STATE_CHECKED);
						lv_obj_clear_state(w3switch4, LV_STATE_CHECKED);
						}

						
            g3 = lv_group_create();//创建组
            //lv_group_set_default(g3);//如果部件已加入默认组，使用这种配置 设置默认组
            lv_group_add_obj(g3,wbt);
//            lv_group_add_obj(g3,w3bt1);
            lv_group_add_obj(g3,w3bt2);
            lv_group_add_obj(g3,w3bt3);
            lv_group_add_obj(g3,w3bt4);
            lv_group_add_obj(g3,w3bt5);
						lv_group_add_obj(g3,w3bt6);
						
            lv_indev_set_group(indev_keypad, g3);//关联按键和组


            //lv_group_focus_obj(wbt);//聚焦
//						flashflaginsel=1;
}


				void show_oj_win4(){//本地全局飞秒时钟
            levelflag=3;
            idflag=4;
//						WrBuff[18]=0x34;
            /**********       本地全局飞秒时钟                         */
            ojw4 = lv_obj_create(NULL);
            lv_scr_load(ojw4);
            lv_obj_add_style(ojw4, & stylebk, LV_STATE_DEFAULT);
            lv_obj_clear_flag(ojw4, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
            /* 黑面板 */
            oj = lv_obj_create(ojw4);
            lv_obj_set_size(oj,800, 218);//在主屏幕下创建一个等大的对象屏幕
            lv_obj_set_style_radius(oj,0,0);//设置容器圆角为直角
            lv_obj_align(oj, LV_ALIGN_CENTER, 0, 0);
            lv_obj_add_style(oj, & stylebk, LV_STATE_DEFAULT);

            /* 大标题标签 */
            lb=lv_label_create(ojw4);
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "本地全局飞秒时钟");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_align_to(lb,ojw4,LV_ALIGN_CENTER,0,-105);
            //lv_obj_align_to(menuoj,lb,LV_ALIGN_CENTER,0,150);
            /* 返回按钮 */
            wbt = lv_obj_create(ojw4);//menuback
            lv_obj_set_size(wbt,150,50);
            lv_obj_align(wbt, LV_ALIGN_TOP_LEFT,  10, 125);
            lv_obj_add_style(wbt, &style, 0);//透明化
						lv_obj_add_style(wbt, &style2, LV_STATE_FOCUSED);
						lv_obj_set_style_radius(wbt,0,0);//设置容器圆角为直角
            lv_obj_add_event_cb(wbt, event_cbback, LV_EVENT_CLICKED, NULL);//添加事件
            lv_obj_add_event_cb(wbt, event_cbback, LV_EVENT_FOCUSED, NULL);//添加事件
            lv_obj_clear_flag(wbt, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
            /* 返回图标 */
            ojimg =lv_img_create(wbt);
            lv_img_set_src(ojimg, &back); /* 设置图片源 */
            lv_obj_align_to(ojimg,wbt,LV_ALIGN_CENTER,-50,0);
            /* 返回标签 */
            wlbback=lv_label_create(wbt);//按钮中的标签
            lv_obj_set_style_text_font(wlbback,&lv_font_msyh_23_num,0);
            lv_label_set_text(wlbback, "设置");
            lv_label_set_recolor(wlbback,true);//重新着色
            lv_obj_add_style(wlbback, &styletext2, LV_STATE_DEFAULT);
            lv_obj_align_to(wlbback,wbt, LV_ALIGN_CENTER,0,0);

            /* 8422设置 */
            w4bt1 = lv_obj_create(oj);
            lv_obj_set_size(w4bt1,750, 60);
            lv_obj_set_scroll_dir(w4bt1, LV_DIR_VER);
            lv_obj_set_style_border_color(w4bt1, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
            lv_obj_set_style_bg_color(w4bt1, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
            lv_obj_add_style(w4bt1, &stylesli2, LV_STATE_FOCUSED);
            lv_obj_add_style(w4bt1, &stylesli3, LV_STATE_PRESSED);
            lv_obj_add_event_cb(w4bt1, w3_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
            lv_obj_add_event_cb(w4bt1, w3_event_cb, LV_EVENT_CLICKED, NULL);/* 添加事件 */

            lv_obj_align_to(w4bt1,oj,LV_ALIGN_CENTER,0,20);
            lv_obj_clear_flag(w4bt1, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
						lv_obj_set_style_radius(w4bt1,0,0);//设置容器圆角为直角

            lb=lv_label_create(w4bt1);//标题1标签
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "8422ASYNC");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_align_to(lb,w4bt1, LV_ALIGN_LEFT_MID,0,0);

            wlb1=lv_label_create(oj);
            lv_obj_set_style_text_font(wlb1,&lv_font_msyh_23_num,0);
            lv_label_set_text(wlb1, "8422设置");
            lv_label_set_recolor(wlb1,true);//重新着色
            lv_obj_add_style(wlb1, &styletext2, LV_STATE_DEFAULT);
            lv_obj_align_to(wlb1,lb,LV_ALIGN_LEFT_MID,0,-50);

            wlb2=lv_label_create(oj);
            lv_obj_set_style_text_font(wlb2,&lv_font_msyh_23_num,0);
            lv_label_set_text(wlb2, "配置是否启用本地全局飞秒时钟");
            lv_label_set_recolor(wlb2,true);//重新着色
            lv_obj_add_style(wlb2, &styletext2, LV_STATE_DEFAULT);
            lv_obj_align_to(wlb2,lb,LV_ALIGN_LEFT_MID,0,60);
            //开关1
            w4switch1=lv_switch_create(w4bt1);
            lv_obj_set_size(w4switch1,60, 30);
            lv_obj_set_style_bg_color(w4switch1, lv_color_hex(0x999999), LV_PART_MAIN);
            lv_obj_align_to(w4switch1,w4bt1, LV_ALIGN_LEFT_MID,650,0);
						if(WrBuff[10]==0)lv_obj_add_state(w4switch1, LV_STATE_CHECKED);
						else lv_obj_clear_state(w4switch1, LV_STATE_CHECKED);

            g4 = lv_group_create();//创建组
            //lv_group_set_default(g3);//如果部件已加入默认组，使用这种配置 设置默认组
            lv_group_add_obj(g4,wbt);
            lv_group_add_obj(g4,w4bt1);

            lv_indev_set_group(indev_keypad, g4);//关联按键和组

            //lv_group_focus_obj(wbt);//聚焦
//						flashflaginsel=1;
}

				void show_oj_win2(){//滤波器选择
            levelflag=3;
            idflag=2;
//						WrBuff[18]=0x35;
            /**********       滤波器选择                         */
            ojw2 = lv_obj_create(NULL);
            lv_scr_load(ojw2);
            lv_obj_add_style(ojw2, & stylebk, LV_STATE_DEFAULT);
            lv_obj_clear_flag(ojw2, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
            /* 黑面板 */
            oj = lv_obj_create(ojw2);
            lv_obj_set_size(oj,800, 218);//在主屏幕下创建一个等大的对象屏幕
            lv_obj_set_style_radius(oj,0,0);//设置容器圆角为直角
            lv_obj_align(oj, LV_ALIGN_CENTER, 0, 0);
            lv_obj_add_style(oj, & stylebk, LV_STATE_DEFAULT);

            /* 大标题标签 */
            lb=lv_label_create(ojw2);
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "滤波器选择");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_align_to(lb,ojw2,LV_ALIGN_CENTER,0,-105);
            //lv_obj_align_to(menuoj,lb,LV_ALIGN_CENTER,0,150);
            /* 返回按钮 */
            wbt = lv_obj_create(ojw2);//menuback
            lv_obj_set_size(wbt,150,50);
            lv_obj_align(wbt, LV_ALIGN_TOP_LEFT,  10, 125);
            lv_obj_add_style(wbt, &style, 0);//透明化
						lv_obj_add_style(wbt, &style2, LV_STATE_FOCUSED);
						lv_obj_set_style_radius(wbt,0,0);//设置容器圆角为直角
            lv_obj_add_event_cb(wbt, event_cbback, LV_EVENT_CLICKED, NULL);//添加事件
            lv_obj_add_event_cb(wbt, event_cbback, LV_EVENT_FOCUSED, NULL);//添加事件
            lv_obj_clear_flag(wbt, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
            /* 返回图标 */
            ojimg =lv_img_create(wbt);
            lv_img_set_src(ojimg, &back); /* 设置图片源 */
            lv_obj_align_to(ojimg,wbt,LV_ALIGN_CENTER,-50,0);
            /* 返回标签 */
            wlbback=lv_label_create(wbt);//按钮中的标签
            lv_obj_set_style_text_font(wlbback,&lv_font_msyh_23_num,0);
            lv_label_set_text(wlbback, "设置");
            lv_label_set_recolor(wlbback,true);//重新着色
            lv_obj_add_style(wlbback, &styletext2, LV_STATE_DEFAULT);
            lv_obj_align_to(wlbback,wbt, LV_ALIGN_CENTER,0,0);

            /* 滤波器选择对象 */
            w5bt1 = lv_obj_create(oj);
            lv_obj_set_size(w5bt1,750, 60);
            lv_obj_set_scroll_dir(w5bt1, LV_DIR_VER);
            lv_obj_set_style_border_color(w5bt1, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
            lv_obj_set_style_bg_color(w5bt1, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
            lv_obj_add_style(w5bt1, &stylesli2, LV_STATE_FOCUSED);
            lv_obj_add_style(w5bt1, &stylesli3, LV_STATE_PRESSED);
            lv_obj_add_style(w5bt1, &stylesli3, LV_STATE_EDITED);
            lv_obj_add_event_cb(w5bt1, w5_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
            lv_obj_add_event_cb(w5bt1, w5_event_cb, LV_EVENT_CLICKED, NULL);/* 添加事件 */

            lv_obj_align_to(w5bt1,oj,LV_ALIGN_CENTER,0,20);
            lv_obj_clear_flag(w5bt1, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
						lv_obj_set_style_radius(w5bt1,0,0);//设置容器圆角为直角
            
            lb=lv_label_create(w5bt1);//标题1标签
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "Filter");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_align_to(lb,w5bt1, LV_ALIGN_LEFT_MID,0,0);
            
            wlb1=lv_label_create(oj);
            lv_obj_set_style_text_font(wlb1,&lv_font_msyh_23_num,0);
            lv_label_set_text(wlb1, "滤波器");
            lv_label_set_recolor(wlb1,true);//重新着色
            lv_obj_add_style(wlb1, &styletext2, LV_STATE_DEFAULT);
            lv_obj_align_to(wlb1,lb,LV_ALIGN_LEFT_MID,0,-50);
            //底部标题
            wlb2=lv_label_create(oj);
            lv_obj_set_style_text_font(wlb2,&lv_font_msyh_23_num,0);
            lv_label_set_text(wlb2, "配置9039MPRO滤波器");
            lv_label_set_recolor(wlb2,true);//重新着色
            lv_obj_add_style(wlb2, &styletext2, LV_STATE_DEFAULT);
            lv_obj_align_to(wlb2,lb,LV_ALIGN_LEFT_MID,0,60);

            /* 文本框1 */
            menulb =lv_obj_create(w5bt1);
            lv_obj_set_size(menulb,300,30);
            lv_obj_align_to(menulb,w5bt1, LV_ALIGN_TOP_LEFT,415,-5);
            lv_obj_add_style(menulb, &style, 0);//透明化
            lv_obj_clear_flag(menulb, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
            w5lb=lv_label_create(w5bt1);
            lv_label_set_recolor(w5lb,true);//重新着色
            lv_obj_add_style(w5lb, &styletext2, LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(w5lb,&lv_font_msyh_23_num,0);
            //lv_label_set_text_fmt(w5lb, "7/%d 最小相位慢降降低色散",0);
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
            
            //lv_group_set_default(g5);//如果部件已加入默认组，使用这种配置 设置默认组
            g5 = lv_group_create();//创建组
            lv_group_add_obj(g5,wbt);
            lv_group_add_obj(g5,w5bt1);						
            lv_indev_set_group(indev_keypad, g5);//关联按键和组

            //lv_group_focus_obj(wbt);//聚焦
//						flashflaginsel=1;
}
        

				
				void show_oj_win7(){//平衡调节
            levelflag=3;
            idflag=7;
//						WrBuff[18]=0x36;
            /**********       平衡调节                         */
            ojw7 = lv_obj_create(NULL);
            lv_scr_load(ojw7);
            lv_obj_add_style(ojw7, & stylebk, LV_STATE_DEFAULT);
            lv_obj_clear_flag(ojw7, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
            /* 黑面板 */
            oj = lv_obj_create(ojw7);
            lv_obj_set_size(oj,800, 268);//在主屏幕下创建一个等大的对象屏幕
            lv_obj_set_style_radius(oj,0,0);//设置容器圆角为直角
            lv_obj_align(oj, LV_ALIGN_CENTER, 0, 0);
            lv_obj_add_style(oj, & stylebk, LV_STATE_DEFAULT);

            /* 大标题标签 */
            lb=lv_label_create(ojw7);
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "平衡调节");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_align_to(lb,ojw7,LV_ALIGN_CENTER,0,-105);
            //lv_obj_align_to(menuoj,lb,LV_ALIGN_CENTER,0,150);
            /* 返回按钮 */
            wbt = lv_obj_create(ojw7);//menuback
            lv_obj_set_size(wbt,150,50);
            lv_obj_align(wbt, LV_ALIGN_TOP_LEFT,  10, 125);
            lv_obj_add_style(wbt, &style, 0);//透明化
						lv_obj_add_style(wbt, &style2, LV_STATE_FOCUSED);
						lv_obj_set_style_radius(wbt,0,0);//设置容器圆角为直角
            lv_obj_add_event_cb(wbt, event_cbback, LV_EVENT_CLICKED, NULL);//添加事件
            lv_obj_add_event_cb(wbt, event_cbback, LV_EVENT_FOCUSED, NULL);//添加事件
            lv_obj_clear_flag(wbt, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
            /* 返回图标 */
            ojimg =lv_img_create(wbt);
            lv_img_set_src(ojimg, &back); /* 设置图片源 */
            lv_obj_align_to(ojimg,wbt,LV_ALIGN_CENTER,-50,0);
            /* 返回标签 */
            wlbback=lv_label_create(wbt);//按钮中的标签
            lv_obj_set_style_text_font(wlbback,&lv_font_msyh_23_num,0);
            lv_label_set_text(wlbback, "设置");
            lv_label_set_recolor(wlbback,true);//重新着色
            lv_obj_add_style(wlbback, &styletext2, LV_STATE_DEFAULT);
            lv_obj_align_to(wlbback,wbt, LV_ALIGN_CENTER,0,0);
                /**********       面板1 设置输出音量平衡                           */
            menuoj = lv_obj_create(oj);
            lv_obj_set_size(menuoj,750,120);
            lv_obj_set_scroll_dir(menuoj, LV_DIR_VER);
            lv_obj_set_style_border_color(menuoj, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
            lv_obj_set_style_bg_color(menuoj, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
            lv_obj_align_to(menuoj,oj,LV_ALIGN_CENTER,0,50);
            lv_obj_clear_flag(menuoj, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
						lv_obj_set_style_radius(menuoj,0,0);//设置容器圆角为直角
						
            int a,b=0;
            /* 线条 */
            for(a=1;a<=1;a++){
            line=lv_obj_create(menuoj);
            lv_obj_set_style_bg_color(line, lv_color_hex(0x2A2829), LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(line, lv_color_hex(0x2A2829), LV_STATE_DEFAULT);
            lv_obj_set_size(line,720,1);
            if(a==1)
            lv_obj_align_to(line, menuoj,LV_ALIGN_TOP_MID, 5, 40);
            else lv_obj_align_to(line, menuoj,LV_ALIGN_TOP_MID, 5, 40+b);
            b+=60;
            }

                /**********       L                           */
                w6bt1 = lv_obj_create(menuoj);
                lv_obj_set_size(w6bt1,740,45);
                lv_obj_align_to(w6bt1, menuoj, LV_ALIGN_TOP_LEFT, -15, -15);
						    lv_obj_set_style_border_color(w6bt1, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
								lv_obj_set_style_bg_color(w6bt1, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
                lv_obj_add_style(w6bt1, &stylesli2, LV_STATE_FOCUSED);
								lv_obj_add_style(w6bt1, &stylesli3, LV_STATE_EDITED);
                lv_obj_clear_flag(w6bt1, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
								lv_obj_set_style_radius(w6bt1,0,0);//设置容器圆角为直角
						
                lb=lv_label_create(w6bt1);//标题1标签
                lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
                lv_label_set_text(lb, "L");
                lv_label_set_recolor(lb,true);//重新着色
                lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
                lv_obj_add_event_cb(w6bt1, w6_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
                lv_obj_add_event_cb(w6bt1, w6_event_cb, LV_EVENT_CLICKED, NULL);/* 添加事件 */
                lv_obj_align_to(lb,w6bt1, LV_ALIGN_TOP_LEFT,10,-10);

                wlb1=lv_label_create(oj);
                lv_obj_set_style_text_font(wlb1,&lv_font_msyh_23_num,0);
                lv_label_set_text(wlb1, "设置输出音量平衡");
                lv_label_set_recolor(wlb1,true);//重新着色
                lv_obj_add_style(wlb1, &styletext2, LV_STATE_DEFAULT);
                lv_obj_align_to(wlb1,lb,LV_ALIGN_LEFT_MID,-20,-50);

                //左平衡图片
                limg=lv_img_create(w6bt1);
                //lv_img_set_src(limg, &BL1); /* 设置亮度 */
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
                /* 文本框1 */
                menulb =lv_obj_create(w6bt1);
                lv_obj_set_size(menulb,300,30);
                lv_obj_align_to(menulb,w6bt1, LV_ALIGN_TOP_LEFT,415,-5);
                lv_obj_add_style(menulb, &style, 0);//透明化
                lv_obj_clear_flag(menulb, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
                w6lbx1=lv_label_create(w6bt1);
                lv_label_set_recolor(w6lbx1,true);//重新着色
                lv_obj_add_style(w6lbx1, &styletext2, LV_STATE_DEFAULT);
                lv_obj_set_style_text_font(w6lbx1,&lv_font_msyh_23_num,0);
									if(soundBLtemp==-1){soundBLtemp=0;lv_label_set_text_fmt(w6lbx1, "%d", soundBLtemp);}
									else lv_label_set_text_fmt(w6lbx1, "%.1f", -soundBLtemp/2.0);
                //lv_label_set_text_fmt(w6lbx1, "%.1f",-1.0);
                lv_obj_align_to(w6lbx1,w6bt1, LV_ALIGN_RIGHT_MID,0,5);

                /**********       R                           */
                w6bt2 = lv_obj_create(menuoj);
                lv_obj_set_size(w6bt2,740,45);
                lv_obj_align_to(w6bt2, menuoj, LV_ALIGN_TOP_LEFT, -15, 45);
						    lv_obj_set_style_border_color(w6bt2, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
								lv_obj_set_style_bg_color(w6bt2, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
                lv_obj_add_style(w6bt2, &stylesli2, LV_STATE_FOCUSED);
								lv_obj_add_style(w6bt2, &stylesli3, LV_STATE_EDITED);
                lv_obj_clear_flag(w6bt2, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
								lv_obj_set_style_radius(w6bt2,0,0);//设置容器圆角为直角

                lb=lv_label_create(w6bt2);//标题1标签
                lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
                lv_label_set_text(lb, "R");
                lv_label_set_recolor(lb,true);//重新着色
                lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
                lv_obj_add_event_cb(w6bt2, w6_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
                lv_obj_add_event_cb(w6bt2, w6_event_cb, LV_EVENT_CLICKED, NULL);/* 添加事件 */
                lv_obj_align_to(lb,w6bt2, LV_ALIGN_TOP_LEFT,10,-10);

                //右平衡图片
                rimg=lv_img_create(w6bt2);
                //lv_img_set_src(rimg, &BL8); /* 设置亮度 */
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
                /* 文本框2 */
                menulb =lv_obj_create(w6bt2);
                lv_obj_set_size(menulb,300,30);
                lv_obj_align_to(menulb,w6bt2, LV_ALIGN_TOP_LEFT,415,-5);
                lv_obj_add_style(menulb, &style, 0);//透明化
                lv_obj_clear_flag(menulb, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
                w6lbx2=lv_label_create(w6bt2);
                lv_label_set_recolor(w6lbx2,true);//重新着色
                lv_obj_add_style(w6lbx2, &styletext2, LV_STATE_DEFAULT);
                lv_obj_set_style_text_font(w6lbx2,&lv_font_msyh_23_num,0);
									if(soundBRtemp==-1){soundBRtemp=0;lv_label_set_text_fmt(w6lbx2, "%d", soundBRtemp);}
									else lv_label_set_text_fmt(w6lbx2, "%.1f", -soundBRtemp/2.0);
                //lv_label_set_text_fmt(w6lbx2, "%.1f",-0);
                lv_obj_align_to(w6lbx2,w6bt2, LV_ALIGN_RIGHT_MID,0,5);

            g6 = lv_group_create();//创建组
            //lv_group_set_default(g3);//如果部件已加入默认组，使用这种配置 设置默认组
            lv_group_add_obj(g6,wbt);
            lv_group_add_obj(g6,w6bt1);
            lv_group_add_obj(g6,w6bt2);

									
            lv_indev_set_group(indev_keypad, g6);//关联按键和组

            //lv_group_focus_obj(wbt);//聚焦
									
//						flashflaginsel=1;
        }

 
				
				
				void show_oj_win8(){//左右输出互换
            levelflag=3;
            idflag=8;
//						WrBuff[18]=0x37;
            /**********       左右输出互换                         */
            ojw8 = lv_obj_create(NULL);
            lv_scr_load(ojw8);
            lv_obj_add_style(ojw8, & stylebk, LV_STATE_DEFAULT);
            lv_obj_clear_flag(ojw8, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
            /* 黑面板 */
            oj = lv_obj_create(ojw8);
            lv_obj_set_size(oj,800, 218);//在主屏幕下创建一个等大的对象屏幕
            lv_obj_set_style_radius(oj,0,0);//设置容器圆角为直角
            lv_obj_align(oj, LV_ALIGN_CENTER, 0, 0);
            lv_obj_add_style(oj, & stylebk, LV_STATE_DEFAULT);

            /* 大标题标签 */
            lb=lv_label_create(ojw8);
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "左右输出互换");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_align_to(lb,ojw8,LV_ALIGN_CENTER,0,-105);
            //lv_obj_align_to(menuoj,lb,LV_ALIGN_CENTER,0,150);
            /* 返回按钮 */
            wbt = lv_obj_create(ojw8);//menuback
            lv_obj_set_size(wbt,150,50);
            lv_obj_align(wbt, LV_ALIGN_TOP_LEFT,  10, 125);
            lv_obj_add_style(wbt, &style, 0);//透明化
						lv_obj_add_style(wbt, &style2, LV_STATE_FOCUSED);
						lv_obj_set_style_radius(wbt,0,0);//设置容器圆角为直角
            lv_obj_add_event_cb(wbt, event_cbback, LV_EVENT_CLICKED, NULL);//添加事件
            lv_obj_add_event_cb(wbt, event_cbback, LV_EVENT_FOCUSED, NULL);//添加事件
            lv_obj_clear_flag(wbt, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
            /* 返回图标 */
            ojimg =lv_img_create(wbt);
            lv_img_set_src(ojimg, &back); /* 设置图片源 */
            lv_obj_align_to(ojimg,wbt,LV_ALIGN_CENTER,-50,0);
            /* 返回标签 */
            wlbback=lv_label_create(wbt);//按钮中的标签
            lv_obj_set_style_text_font(wlbback,&lv_font_msyh_23_num,0);
            lv_label_set_text(wlbback, "设置");
            lv_label_set_recolor(wlbback,true);//重新着色
            lv_obj_add_style(wlbback, &styletext2, LV_STATE_DEFAULT);
            lv_obj_align_to(wlbback,wbt, LV_ALIGN_CENTER,0,0);

            /* L-R设置 */
            w7bt1 = lv_obj_create(oj);
            lv_obj_set_size(w7bt1,750, 60);
            lv_obj_set_scroll_dir(w7bt1, LV_DIR_VER);
            lv_obj_set_style_border_color(w7bt1, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
            lv_obj_set_style_bg_color(w7bt1, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
            lv_obj_add_style(w7bt1, &stylesli2, LV_STATE_FOCUSED);
            lv_obj_add_style(w7bt1, &stylesli3, LV_STATE_PRESSED);
            lv_obj_add_event_cb(w7bt1, w3_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
            lv_obj_add_event_cb(w7bt1, w3_event_cb, LV_EVENT_CLICKED, NULL);/* 添加事件 */

            lv_obj_align_to(w7bt1,oj,LV_ALIGN_CENTER,0,20);
            lv_obj_clear_flag(w7bt1, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
						lv_obj_set_style_radius(w7bt1,0,0);//设置容器圆角为直角

            lb=lv_label_create(w7bt1);//标题1标签
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "L-R");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_align_to(lb,w7bt1, LV_ALIGN_LEFT_MID,0,0);

            wlb1=lv_label_create(oj);
            lv_obj_set_style_text_font(wlb1,&lv_font_msyh_23_num,0);
            lv_label_set_text(wlb1, "L-R设置");
            lv_label_set_recolor(wlb1,true);//重新着色
            lv_obj_add_style(wlb1, &styletext2, LV_STATE_DEFAULT);
            lv_obj_align_to(wlb1,lb,LV_ALIGN_LEFT_MID,0,-50);

            wlb2=lv_label_create(oj);
            lv_obj_set_style_text_font(wlb2,&lv_font_msyh_23_num,0);
            lv_label_set_text(wlb2, "是否启用左右通道互换");
            lv_label_set_recolor(wlb2,true);//重新着色
            lv_obj_add_style(wlb2, &styletext2, LV_STATE_DEFAULT);
            lv_obj_align_to(wlb2,lb,LV_ALIGN_LEFT_MID,0,60);
            //开关1
            w7switch1=lv_switch_create(w7bt1);
            lv_obj_set_size(w7switch1,60, 30);
            lv_obj_set_style_bg_color(w7switch1, lv_color_hex(0x999999), LV_PART_MAIN);
            lv_obj_align_to(w7switch1,w7bt1, LV_ALIGN_LEFT_MID,650,0);
						
						if(WrBuff[11]==0)lv_obj_clear_state(w7switch1, LV_STATE_CHECKED);
						else lv_obj_add_state(w7switch1, LV_STATE_CHECKED);

            g7 = lv_group_create();//创建组
            //lv_group_set_default(g7);//如果部件已加入默认组，使用这种配置 设置默认组
            lv_group_add_obj(g7,wbt);
            lv_group_add_obj(g7,w7bt1);

            lv_indev_set_group(indev_keypad, g7);//关联按键和组


            //lv_group_focus_obj(wbt);//聚焦
						
//						flashflaginsel=1;
}

        void show_oj_win6(){//亮度与主题
            levelflag=3;
            idflag=6;
//						WrBuff[18]=0x32;
            /**********       亮度与主题                          */
            ojw6 = lv_obj_create(NULL);
            lv_scr_load(ojw6);
            lv_obj_add_style(ojw6, & stylebk, LV_STATE_DEFAULT);
            lv_obj_clear_flag(ojw6, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
            /* 黑面板 */
            oj = lv_obj_create(ojw6);
            lv_obj_set_size(oj,770, 218);//在主屏幕下创建一个等大的对象屏幕
            lv_obj_set_style_radius(oj,0,0);//设置容器圆角为直角
            lv_obj_align(oj, LV_ALIGN_CENTER, 0, 40);
            lv_obj_add_style(oj, & stylebk, LV_STATE_DEFAULT);
						lv_obj_add_style(oj, &scrollstyle, LV_PART_SCROLLBAR);
						lv_obj_set_scroll_dir(oj, LV_DIR_VER);//滚动方向
					
            /* 大标题标签 */
            lb=lv_label_create(ojw6);
            lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
            lv_label_set_text(lb, "亮度");
            lv_label_set_recolor(lb,true);//重新着色
            lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
            lv_obj_align_to(lb,ojw2,LV_ALIGN_CENTER,0,-105);
//            lv_obj_align_to(menuoj,lb,LV_ALIGN_CENTER,0,150);
            /* 返回按钮 */
            wbt = lv_obj_create(ojw6);//menuback
            lv_obj_set_size(wbt,150,50);
            lv_obj_align(wbt, LV_ALIGN_TOP_LEFT,  10, 125);
            lv_obj_add_style(wbt, &style, 0);//透明化
						lv_obj_add_style(wbt, &style2, LV_STATE_FOCUSED);
						lv_obj_set_style_radius(wbt,0,0);//设置容器圆角为直角
            lv_obj_add_event_cb(wbt, event_cbback, LV_EVENT_CLICKED, NULL);//添加事件
            lv_obj_add_event_cb(wbt, event_cbback, LV_EVENT_FOCUSED, NULL);//添加事件
            lv_obj_clear_flag(wbt, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
            /* 返回图标 */
            ojimg =lv_img_create(wbt);
            lv_img_set_src(ojimg, &back); /* 设置图片源 */
            lv_obj_align_to(ojimg,wbt,LV_ALIGN_CENTER,-50,0);
            /* 返回标签 */
            wlbback=lv_label_create(wbt);//按钮中的标签
            lv_obj_set_style_text_font(wlbback,&lv_font_msyh_23_num,0);
            lv_label_set_text(wlbback, "设置");
            lv_label_set_recolor(wlbback,true);//重新着色
            lv_obj_add_style(wlbback, &styletext2, LV_STATE_DEFAULT);
            lv_obj_align_to(wlbback,wbt, LV_ALIGN_CENTER,0,0);

            /* 亮度按键 */
            w2bt1 = lv_obj_create(oj);
            lv_obj_set_size(w2bt1,715, 60);
            lv_obj_set_scroll_dir(w2bt1, LV_DIR_VER);
            lv_obj_set_style_border_color(w2bt1, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
            lv_obj_set_style_bg_color(w2bt1, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
            lv_obj_add_style(w2bt1, &stylesli2, LV_STATE_FOCUSED);
            //lv_obj_add_style(w2bt1, &stylesli3, LV_STATE_PRESSED);
						lv_obj_add_style(w2bt1, &stylesli3, LV_STATE_EDITED);
            lv_obj_add_event_cb(w2bt1, w2_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
            lv_obj_add_event_cb(w2bt1, w2_event_cb, LV_EVENT_CLICKED, NULL);/* 添加事件 */
            lv_obj_align_to(w2bt1,oj,LV_ALIGN_CENTER,-5,-20);
            lv_obj_clear_flag(w2bt1, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
						lv_obj_set_style_radius(w2bt1,0,0);//设置容器圆角为直角

            wlb1=lv_label_create(oj);
            lv_obj_set_style_text_font(wlb1,&lv_font_msyh_23_num,0);
            lv_label_set_text(wlb1, "亮度");
            lv_label_set_recolor(wlb1,true);//重新着色
            lv_obj_add_style(wlb1, &styletext2, LV_STATE_DEFAULT);
            //lv_obj_align_to(wlb1,wbt,LV_ALIGN_CENTER,-5,75);

            /* 亮度最小 */
            blmaximg= lv_img_create(w2bt1);
            lv_img_set_src(blmaximg, &blmin);
            lv_obj_align_to(blmaximg,w2bt1, LV_ALIGN_LEFT_MID,0,0);
						lv_obj_align_to(wlb1,blmaximg,LV_ALIGN_LEFT_MID,5,-50);
						
            wlb2=lv_label_create(oj);
            lv_obj_set_style_text_font(wlb2,&lv_font_msyh_23_num,0);
            lv_label_set_text(wlb2, "屏幕亮度调节");
            lv_label_set_recolor(wlb2,true);//重新着色
            lv_obj_add_style(wlb2, &styletext2, LV_STATE_DEFAULT);
            lv_obj_align_to(wlb2,w2bt1,LV_ALIGN_BOTTOM_LEFT,5,60);

            /* 亮度最大 */
            blmaximg= lv_img_create(w2bt1);
            lv_img_set_src(blmaximg, &blmax);
            lv_obj_align_to(blmaximg,w2bt1, LV_ALIGN_LEFT_MID,625,0);

            /* 亮度条图片 */
            blimg= lv_img_create(w2bt1);
						backlight=*((volatile uint32_t *)(EEPROM_ADDR + 5*4));//背光数据包
						if(backlight<0)backlight=1;if(backlight>7)backlight=8;
						if(backlight==1)lv_img_set_src(blimg, &BL1); /* 设置亮度 */
						else if(backlight==2)lv_img_set_src(blimg, &BL2); /* 设置亮度 */
						else if(backlight==3)lv_img_set_src(blimg, &BL3); /* 设置亮度 */
						else if(backlight==4)lv_img_set_src(blimg, &BL4); /* 设置亮度 */
						else if(backlight==5)lv_img_set_src(blimg, &BL5); /* 设置亮度 */
						else if(backlight==6)lv_img_set_src(blimg, &BL6); /* 设置亮度 */
						else if(backlight==7)lv_img_set_src(blimg, &BL7); /* 设置亮度 */
						else if(backlight==8)lv_img_set_src(blimg, &BL8); /* 设置亮度 */
						//lv_slider_set_value(w1sli, backlight*12.5, LV_ANIM_OFF); /* 设置当前值 */
						
            lv_obj_align_to(blimg,w2bt1, LV_ALIGN_CENTER,0,0);

            wlb1=lv_label_create(oj);
            lv_obj_set_style_text_font(wlb1,&lv_font_msyh_23_num,0);
            lv_label_set_text(wlb1, "变暗开关");
            lv_label_set_recolor(wlb1,true);//重新着色
            lv_obj_add_style(wlb1, &styletext2, LV_STATE_DEFAULT);

            /********       菜单面板                           */
            menuoj = lv_obj_create(oj);
            lv_obj_set_size(menuoj,715,180);
            lv_obj_set_scroll_dir(menuoj, LV_DIR_VER);
            lv_obj_set_style_border_color(menuoj, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
            lv_obj_set_style_bg_color(menuoj, lv_color_hex(0x1A1C1D), LV_STATE_DEFAULT );
            lv_obj_align_to(menuoj,oj,LV_ALIGN_CENTER,0,230);
            lv_obj_clear_flag(menuoj, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
            int a,b=0;
            /* 线条 */
            for(a=1;a<=2;a++){
            line=lv_obj_create(menuoj);
            lv_obj_set_style_bg_color(line, lv_color_hex(0x2A2829), LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(line, lv_color_hex(0x2A2829), LV_STATE_DEFAULT);
            lv_obj_set_size(line,690,1);
            if(a==1)
            lv_obj_align_to(line, menuoj,LV_ALIGN_TOP_MID, 5, 40);
            else lv_obj_align_to(line, menuoj,LV_ALIGN_TOP_MID, 5, 40+b);
            b+=60;
            }

                /********       变暗开关                           */
                w2bt2 = lv_obj_create(menuoj);
                lv_obj_set_size(w2bt2,700,50);
                lv_obj_align_to(w2bt2, menuoj, LV_ALIGN_TOP_LEFT, -15, -15);
                lv_obj_add_style(w2bt2, &style, 0);
                lv_obj_add_style(w2bt2, &style2, LV_STATE_FOCUSED);
								lv_obj_set_style_radius(w2bt2,0,0);//设置容器圆角为直角
                lv_obj_clear_flag(w2bt2, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条

                lb=lv_label_create(w2bt2);//标题1标签
                lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
                lv_label_set_text(lb, "变暗开关");
                lv_label_set_recolor(lb,true);//重新着色
                lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
                lv_obj_add_event_cb(w2bt2, w2_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
                lv_obj_add_event_cb(w2bt2, w2_event_cb, LV_EVENT_CLICKED, NULL);/* 添加事件 */
                lv_obj_align_to(lb,w2bt2, LV_ALIGN_TOP_LEFT,-5,-10);
                lv_obj_align_to(wlb1,lb,LV_ALIGN_LEFT_MID,2,-50);
                //开关1
                w2switch=lv_switch_create(w2bt2);
                lv_obj_set_size(w2switch,60, 30);
                lv_obj_set_style_bg_color(w2switch, lv_color_hex(0x999999), LV_PART_MAIN);
                lv_obj_align_to(w2switch,w2bt2, LV_ALIGN_LEFT_MID,600,0);
								if(WrBuff[14]==1)
                lv_obj_add_state(w2switch, LV_STATE_CHECKED);
								else lv_obj_clear_state(w2switch, LV_STATE_CHECKED);

                /********       变暗时间                           */
                w2bt3 = lv_obj_create(menuoj);
                lv_obj_set_size(w2bt3,700,50);
                lv_obj_align_to(w2bt3, menuoj, LV_ALIGN_TOP_LEFT, -15, 45);
                lv_obj_add_style(w2bt3, &style, 0);
								lv_obj_add_style(w2bt3, &style2, LV_STATE_FOCUSED);
								lv_obj_add_style(w2bt3, &stylesli3, LV_STATE_EDITED);
                lv_obj_clear_flag(w2bt3, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
								lv_obj_set_style_radius(w2bt3,0,0);//设置容器圆角为直角

                lb=lv_label_create(w2bt3);//标题1标签
                lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
                lv_label_set_text(lb, "变暗时间");
                lv_label_set_recolor(lb,true);//重新着色
                lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
                lv_obj_add_event_cb(w2bt3, w2_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
                lv_obj_add_event_cb(w2bt3, w2_event_cb, LV_EVENT_CLICKED, NULL);/* 添加事件 */
                lv_obj_align_to(lb,w2bt3, LV_ALIGN_TOP_LEFT,-5,-10);
								
								/* 文本框1 */
                menulb =lv_obj_create(w2bt3);
                lv_obj_set_size(menulb,300,30);
                lv_obj_align_to(menulb,w2bt3, LV_ALIGN_TOP_LEFT,350,-10);
                lv_obj_add_style(menulb, &style, 0);//透明化
                lv_obj_clear_flag(menulb, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
                    w2lb1=lv_label_create(w2bt3);
                    lv_label_set_recolor(w2lb1,true);//重新着色
                    lv_obj_add_style(w2lb1, &styletext2, LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(w2lb1,&lv_font_msyh_23_num,0);
                    lv_label_set_text_fmt(w2lb1, "%dS",WrBuff[15]);
                    lv_obj_align_to(w2lb1,menulb, LV_ALIGN_RIGHT_MID,20,0);

                /********       变暗亮度                           */
                w2bt4 = lv_obj_create(menuoj);
                lv_obj_set_size(w2bt4,700,50);
                lv_obj_align_to(w2bt4, menuoj, LV_ALIGN_TOP_LEFT, -15, 105);
                lv_obj_add_style(w2bt4, &style, 0);
                lv_obj_add_style(w2bt4, &style2, LV_STATE_FOCUSED);
								lv_obj_add_style(w2bt4, &stylesli3, LV_STATE_EDITED);
                lv_obj_clear_flag(w2bt4, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
								lv_obj_set_style_radius(w2bt4,0,0);//设置容器圆角为直角

                lb=lv_label_create(w2bt4);//标题1标签
                lv_obj_set_style_text_font(lb,&lv_font_simhei_25_num,0);
                lv_label_set_text(lb, "变暗亮度");
                lv_label_set_recolor(lb,true);//重新着色
                lv_obj_add_style(lb, &styletext, LV_STATE_DEFAULT);
                lv_obj_add_event_cb(w2bt4, w2_event_cb, LV_EVENT_FOCUSED, NULL);//添加事件
                lv_obj_add_event_cb(w2bt4, w2_event_cb, LV_EVENT_CLICKED, NULL);/* 添加事件 */
                lv_obj_align_to(lb,w2bt4, LV_ALIGN_TOP_LEFT,-5,-10);

                /* 文本框2 */
                menulb1 =lv_obj_create(w2bt4);
                lv_obj_set_size(menulb1,300,30);
                lv_obj_align_to(menulb1,w2bt4, LV_ALIGN_TOP_LEFT,350,-10);
                lv_obj_add_style(menulb1, &style, 0);//透明化
                lv_obj_clear_flag(menulb1, LV_OBJ_FLAG_SCROLLABLE);//禁用滚动条
                    w2lb2=lv_label_create(w2bt4);
                    lv_label_set_recolor(w2lb2,true);//重新着色
                    lv_obj_add_style(w2lb2, &styletext2, LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(w2lb2,&lv_font_msyh_23_num,0);
                    lv_label_set_text_fmt(w2lb2, "%d",WrBuff[16]);
                    lv_obj_align_to(w2lb2,menulb1, LV_ALIGN_RIGHT_MID,20,0);

            /* 黑面板 */
            w2black = lv_obj_create(oj);
            lv_obj_set_size(w2black,710,80);
            lv_obj_set_style_radius(w2black,0,0);//设置容器圆角为直角
            lv_obj_align_to(w2black,oj,LV_ALIGN_CENTER,0,365);
            lv_obj_add_style(w2black, & stylebk, LV_STATE_DEFAULT);
						
//            /********       主题面板                           */
//            menuoj = lv_obj_create(oj);
//            lv_obj_set_size(menuoj,715,120);
//            lv_obj_set_scroll_dir(menuoj, LV_DIR_VER);
//						lv_obj_add_style(menuoj, & stylebk, LV_STATE_DEFAULT);
//            lv_obj_align_to(menuoj,oj,LV_ALIGN_CENTER,-5,470);

                /* 选中图标 */
//                ojenter2img =lv_img_create(w2bt6);
//                lv_img_set_src(ojenter2img, &ENTER3); /* 设置图片源 */
//                lv_obj_align_to(ojenter2img,w2bt6, LV_ALIGN_TOP_LEFT,625,-5);

                g2 = lv_group_create();//创建组
                //lv_group_set_default(g1);//如果部件已加入默认组，使用这种配置 设置默认组
                lv_group_add_obj(g2,wbt);
                lv_group_add_obj(g2,w2bt1);
                lv_group_add_obj(g2,w2bt2);
                lv_group_add_obj(g2,w2bt3);
                lv_group_add_obj(g2,w2bt4);
//                lv_group_add_obj(g2,w2bt5);
//                lv_group_add_obj(g2,w2bt6);
                lv_obj_scroll_to_y(oj,0,0);
								
                lv_indev_set_group(indev_keypad, g2);//关联按键和组

//				flashflaginsel=1;
//        lv_group_focus_obj(wbt);//聚焦


        }
				
/* 自定义封装 */
void create_main_group(){
    /**********       创建组                           */
    if(lv_group_get_obj_count(g)){//lv_group_del(g);
            lv_group_remove_all_objs(g);
        }
        g = lv_group_create();//创建组
    //    lv_group_set_default(g);//如果部件已加入默认组，使用这种配置 设置默认组
        lv_group_add_obj(g,oj1);
        lv_group_add_obj(g,oj2);
        lv_group_add_obj(g,oj3);
        lv_group_add_obj(g,oj4);
        lv_group_add_obj(g,oj5);
        lv_group_add_obj(g,oj6);
        lv_group_add_obj(g,oj7);
				lv_group_add_obj(g,oj8);
				lv_group_add_obj(g,oj9);
				lv_group_add_obj(g,wbt);
        lv_indev_set_group(indev_keypad, g);//关联按键和组
}

void start_recovery(){//重启运行,恢复数据
	
	if(levelflag!=1){
						soundB=*((volatile uint32_t *)(EEPROM_ADDR + 0*4));
						soundBLtemp=*((volatile uint32_t *)(EEPROM_ADDR + 6*4));
						soundBRtemp=*((volatile uint32_t *)(EEPROM_ADDR + 7*4));

						soundBL=soundB+soundBLtemp,soundBR=soundB+soundBRtemp;
						if(soundBL>255){soundBL=255;}//超出矫正soundBLtemp=255-soundB;
						if(soundBR>255){soundBR=255;}//超出矫正soundBRtemp=255-soundB;
						WrBuff[0]=soundB;//存 写入flash数组 的数据，进入主界面的数据包
						WrBuff[6]=soundBLtemp,WrBuff[7]=soundBRtemp;
						
						
						WrBuff[1]=*((volatile uint32_t *)(EEPROM_ADDR + 1*4));
						WrBuff[2]=*((volatile uint32_t *)(EEPROM_ADDR + 2*4));
						if(WrBuff[1]==-1)WrBuff[1]=0;if(WrBuff[2]==-1)WrBuff[2]=0;
						WrBuff[3]=*((volatile uint32_t *)(EEPROM_ADDR + 3*4));
						WrBuff[4]=*((volatile uint32_t *)(EEPROM_ADDR + 4*4));//滤波器选择数据包
						if(WrBuff[4]==-1)WrBuff[4]=6;
					}			
						

						WrBuff[5]=*((volatile uint32_t *)(EEPROM_ADDR + 5*4));//背光数据包
						backlight=WrBuff[5];
						if(backlight<0)backlight=7;if(backlight>7)backlight=8;
	
						/*写入音量*/
						write_volume(soundBL,soundBR);
						/*写入滤波器*/
//						read_9039m_regs(FIR_Shape_Set,&FIR_value);//先读出寄存器值	
//						FIR_value &=(~0x07);//把0-2Bit位清0，其他位不变
//						if(WrBuff[4]==7){	
//						FIR_value |=M_P_SLOW_LOWDP;
//						write_9039m_regs(FIR_Shape_Set, FIR_value);}
//						else if(WrBuff[4]==0){
//						FIR_value |=M_P;
//						write_9039m_regs(FIR_Shape_Set, FIR_value);}
//						else if(WrBuff[4]==5){
//						FIR_value |=M_P_FAST;
//						write_9039m_regs(FIR_Shape_Set, FIR_value);}
//						else if(WrBuff[4]==1){
//						FIR_value |=APODIZING;
//						write_9039m_regs(FIR_Shape_Set, FIR_value);}
//						else if(WrBuff[4]==2){
//						FIR_value |=L_P_FAST;
//						write_9039m_regs(FIR_Shape_Set, FIR_value);}
//						else if(WrBuff[4]==4){
//						FIR_value |=L_P_SLOW;
//						write_9039m_regs(FIR_Shape_Set, FIR_value);}
//						else if(WrBuff[4]==6){
//						FIR_value |=M_P_SLOW;
//						write_9039m_regs(FIR_Shape_Set, FIR_value);}

				write_9039m_regs(FIR_Shape_Set, 0x60+WrBuff[4]);
				
						if(WrBuff[1]==0&&WrBuff[2]==0){
							LIN0;LIN1;//00 usb(xmos) or DSD
							if(WrBuff[3]==1);
							else if(WrBuff[3]==2){
								write_9039m_regs(0x01, 0XB2);//DSD解码
								write_9039m_regs(INSEL_SET, 0x42);//DSD输入
								}
						}	
						else if(WrBuff[1]==1&&WrBuff[2]==0){
							HIN0;LIN1;//10 CS8422
							if(WrBuff[3]==8){
	            write_CS8422_regs(0x03,0xC4);}//8422选择同轴
							else if(WrBuff[3]==16){
							write_CS8422_regs(0x03, 0x84);}//8422选择光纤
						}
						else if((WrBuff[1]==0&&WrBuff[2]==1)){
						LIN0;HIN1;//01 hdmi
						}
						else{
						LIN0;LIN1;//00 usb(xmos)
						insel0=0,insel1=0;
						}

						swm_delay_ms(10);
						write_9039m_regs(0x56, 0x00);//CH12345678 正常 其他静音		
								
						if(WrBuff[5]==-1)WrBuff[5]=7;
						PWM_SetHDuty(PWM1, PWM_CH_B,WrBuff[5]*100*10);//设置PWM电平时间
						
						/* 9039MPRO设置数据包 */
						WrBuff[8]=*((volatile uint32_t *)(EEPROM_ADDR + 8*4));//ASYNC
						if(WrBuff[8]==1){x01=0xFF;write_9039m_regs(0x01, x01);}
						else {WrBuff[8]=0;x01=0xBF;write_9039m_regs(0x01, x01);}//启用
						
						WrBuff[9]=*((volatile uint32_t *)(EEPROM_ADDR + 9*4));//IIR滤波器 2xOSF 4xOSF	
						if(WrBuff[9]==-1)WrBuff[9]=0;
						write_9039m_regs(0x5A, WrBuff[9]);
						WrBuff[11]=*((volatile uint32_t *)(EEPROM_ADDR + 11*4));//左右输出互换
						if(WrBuff[11]==1){//开
						    	write_9039m_regs(0x40, 0x01);//TDM CH1配置,选择TDM中的L声道
									swm_delay_ms(5);
									write_9039m_regs(0x41, 0x00);//TDM CH2配置,选择TDM中的R声道
									swm_delay_ms(5);
									write_9039m_regs(0x42, 0x01);//TDM CH3配置,选择TDM中的L声道
									swm_delay_ms(5);
									write_9039m_regs(0x43, 0x00);//TDM CH4配置,选择TDM中的R声道
									swm_delay_ms(5);
									write_9039m_regs(0x44, 0x01);//TDM CH5配置,选择TDM中的L声道
									swm_delay_ms(5);
									write_9039m_regs(0x45, 0x00);//TDM CH6配置,选择TDM中的R声道
									swm_delay_ms(5);
									write_9039m_regs(0x46, 0x01);//TDM CH7配置,选择TDM中的L声道
									swm_delay_ms(5);
									write_9039m_regs(0x47, 0x00);//TDM CH8配置,选择TDM中的R声道
									swm_delay_ms(5);
						}else{//关
							WrBuff[11]=0;
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
						}
						WrBuff[13]=*((volatile uint32_t *)(EEPROM_ADDR + 13*4));//相位反转
						if(WrBuff[13]==1){write_9039m_regs(0x57, 0xFF);}//启用
						else {WrBuff[13]=0;write_9039m_regs(0x57, 0x00);}//关闭
						
						/* CS8422设置数据包 */
						WrBuff[10]=*((volatile uint32_t *)(EEPROM_ADDR + 10*4));//8422 ASYNC
						if(WrBuff[10]==-1){WrBuff[10]=0;}
						if(WrBuff[10]==1){x0A=0x52;write_CS8422_regs(0x0A, x0A);}
						else {WrBuff[10]=0;x0A=0x02;write_CS8422_regs(0x0A, x0A);}
						
						swm_delay_ms(500);
						/* 输出选择数据包 */
						WrBuff[12]=*((volatile uint32_t *)(EEPROM_ADDR + 12*4));//输出选择
						if(WrBuff[12]==-1)WrBuff[12]=0;
						if(WrBuff[12]==0){GPIO_SetBit(GPIOM, PIN5);GPIO_SetBit(GPION, PIN0);}
						else if(WrBuff[12]==1){GPIO_SetBit(GPIOM, PIN5);GPIO_ClrBit(GPION, PIN0);}
						else if(WrBuff[12]==2){GPIO_ClrBit(GPIOM, PIN5);GPIO_SetBit(GPION, PIN0);}
						else if(WrBuff[12]==3){GPIO_ClrBit(GPIOM, PIN5);GPIO_ClrBit(GPION, PIN0);}
						
						/* 变暗数据数据包 */
						WrBuff[14]=*((volatile uint32_t *)(EEPROM_ADDR + 14*4));//开关
						if(WrBuff[14]==-1)WrBuff[14]=1;
						
						WrBuff[15]=*((volatile uint32_t *)(EEPROM_ADDR + 15*4));//时间
						if(WrBuff[15]==-1)WrBuff[15]=12;
						WrBuff[16]=*((volatile uint32_t *)(EEPROM_ADDR + 16*4));//亮度
						if(WrBuff[16]==-1)WrBuff[16]=2;
						
}


void s9039_recovery(){//重启运行,恢复数据
						/*写入音量*/
						write_volume(soundBL,soundBR);
						/*写入滤波器*/
						read_9039m_regs(FIR_Shape_Set,&FIR_value);//先读出寄存器值	
						FIR_value &=(~0x07);//把0-2Bit位清0，其他位不变
						if(WrBuff[4]==7){	
						FIR_value |=M_P_SLOW_LOWDP;
						write_9039m_regs(FIR_Shape_Set, FIR_value);}
						else if(WrBuff[4]==0){
						FIR_value |=M_P;
						write_9039m_regs(FIR_Shape_Set, FIR_value);}
						else if(WrBuff[4]==5){
						FIR_value |=M_P_FAST;
						write_9039m_regs(FIR_Shape_Set, FIR_value);}
						else if(WrBuff[4]==1){
						FIR_value |=APODIZING;
						write_9039m_regs(FIR_Shape_Set, FIR_value);}
						else if(WrBuff[4]==2){
						FIR_value |=L_P_FAST;
						write_9039m_regs(FIR_Shape_Set, FIR_value);}
						else if(WrBuff[4]==4){
						FIR_value |=L_P_SLOW;
						write_9039m_regs(FIR_Shape_Set, FIR_value);}
						else if(WrBuff[4]==6){
						FIR_value |=M_P_SLOW;
						write_9039m_regs(FIR_Shape_Set, FIR_value);}
				

						swm_delay_ms(10);
						write_9039m_regs(0x56, 0x00);//CH12345678 正常 其他静音		
										
						/* 9039MPRO设置数据包 */
						WrBuff[8]=*((volatile uint32_t *)(EEPROM_ADDR + 8*4));//ASYNC
						if(WrBuff[8]==1){x01=0xFF;write_9039m_regs(0x01, x01);}
						else {WrBuff[8]=0;x01=0xBF;write_9039m_regs(0x01, x01);}//启用
						
						WrBuff[9]=*((volatile uint32_t *)(EEPROM_ADDR + 9*4));//IIR滤波器 2xOSF 4xOSF	
						if(WrBuff[9]==-1)WrBuff[9]=0;
						write_9039m_regs(0x5A, WrBuff[9]);
						WrBuff[11]=*((volatile uint32_t *)(EEPROM_ADDR + 11*4));//左右输出互换
						if(WrBuff[11]==1){//开
						    	write_9039m_regs(0x40, 0x01);//TDM CH1配置,选择TDM中的L声道
									swm_delay_ms(5);
									write_9039m_regs(0x41, 0x00);//TDM CH2配置,选择TDM中的R声道
									swm_delay_ms(5);
									write_9039m_regs(0x42, 0x01);//TDM CH3配置,选择TDM中的L声道
									swm_delay_ms(5);
									write_9039m_regs(0x43, 0x00);//TDM CH4配置,选择TDM中的R声道
									swm_delay_ms(5);
									write_9039m_regs(0x44, 0x01);//TDM CH5配置,选择TDM中的L声道
									swm_delay_ms(5);
									write_9039m_regs(0x45, 0x00);//TDM CH6配置,选择TDM中的R声道
									swm_delay_ms(5);
									write_9039m_regs(0x46, 0x01);//TDM CH7配置,选择TDM中的L声道
									swm_delay_ms(5);
									write_9039m_regs(0x47, 0x00);//TDM CH8配置,选择TDM中的R声道
									swm_delay_ms(5);
						}else{//关
							WrBuff[11]=0;
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
						}
						

}
void switch_num_ge(int soundBt){//显示个位
	int ge;
		ge=soundBt%10;
		switch(ge){
			case 0:
				lv_img_set_src(mainge, &j0);
			break;
			case 1:
				lv_img_set_src(mainge, &j1);
			break;
			case 2:
				lv_img_set_src(mainge, &j2);
			break;
			case 3:
				lv_img_set_src(mainge, &j3);
			break;
			case 4:
				lv_img_set_src(mainge, &j4);
			break;
			case 5:
				lv_img_set_src(mainge, &j5);
			break;
			case 6:
				lv_img_set_src(mainge, &j6);
			break;
			case 7:
				lv_img_set_src(mainge, &j7);
			break;
			case 8:
				lv_img_set_src(mainge, &j8);
			break;
			case 9:
				lv_img_set_src(mainge, &j9);	
			break;

			defalut:
			lv_img_set_src(mainge, &j0);	
				break;
		}

}

void switch_num_shi(int soundBt){//显示十位
	int shi;
		shi=soundBt/10;
		switch(shi){
			case 0:
				lv_img_set_src(mainshi, &j0);
			break;
			case 1:
				lv_img_set_src(mainshi, &j1);
			break;
			case 2:
				lv_img_set_src(mainshi, &j2);
			break;
			case 3:
				lv_img_set_src(mainshi, &j3);
			break;
			case 4:
				lv_img_set_src(mainshi, &j4);
			break;
			case 5:
				lv_img_set_src(mainshi, &j5);
			break;
			case 6:
				lv_img_set_src(mainshi, &j6);
			break;
			case 7:
				lv_img_set_src(mainshi, &j7);
			break;
			case 8:
				lv_img_set_src(mainshi, &j8);
			break;
			case 9:
				lv_img_set_src(mainshi, &j9);	
			break;

			defalut:
			lv_img_set_src(mainshi, &j0);	
				break;
		}

}