#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "main.h"


int main(void)
{   
    /* 使能时钟 */
    clk_enable();

    /* 初始化led */
    led_init();

    /* 设置led闪烁 */
    while(1)
    {
        led_red_on();
        delay(500);
        led_red_off();

        led_green_on();
        delay(500);
        led_green_off();


        led_blue_on();
        delay(500);
        led_blue_off();
        
    }

    return 0;
}