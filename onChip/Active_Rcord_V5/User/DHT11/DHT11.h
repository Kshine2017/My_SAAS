#ifndef __DHT11_H
#define __DHT11_H
#include "bit_band.h"
//#define DHT11_OUT() do{\
//											*(volatile unsigned long *)(GPIOC_BASE) &= (~(0xf<<28));\
//											*(volatile unsigned long *)(GPIOC_BASE) |= (0x07<<28);\
//											}while(0) 
//#define DHT11_IN() do{ \
//											*(volatile unsigned long *)(GPIOC_BASE) &= (~(0xf<<28));\
//											*(volatile unsigned long *)(GPIOC_BASE) |= (0x8<<28);\
//											}while(0)

////IO方向设置
#define DHT11_IN()  do{GPIOC->CRL&=0X0FFFFFFF;GPIOC->CRL|=8<<28;}while(0)
#define DHT11_OUT() do{GPIOC->CRL&=0X0FFFFFFF;GPIOC->CRL|=3<<28;}while(0)

#define DHT11_DATA_OUT(DATA) { \
																if(DATA) PCout(7) = 1;\
																else  PCout(7) = 0;\
															}
#define DHT11_DATA_IN         PCin(7)

//#define DHT11_DATA_OUT(DATA) do{\
//											        if(DATA) *(volatile unsigned long *)(GPIOC_BASE + 0x0c) |= (1<<7);\
//											        else *(volatile unsigned long *)(GPIOC_BASE + 0x0c) &= (~(1<<7));\
//                             }while(0)

//#define DHT11_DATA_IN   *(volatile unsigned long *)(GPIOC_BASE+0X08)
															
typedef struct DHT11_Data{
		u8 Tempeture;         //温度
		u8 Humidity;          //湿度
	  volatile u8 run_flag;          //是否采集标志
}t_Evironment_Data;		

															
															
unsigned char DHT11_Init(void);
unsigned char DHT11_Read_Data(t_Evironment_Data*data);
#endif






