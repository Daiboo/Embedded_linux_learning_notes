#include "bsp_clk.h"

/* 使能外设时钟 */
void clk_enable(void)
{
    CCM->CCGR0 = 0xFFFFFFFF;  // 使能所有GPIO5时钟
    CCM->CCGR1 = 0xFFFFFFFF;  // 使能所有GPIO1时钟
    CCM->CCGR2 = 0xFFFFFFFF;  // 使能所有GPIO2时钟
    CCM->CCGR3 = 0xFFFFFFFF;  // 使能其中的GPIO4时钟
    CCM->CCGR4 = 0xFFFFFFFF;  // 使能所有GPIO3时钟
    CCM->CCGR5 = 0xFFFFFFFF;  // 使能所有GPIO7时钟
    

}