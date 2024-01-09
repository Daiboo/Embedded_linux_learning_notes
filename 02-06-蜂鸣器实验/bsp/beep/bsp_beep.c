#include "bsp_beep.h"


void beep_init(void)
{
    /* 初始化IO复用 */
    IOMUXC_SetPinMux(BEEP_CTL_PIN_MUX_IO, 0);  // 复用为GPIO1_IO19
    IOMUXC_SetPinConfig(BEEP_CTL_PIN_MUX_IO, 0x10B0);  // 设置电器属性

    /* 配置GPIO1_IO19为输出功能 */
    BEEP_GPIO->GDIR |= (1 << BEEP_GPIO_PIN);  // 设置为输出

    /* 设置GPIO1_IO19输出高电平，关闭蜂鸣器 */
    BEEP_GPIO->DR |= (1 << BEEP_GPIO_PIN);

}

void beep_on(void)
{
    BEEP_GPIO->DR &= ~(1 << BEEP_GPIO_PIN);  // 输出低电平，打开蜂鸣器
}

void beep_off(void)
{
    BEEP_GPIO->DR |= (1 << BEEP_GPIO_PIN);  // 输出高电平，关闭蜂鸣器
}


void beep_switch(int status)
{
    if (status == ON) {
        BEEP_GPIO->DR &= ~(1 << BEEP_GPIO_PIN);  // 输出低电平，打开蜂鸣器
    } else if (status == OFF) {
        BEEP_GPIO->DR |= (1 << BEEP_GPIO_PIN);  // 输出高电平，关闭蜂鸣器
    }
}