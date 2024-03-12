#include "bsp_clk.h"

/* 使能外设时钟 */
void clk_enable(void)
{

	CCM->CCGR0 = 0XFFFFFFFF;
	CCM->CCGR1 = 0XFFFFFFFF;
	CCM->CCGR2 = 0XFFFFFFFF;
	CCM->CCGR3 = 0XFFFFFFFF;
	CCM->CCGR4 = 0XFFFFFFFF;
	CCM->CCGR5 = 0XFFFFFFFF;
	CCM->CCGR6 = 0XFFFFFFFF;

}


/* 初始化时钟 */
void imx6u_clkinit(void)
{
    uint32_t reg = 0;
    /* 初始化6u的主频为528MHZ */
    if((CCM->CCSR >> 2 & 0x1) == 0)  // 当前时钟使用pll1_main_clk,也就是pll1
    {
        CCM->CCSR &= ~(1 << 8);   // 设置step_clk时钟源osc_clk=24MHZ
        CCM->CCSR |= (1 << 2);    // 设置pll1_sw_clk时钟源step_clk=24MHZ
    }
    /* 设置PLL1为1056MHZ */
    // CCM_ANALOG->PLL_ARM |= (1 << 13);        // 使能时钟输出
    // CCM_ANALOG->PLL_ARM |= (88 << 0) & 0x7F; // 设置88,pll1=1056MHZ
    CCM_ANALOG->PLL_ARM = (1 << 13) | ((88 << 0) & 0x7F);   // !!这里需要用=,才能让程序运行起来
    CCM->CACRR = 1;                          // 设置2分频,pll1_main_clk=1056/2=528MHZ
    CCM->CCSR &= ~(1 << 2);                  // 设置pll1_sw_clk时钟源为pll1_main_clk=528MHZ

    /* 设置PLL2的4路PFD*/
    reg = CCM_ANALOG->PFD_528;
    reg &= ~(0X3F3F3F3F);   // 清除原来的设置
    reg |= 32 << 24;        // PLL2_PFD3=528*18/32=297MHZ
    reg |= 24 << 16;        // PLL2_PFD2=528*18/24=396MHZ
    reg |= 16 << 8;         // PLL2_PFD1=528*18/16=594MHZ
    reg |= 27 << 0;         // PLL2_PFD0=528*18/27=352MHZ
    CCM_ANALOG->PFD_528 = reg;  // 设置PLL2_PFD0~3

    /* 设置PLL3的4路PFD*/
    reg = CCM_ANALOG->PFD_480;
    reg &= ~(0X3F3F3F3F);   // 清除原来的设置
    reg |= 19 << 24;        // PLL3_PFD3=480*18/19=454.7MHZ
    reg |= 17 << 16;        // PLL3_PFD2=480*18/17=508.2MHZ
    reg |= 16 << 8;         // PLL3_PFD1=480*18/16=540MHZ
    reg |= 12 << 0;         // PLL3_PFD0=480*18/12=720MHZ
    CCM_ANALOG->PFD_480 = reg;  // 设置PLL3_PFD0~3

    /* 设置AHB_CLK_ROOT */
    CCM->CBCMR &= ~(3 << 18);   // 清除设置
    CCM->CBCMR |= (1 << 18);   // 设置pre_periph clock = PLL2_PFD2=396MHZ
    CCM->CBCDR &= ~(1 << 25);   // 清除设置,设置pre_periph_clk_sel=pre_periph_clk=396MHZ
    while (CCM->CDHIPR & (1 << 5)); // 等待握手完成

    // CCM->CBCDR &= ~(7 << 10);   // !!清除设置     这里不能直接清零,清零就变成1分频了
    CCM->CBCDR = (CCM->CBCDR & ~(7 << 10)) | (2 << 10);   // 396/3=132MHZ  这样就可以了
    while (CCM->CDHIPR & (1 << 1)); // 等待握手完成


    /* IPG_CLK_ROOT=66MHZ */
    CCM->CBCDR &= ~(3 << 8);   // 清除设置
    CCM->CBCDR |= (1 << 8);   // 设置ipg_podf=2分频,ipg_clk_root=132/2=66MHZ


    /* PERCLK_CLK_ROOT */
    CCM->CSCMR1 &= ~(1 << 6);   // 清除设置,设置为来源于ipg_clk_root=66MHZ
    CCM->CSCMR1 &= ~(0x3f << 0);   // 1分频,perclk_clk_root=66MHZ

    /* 设置UART的时钟 */
    CCM->CCSR &= ~(1 << 0);   // ppl3_main_clk
    CCM->CSCDR1 &= ~(1 << 6);   // uart_clk_root时钟源选择pll3_80m


}





