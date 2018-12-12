#ifndef __TIME_H
#define __TIME_H
#include "stm32f10x.h"
#include "ADXL345.h"
#include "usb.h"

enum Run_Time{                  //����ģ������ʱ��
	GetTempet_RunTime = 40,           //40*50ms
	GetVoice_RunTime = 40,           //40*50ms
	GetLight_RunTime = 40,           //40*50ms
	GetPower_RunTime = 40,
	ADXL345_RunTime = ADXL_SAMPLE_CYCLE / 50,
	POWER_CHECK_RunTime = 40,
  
};


#define TIME3_PSR 4799        //��ʱ��3Ԥ��Ƶϵ��
#define TIME3_BAST_TIME  499     //��ʱ��3�ж�ʱ�����
void TIM3_Int_Init(u16 arr,u16 psc);


#endif
