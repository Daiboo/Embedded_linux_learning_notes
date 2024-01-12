#ifndef __BSP_EXTI_H__
#define __BSP_EXTI_H__
#include "imx6ul.h"

void exti_init(void);
void key0_irqhandler(unsigned int gicciar, void *param);

#endif // __BSP_EXTI_H__