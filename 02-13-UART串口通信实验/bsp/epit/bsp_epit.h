#ifndef __BSP_EPIT_H__
#define __BSP_EPIT_H__
#include "imx6ul.h"
void epit_init(uint32_t frac, uint32_t value);
void epit1_irqhandler(void);
#endif // __BSP_EPIT_H__