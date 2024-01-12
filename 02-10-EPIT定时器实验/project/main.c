#include "bsp_clk.h"
#include "bsp_key.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_int.h"
#include "bsp_exti.h"
#include "bsp_epit.h"
#include "main.h"


int main(void)
{

	int_init(); 		/* 初始化中断 			*/
	imx6u_clkinit();    /* 初始化系统时钟 			*/

	clk_enable();		/* 使能所有的时钟 			*/
	led_init();			/* 初始化led 			*/
	beep_init();		/* 初始化beep	 		*/
	// key_init();			/* 初始化key 			*/
	epit_init(0, 66000000/2);	/* 初始化EPIT1定时器 		*/

	while(1)			
	{	

		delay(500);
	}

	return 0;
}
