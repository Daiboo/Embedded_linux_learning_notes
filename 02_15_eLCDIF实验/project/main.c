#include "bsp_clk.h"
#include "bsp_key.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_int.h"
// #include "bsp_exti.h"
// #include "bsp_epit.h"
#include "bsp_keyfilter.h"
#include "bsp_uart.h"	
#include "stdio.h"
#include "bsp_lcdapi.h"
#include "main.h"

/* 背景颜色索引 */
unsigned int backcolor[10] = {
	LCD_BLUE, 		LCD_GREEN, 		LCD_RED, 	LCD_CYAN, 	LCD_YELLOW, 
	LCD_LIGHTBLUE, 	LCD_DARKBLUE, 	LCD_WHITE, 	LCD_BLACK, 	LCD_ORANGE

}; 


int main(void)
{
	int_init(); 		/* 初始化中断 			*/
	imx6u_clkinit();    /* 初始化系统时钟 			*/
	delay_init();		/* 初始化延时 			*/
	clk_enable();		/* 使能时钟 			*/
	uart1_init();		/* 初始化串口1 			*/
	led_init();			/* 初始化led 			*/
	beep_init();		/* 初始化beep	 		*/
	lcd_init();			/* 初始化LCD 			*/


	unsigned char index = 0;
	unsigned char state = OFF;
	tftlcd_dev.forecolor = LCD_RED;	  
	while(1)			
	{	
#if 1
		lcd_clear(backcolor[index]);
		delayms(1); 	  
		// lcd_show_string(10, 40, 260, 32, 32,(char*)"FIRE IMX6U"); 	
		// lcd_show_string(10, 80, 240, 24, 24,(char*)"RGBLCD TEST");
		// lcd_show_string(10, 110, 240, 16, 16,(char*)"Daiboo");      					 
		// lcd_show_string(10, 130, 240, 12, 12,(char*)"2024-3-11");
			      					 
	    index++;
		if(index == 10)
			index = 0;      
		state = !state;
		led_switch(led_green,state);
		delayms(1000);
#endif
#if 10
		index++;
		if(index == 10)
			index = 0;
		lcd_fill(0, 300, 1023, 599, backcolor[index]);
		lcd_show_string(800,10,240,32,32,(char*)"INDEX=");  /*显示字符串				  */
		lcd_shownum(896,10, index, 2, 32); 					/* 显示数字，叠加显示	*/
		
		state = !state;
		led_switch(led_green,state);
		delayms(1000);	/* 延时一秒	*/
#endif
	}
	return 0;
}
