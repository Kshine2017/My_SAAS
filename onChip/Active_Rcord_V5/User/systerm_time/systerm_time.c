#include "systerm_time.h"


static u32 sys_us;
static u32 sys_ms;
/*
 利用系统定时器进行延迟
*/
void delay_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	sys_us = SystemCoreClock/8000000;				//为系统时钟的1/8  
	sys_ms = sys_us *1000;
}

/* us延迟函数*/
void delay_us(u32 us)
{
	u32 temp;
	SysTick->LOAD = (u32)us * sys_us;
	SysTick->VAL = 0X00;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	do{
		temp=SysTick->CTRL;
		}while( (temp & 0x01) && !(temp &(1<<16)) );
		SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);	//关闭定时器
	  SysTick->VAL = 0;
}

/* ms延迟函数*/
void delay_ms(u32 ms)
{
	u32 temp;
	SysTick->LOAD = (u32)ms * sys_ms;
	SysTick->VAL = 0X00;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	do{
		temp=SysTick->CTRL;
		}while( (temp & 0x01) && !(temp &(1<<16)) );
		SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);	//关闭定时器
	  SysTick->VAL = 0;
}









/***************endif****************/


