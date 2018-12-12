#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

#define RECEIVE_OVER_TIME 100    //���ճ�ʱʱ��

typedef struct USART1_Struct{
	int send_over_flag:4; //���ڷ�����ɱ�־
	int usart_rx_ok:4; //���ڽ�����ɱ�־
	u8 receive_count;  //���ڽ�������
	u16 receive_over_time; //���ճ�ʱʱ��
	u8 pro_receiver_count;
}usart1_t;

extern usart1_t usart1;
extern u8 usart1_buf[50];
void USART1_Config(void);
void usart1_dealHandle( u8 *);

#endif /* __USART1_H */
