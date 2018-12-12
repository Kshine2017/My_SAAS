#include "systerm_time.h"


static u32 sys_us;
static u32 sys_ms;
/*
 ����ϵͳ��ʱ�������ӳ�
*/
void delay_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
	sys_us = SystemCoreClock/8000000;				//Ϊϵͳʱ�ӵ�1/8  
	sys_ms = sys_us *1000;
}

/* us�ӳٺ���*/
void delay_us(u32 us)
{
	u32 temp;
	SysTick->LOAD = (u32)us * sys_us;
	SysTick->VAL = 0X00;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	do{
		temp=SysTick->CTRL;
		}while( (temp & 0x01) && !(temp &(1<<16)) );
		SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);	//�رն�ʱ��
	  SysTick->VAL = 0;
}

/* ms�ӳٺ���*/
void delay_ms(u32 ms)
{
	u32 temp;
	SysTick->LOAD = (u32)ms * sys_ms;
	SysTick->VAL = 0X00;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	do{
		temp=SysTick->CTRL;
		}while( (temp & 0x01) && !(temp &(1<<16)) );
		SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);	//�رն�ʱ��
	  SysTick->VAL = 0;
}









/***************endif****************/


