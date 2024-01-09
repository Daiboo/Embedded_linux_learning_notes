#include "bsp_key.h"
#include "bsp_delay.h"

void key_init(void)
{
    /* 初始化IO复用*/
    IOMUXC_SetPinMux(KEY0_IOMUXC, 0);
    /* 配置GPIO5_IO01的IO属性 */
    IOMUXC_SetPinConfig(KEY0_IOMUXC, KEY0_CONFIG);

    /* 配置GPIO5_IO01为输入 */
    KEY0_GPIO->GDIR &= ~(1 << KEY0_GPIO_PIN);

}

int key_read(void)
{
    return (keyvalue)((KEY0_GPIO->DR & (1 << KEY0_GPIO_PIN)) >> KEY0_GPIO_PIN);
}

int get_key_value(void)
{
    int ret = 0;
    static unsigned char release = 1;  // 1: 按键释放状态 0: 按键按下状态
    if((release == 1) && (key_read() == KEY0_VALUE))  
    {
        delay(10);
        release = 0;
        if(key_read() == KEY0_VALUE)
        {
            ret = KEY0_VALUE;
        }
    }
    else if((key_read() == KEY_NONE))  // 按键没有按下
    {
        ret = KEY_NONE;
        release = 1;
    }
    return ret;
}