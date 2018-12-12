/******************** (C) COPYRIGHT 2009 www.armjishu.com ************************
* File Name          : date.h
* Author             : www.armjishu.com Team
* Version            : V1.0
* Date               : 12/1/2009
* Description        : 日期相关函数
*******************************************************************************/
#ifndef __DATE_H
#define __DATE_H

#include "stm32f10x.h"

struct rtc_time {
	volatile unsigned char tm_sec;
	volatile unsigned char tm_min;
	volatile unsigned char tm_hour;
	volatile unsigned char tm_mday;
	volatile unsigned char tm_mon;
	volatile unsigned int tm_year;
	volatile unsigned char tm_wday;
};
   

extern int month_days[12];
void GregorianDay(volatile struct rtc_time * tm);
uint32_t mktimev(volatile struct rtc_time *tm);
void to_tm(uint32_t tim, volatile struct rtc_time * tm);
u8 Is_Leap_Year(u16 year);
#endif 
