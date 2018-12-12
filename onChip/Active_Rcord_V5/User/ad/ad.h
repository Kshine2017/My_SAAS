#ifndef __AD_H
#define __AD_H
#include "stm32f10x.h"

//#define ADC1_DR_Address (0x40012400+0x4c)   //ADC1����ת��ֵ
#define M 	4   														//ͨ������ 5  4(ȥ����ǿ�Ĳ���ֵ)
#define N 	10                              //ÿ��ͨ�������С


enum LightAD_Rank{       										//���ߵȼ�
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
	MAX = 2296,   //3.7V ADֵ
  MIN = 2047,   //3.3V ADֵ
};

extern u16 AD_Value[N][M]; //AD�ɼ�ֵ���浥Ԫ
extern u16 Power_Value ;             //��Դֵ
extern void AD_InitConfig(void);
extern void L_Get_Value(u16 (*data)[M],u8 *value);
extern void P_Get_Value(u16 (*data)[M],u8 *value);
extern void T_Get_Value(u16 (*data)[M],u8 *valuet);
extern void V_Get_Value(u16 (*data)[M],u8 *v_v);
#endif
