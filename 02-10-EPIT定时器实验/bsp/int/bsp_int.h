#ifndef __BSP_INT_H__
#define __BSP_INT_H__
#include "imx6ul.h"

/* 中断处理函数 */
typedef void (*system_irq_handler_t)(unsigned int gicciar, void *param);

/* 中断处理函数结构体 */
typedef struct _sys_irq_handle
{
    system_irq_handler_t irqHandler;    /* 中断处理函数 */
    void *userParam;                    /* 中断处理函数参数 */
}sys_irq_handle_t;

void int_init(void);
/* 默认中断处理函数 */
void default_irqHandler(unsigned int gicciar, void *param);
/* 注册中断处理函数 */
void system_register_irqhandler(IRQn_Type irq, system_irq_handler_t handler, void *userParam);

#endif // __BSP_INT_H__
