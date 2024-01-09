#include "bsp_clk.h"

/* 使能外设时钟 */
void clk_enable(void)
{
    CCM->CCGR1 = 0xFFFFFFFF;  // 使能所有GPIO1时钟
 
    
}