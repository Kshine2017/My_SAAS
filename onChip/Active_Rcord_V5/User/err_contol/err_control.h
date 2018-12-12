#ifndef __ERR_CONTROL_H
#define __ERR_CONTROL_H
#include "stdio.h"
#include "stm32f10x.h"
#include "machine_control.h"

/*ֱ�Ӳ���IO */
#define	digitalHi(p,i)			{(p)->BSRR=i;}			//�ø�	
#define digitalLo(p,i)			{(p)->BRR=i;}				//�õ�
#define digitalToggle(p,i)		{(p)->ODR ^=i;}			//ȡ��


#define RED_LED_TOGGLE		digitalToggle(GPIOB,GPIO_Pin_2)
#define RED_LED_OFF			  digitalHi(GPIOB,GPIO_Pin_2)
#define RED_LED_ON				digitalLo(GPIOB,GPIO_Pin_2)

#define GREEN_LED_TOGGLE  digitalToggle(GPIOB,GPIO_Pin_3)
#define GREEN_LED_OFF		  digitalHi(GPIOB,GPIO_Pin_3)
#define GREEN_LED_ON			digitalLo(GPIOB,GPIO_Pin_3)

#define GREEN_BLINK_TIME  (500 / 50) //�̵���˸ʱ����
#define RED_BLINK_TIME  (500 / 50) //�����˸ʱ����
extern  u32 RedLed_BlinkTime ;  //500ms   //����˸ʱ��
extern  u32 GreenLed_BlinkTime;  //500ms

extern void led_display(unsigned char state);

extern void err_control(module_state *state,void (*function)(unsigned char ));

extern void LED_GPIO_init(void);
#endif
