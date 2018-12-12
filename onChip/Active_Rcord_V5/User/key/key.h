#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"
#include "machine_control.h"
#include "err_control.h"

#define POWER_ON do{ GPIO_SetBits(GPIOB, GPIO_Pin_5);} while(0)    //开机
#define POWER_OFF do{GPIO_ResetBits(GPIOB, GPIO_Pin_5);} while(0)  //关机
#define GET_STDBY_VALUE   GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8)   	//充电完成
#define GET_CHRG_VALUE		GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_9)   	//正在充电
#define GET_KEY_VLAUE		GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4);       //电源按键
extern void KEY_init(void);
extern void key_control(module_state * state);
extern void PowerKEY_Control(module_state *Module_State);
extern void Power_ON(module_state *state);
extern void DealKey(module_state *Module_State);
#endif
