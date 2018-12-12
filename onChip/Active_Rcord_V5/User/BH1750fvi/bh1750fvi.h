#ifndef __BH1750_H
#define __BH1750_H


#include "machine_control.h"
#include "file.h"
#include "Environment.h"
#define BH1750_ADDRESS 0x46     //器件地址

/*
器件地址说明：
根据BH1750FVI-中文手册（第10页）所述：
从属地址有2种形式，由ADDR端口决定。
ADDR = "H" (ADDR ≥ 0.7VCC) -> "1011 100" ->  0b10111000 -> 0xB8
ADDR = "L"（ADDR ≥ 0.3VCC）-> "0100 011" ->  0b01000110  -> 0x46
*/

//typedef struct BH_data  											//BH1750数据类型
//{   
//    short int data;
//    volatile unsigned char run_flag;          //BH1750数据采集标志
//} BH1750_Data_t;


////软件 取样  时间间隔
//#define   BH_SAMPLE_CYCLE  120   							//每120ms取一次数据
//#define   GET_BHDATA_CYCLE 1000  							//实际需要得到数据周期1000ms



/*
BH1750FIV工六种模式分为连续型和一次型，

连续型又分为高精度1、2和低精度模式，

单次型同样分为高精度1、2和低精度模式。

单次型与连续型的区别在于单次型读完一次数据就会自动转为PowerDown模式。
*/
#define BHAddWrite     0x46      //从机地址+最后写方向位    设置高精度连续读模式先发送IIC起始信号再发送7位器件地址+1位写标志位也就是0X46
#define BHAddRead      0x47      //从机地址+最后读方向位  	 设置高精度连续读模式先发送IIC起始信号再发送7位器件地址+1位读标志位也就是0X47
#define BHPowDown      0x00      //关闭模块  
#define BHPowOn        0x01      //打开模块等待测量指令  
#define BHReset        0x07      //重置数据寄存器值在PowerOn模式下有效  
#define BHModeH1       0x10      //高分辨率 单位1lx 测量时间120ms  
#define BHModeH2       0x11      //高分辨率模式2 单位0.5lx 测量时间120ms  
#define BHModeL        0x13      //低分辨率 单位4lx 测量时间16ms  
#define BHSigModeH     0x20      //一次高分辨率 测量 测量后模块转到 PowerDown模式  
#define BHSigModeH2    0x21      //同上类似  
#define BHSigModeL     0x23      // 上类似


//extern BH1750_Data_t BH1750_struct_Data;
void   		 BH1750_init();
float   get_data_BH1750();
uint8_t Check_BH1750();






#endif

