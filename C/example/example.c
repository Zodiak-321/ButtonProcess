#include <ButtonProcess.h>    // 引入头文件

uint16_t button1_socrce;    // 按钮源
uint16_t button2_socrce;

void Button1_shortPress_callback(void* context)    // 自定义按键1短按回调函数
{
    // 短按任务
}

void Button1_longPress_callback(void* context)    // 自定义按键1长按回调函数
{
    // 长按任务
}

void Button2_shortPress_callback(void* context)    // 自定义按键2短按回调函数
{
    // 短按任务
}

void Button2_doublePress_callback(void* context)    // 自定义按键2双击回调函数
{
    // 双击任务
}

void Button_init(void)    // 按键任务注册以及函数绑定
{
    button1 = create_button(button1_socrce, false, true, High_Active, 40, 30, 650, 350, 30);
    button2 = create_button(button2_socrce, false, true, High_Active, 40, 30, 650, 350, 30);

    set_shortPress_event(button1, button1_shortPress_callback, NULL);
    set_longPress_event(button1, button1_longPress_callback, NULL);
    set_shortPress_event(button2, button2_shortPress_callback, NULL);
    set_doublePress_event(button2, button2_doublePress_callback, NULL);
}

void main(void)
{
    Button_init();    // 执行按钮任务注册

    while(1)
    {
        updata_button(button1);    // 监视按键状态
        updata_button(button2);
    }
}
