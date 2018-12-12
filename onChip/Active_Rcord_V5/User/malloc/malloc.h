#ifndef __MALLOC_H
#define __MALLOC_H
#include "stm32f10x.h"
#include <stdio.h>
#define SRAMBANK 1  //支持内存块数

#define MEM_BLOCK_SIZE 32   //内存块大小byte
#define MEM_MAX_SIZE 	40*1024 //内存最大管理内存 40K
#define MEM_ALLOC_TABLE_SIZE (MEM_MAX_SIZE / MEM_BLOCK_SIZE)	//内存表大小 

struct _m_malloc_dev{
	void(*init)(void);  			//初始化
	u8 (*perused)(void);  		//内存使用率
	u8 *membase;  					//内存池管理整个SRAMBANK个区域内存
	u16 *memmap;   					//内存管理状态表
	u8 memready;             //内存管理是否就绪
};

extern struct _m_malloc_dev malloc_dev;   //内存管理结构实体
void my_mem_init(void);
u8 my_mem_perused(void);
void *mymalloc(u32 size);
void my_free(void *ptr);
void* my_memcpy(void *des,void *src,u32 n);
void* my_memset(void *s, u8 c, u32 count);
#endif
