#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_beep.h"
#include "main.h"


int main(void)
{   
    /* 使能时钟 */
    clk_enable();

    /* 初始化beep */
    beep_init();

    while (1) {
        beep_switch(ON);
        delay(500);
        beep_switch(OFF);
        delay(500);
    }

    return 0;
}