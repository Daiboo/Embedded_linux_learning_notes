#include "bsp_int.h"

/* 中断处理函数表 */
static sys_irq_handle_t irqTable[NUMBER_OF_INT_VECTORS];

/* 记录中断嵌套 */
static unsigned int irqNesting;

/* 初始化中断处理函数表*/
void system_irqTable_init(void)
{
    uint32_t i = 0;
    irqNesting = 0;
    for(i = 0; i < NUMBER_OF_INT_VECTORS; i++)
    {
        irqTable[i].irqHandler = default_irqHandler;
        irqTable[i].userParam = NULL;
    }

}

/* 注册中断处理函数 */
void system_register_irqhandler(IRQn_Type irq, system_irq_handler_t handler, void *userParam)
{
    irqTable[irq].irqHandler = handler;
    irqTable[irq].userParam = userParam;
}

/* 中断初始化函数 */
void int_init(void)
{
    GIC_Init();     /* 初始化GIC */
    system_irqTable_init();     /* 初始化中断处理函数表 */
    __set_VBAR(0x87800000);     /* 设置中断向量表偏移地址 */

}

/* 具体的中断处理函数,IRQ_Handler会调用此函数 */
void system_irqhandler(unsigned int gicciar)
{
    uint32_t intNum = gicciar & 0x3ff;  /* 取出中断ID */
    /* 检查中断号 */
    if(intNum >= NUMBER_OF_INT_VECTORS)
    {
        return;
    }
    irqNesting++;
    /* 根据中断id号,读取中断处理函数,然后执行 */
    irqTable[intNum].irqHandler(intNum, irqTable[intNum].userParam);
    irqNesting--;
}



/* 默认中断处理函数 */
void default_irqHandler(unsigned int gicciar, void *param)
{
    while(true);
}