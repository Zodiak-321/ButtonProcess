#include <ButtonProcess.h>    // 引入头文件

bool button1_socrce;    // 按钮源
bool button2_socrce;

BUTTON button1(&button1_socrce, false, false, BUTTON::Active::HIGH_ACTIVE);    // 按钮注册,可以同时注册多个按钮
BUTTON button2(&button2_socrce, true, false, BUTTON::Active::HIGH_ACTIVE);

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

void Button_init(void)    // 按键任务注册
{
    button1.set_shortPress_event(Button1_shortPress_callback, nullptr);
    button1.set_longPress_event(Button1_longPress_callback, nullptr);
    button2.set_shortPress_event(Button2_shortPress_callback, nullptr);
    button2.set_doublePress_event(Button2_doublePress_callback, nullptr);
}

void main(void)
{
    Button_init();    // 执行按钮任务注册

    while(1)
    {
        button1.update_button();    // 监视按键状态
        button2.update_button();
    }
}







