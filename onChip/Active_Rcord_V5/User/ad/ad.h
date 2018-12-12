#ifndef __AD_H
#define __AD_H
#include "stm32f10x.h"

//#define ADC1_DR_Address (0x40012400+0x4c)   //ADC1外设转换值
#define M 	4   														//通道数量 5  4(去掉光强的测量值)
#define N 	10                              //每个通道缓存大小


enum LightAD_Rank{       										//光线等级
	Light_One = 1,                         
	Light_Two,
	Light_Three,
	Light_Four,
	Light_Five,
	Light_Six,
	Light_Seven,
	Light_Eight,
};

enum PowerAD_Rang{
	MAX = 2296,   //3.7V AD值
  MIN = 2047,   //3.3V AD值
};

extern u16 AD_Value[N][M]; //AD采集值保存单元
extern u16 Power_Value ;             //电源值
extern void AD_InitConfig(void);
extern void L_Get_Value(u16 (*data)[M],u8 *value);
extern void P_Get_Value(u16 (*data)[M],u8 *value);
extern void T_Get_Value(u16 (*data)[M],u8 *valuet);
extern void V_Get_Value(u16 (*data)[M],u8 *v_v);
#endif
