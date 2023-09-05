# LVGL常用函数指南：
    void lv_obj_del(lv_obj_t * obj);
    lv_obj_del 立即删除对象。

    lv_obj_del_async(obj) 异步删除，系统将在下一次调用lv_timer_handler()时删除对象

    例如，如果要在子对象LV_EVENT_DELETE处理程序中删除此对象的父对象时，可以使用 lv_obj_clean(obj) 删除对象的所有子项（但不是对象本身），一段时间后，您可以使用 lv_obj_del_delayed(obj, 1000) 来删除对象。 延迟以毫秒表示。



# static void clk1_event_cb( lv_event_t *e )

一个事件回调函数，调用lv_obj_t  *target = lv_event_get_target(e);来获取触发事件对象，当触发事件的部件是 ojX 对象时，会执行以下操作：

    1、调用 show_oj_winX 函数，可能是用于显示或处理与 ojX 相关的界面或功能。

    2、使用 lv_scr_load 函数加载 ojwX 对象作为新的屏幕，这可能会切换到与 ojX 相关的界面。

    3、使用 lv_obj_del_async 异步删除 scr 对象，这可能会清除当前屏幕上的内容，以便加载新的界面。
总之，这段代码的目的是在用户点击 ojX 对象时触发特定操作，可能是切换到与 ojX 相关的界面并清除当前屏幕内容。具体的操作和行为会根据应用程序的需求和界面设计而有所不同。
