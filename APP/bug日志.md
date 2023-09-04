调整菜单UI顺序显示目前存在BUG：
    1、旋钮双击返回卡死✔(已解决)

    2、输出选择进入二级菜单后卡死✔(已解决)
      //输出选择 涉及对象与参数
      父级对象：oj3 
      本身对象：target = ojw3 
      显示函数：show_oj_win3 
      识别标志：idflag = 3 
      耳放输出对象：w8bt1、w8switch1
      RCA-XLR输出对象：w8bt2、w8switch2     
      事件回调：w8_event_cb，回调没问题，都能正确选择。
      BUG描述：首次加载输出选择会卡死，但是加载滤波器选择之后 又能正常加载选择菜单。
      g8
      确保没有死循环，长时间的计算或等待条件。
    3、左右输出互换进入二级菜单页面后卡死✔(已解决)
      //左右输出互换 涉及对象与参数
      父级对象：oj8 
      本身对象：target = ojw8 
      显示函数：show_oj_win8 
      识别标志：idflag = 8
      BUG描述：首次加载输出选择会卡死，但是加载滤波器选择之后 又能正常加载选择菜单。

    4、滤波选择菜单 跳出不正常。
      //滤波器选择 涉及对象与参数
      父级对象：oj2 
      本身对象：target = ojw2 
      显示函数：show_oj_win2 
      识别标志：idflag = 2 
      滤波器选择对象：w5bt1
      滤波器文本标题：w5lb
      事件回调：w5_event_cb ，
      组对象：g5
      滑条选择编辑：sliflag = 4

        lv_group_add_obj(g5,wbt);
        lv_group_add_obj(g5,w5bt1);
        lv_indev_set_group(indev_keypad, g5);//关联按键和组
      
      lv_obj_add_event_cb(w5bt1, w5_event_cb, LV_EVENT_FOCUSED, NULL)
      lv_obj_add_event_cb(w5bt1, w5_event_cb, LV_EVENT_CLICKED, NULL)

      改动顺序从idflag = 5 --> idflag = 2，WrBuff[18] = 0x32


      BUG描述：进入滤波器选择 二级菜单后，能够聚焦且选择滤波选项，
            但是在按下按钮（旋钮、遥控），点击事件之后会黑屏，之后显示9039MPRO设置菜单页面。
      关键点：点击事件， 黑屏， 显示9039设置页面



    5、输出选择全关后到底锁死 不能往下滚动✔(已解决)
        查找idflag = 3 相关旋钮锁操作
        双击返回 操作添加 解除旋钮向下锁标志
        prelock 为 0，表示到达列表的顶部，解锁上滚操作。
        nextlock 为 1，表示锁死下滚操作

    6、"亮度与主题"设置标题改为"休眠与屏幕"
      BUG描述：里面的"变暗亮度"顺时针到底没有锁死，在7之后会回弹为6。

    7、功能添加在"休眠与屏幕"里面，在"变暗亮度"往下滚动显示调节面板，面板包括一个开关和一个时间选项，从5分钟到30分钟，1分钟步进长度。    
      

旋钮按键部分：
宏定义：
  ENCODER_L        GPIO_GetBit(GPIOM, PIN1)   //KB
  ENCODER_R        GPIO_GetBit(GPIOM, PIN0)   //KA
  ENCODER_SW       GPIO_GetBit(GPIOD, PIN11)   //SW

ENCODER_READ(uint8_t levelflag)//旋钮状态读取
  uint8_t a=99,b=99;//存放按键的值
  - levelflag 判断界面：1为主界面 2为设置菜单 3为二级设置菜单
  - KUP 旋钮编码器是否锁死状态标志位 1为锁死 0为解除
    b = check_wkup();//获取旋钮按键键值
    --- 0 没按
    --- 1 单击
    --- 2 双击
    --- 4 长按
  return a //旋钮状态值
  - 1 逆时针转
  - 2 顺时针转
  - 3 单击 确认
  - 4 双击 返回
  - 5 长按
  ENCODER_LOOP(void) //扫描并获取旋钮编码器状态并执行相对应的操作
    getcode=ENCODER_READ(0);//读出旋转编码器按键值，初始化为0，旋钮无状态
  
lv_port_indev.c
  keypad_read()//图形库键盘读取回调函数
  act_key//状态键值
  --1 旋钮逆转/红外遥控←
  --2 旋钮顺转/红外遥控→
  --3 单击确认Enter
  --4 双击返回Back
  --5 长按休眠

LVGL示例函数：
switch(act_key) {
        case 1:
					data->state = LV_INDEV_STATE_PR;
                    act_key = LV_KEY_PREV;
				
            break;
        case 2:
					data->state = LV_INDEV_STATE_PR;     
					act_key = LV_KEY_NEXT;
				
            break;
        case 3:
					//data->state = LV_INDEV_STATE_PR;
				    data->state = LV_INDEV_STATE_PR;
            act_key = LV_KEY_ENTER;
			      break;
        case 4://双击
					if ( levelflag==2)  
				 {
					idflag=0;
          create_main_win();
          lv_obj_del_async(scr);
					//lv_scr_load(mainoj);
					//lv_obj_del(scr);

				 }
				  else if ( levelflag==3) 
				 {
          create_menu_win();
          if(idflag==1){lv_obj_del_async(ojw1);}
          if(idflag==2){lv_obj_del_async(ojw2);}
          if(idflag==3){lv_obj_del_async(ojw3);}
          if(idflag==4){lv_obj_del_async(ojw4);}
          if(idflag==5){lv_obj_del_async(ojw5);}
          if(idflag==6){lv_obj_del_async(ojw6);}
          if(idflag==7){lv_obj_del_async(ojw7);}
          if(idflag==8){lv_obj_del_async(ojw8);}
					//lv_scr_load(scr);
					//lv_obj_del(mainoj);
				 }
            break;
				case 5://长按
            break;

        default:
            break;
}


//旋钮编码器确认回调
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
				WrBuff[18]=0x35;
				flashflaglvbo=1;
				swm_delay_ms(300);
				GPIO_ClrBit(GPIOB, PIN4);//复位单片机
			flag=0;
			sliflag=0;
			}
}


  

















































