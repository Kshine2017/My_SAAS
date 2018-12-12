/***************��س��������*********************/
#ifndef __USB_H
#define __USB_H
#include "stm32f10x.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "machine_control.h"
#define BAT1_Charg_State  (GPIOB->IDR & GPIO_Pin_9)    //BAT1 ����
#define BAT1_Charg_Finish (GPIOB->IDR & GPIO_Pin_8)  //BAT1 �������



typedef enum Battery_State{                //��ص�״̬
	Free_State = 0,
	Charging_State ,
	Charg_Full_State,
}Bat_State;

typedef enum USB_State{                  //USBͨѶ״̬
	Disconnect = 0,														//�Ͽ�
	Connect       												//����ͨѶ
}USB_State;

typedef enum{    //USB���״̬
	No_chongdian = 0,  //û���
	chong_state = 1,   //���ڳ�
	chong_finsh = 2    //����
}chongdian_value;

extern USB_State Usb_State;
extern u8 chongdian_flag;
extern void Power_Manage(module_state *state);
extern void USB_Control_File(void);
extern void USB_Bat_state(void);
extern void USB_Control_GPIO_Init(void);
#endif


