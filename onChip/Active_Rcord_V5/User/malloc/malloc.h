#ifndef __MALLOC_H
#define __MALLOC_H
#include "stm32f10x.h"
#include <stdio.h>
#define SRAMBANK 1  //֧���ڴ����

#define MEM_BLOCK_SIZE 32   //�ڴ���Сbyte
#define MEM_MAX_SIZE 	40*1024 //�ڴ��������ڴ� 40K
#define MEM_ALLOC_TABLE_SIZE (MEM_MAX_SIZE / MEM_BLOCK_SIZE)	//�ڴ���С 

struct _m_malloc_dev{
	void(*init)(void);  			//��ʼ��
	u8 (*perused)(void);  		//�ڴ�ʹ����
	u8 *membase;  					//�ڴ�ع�������SRAMBANK�������ڴ�
	u16 *memmap;   					//�ڴ����״̬��
	u8 memready;             //�ڴ�����Ƿ����
};

extern struct _m_malloc_dev malloc_dev;   //�ڴ����ṹʵ��
void my_mem_init(void);
u8 my_mem_perused(void);
void *mymalloc(u32 size);
void my_free(void *ptr);
void* my_memcpy(void *des,void *src,u32 n);
void* my_memset(void *s, u8 c, u32 count);
#endif
