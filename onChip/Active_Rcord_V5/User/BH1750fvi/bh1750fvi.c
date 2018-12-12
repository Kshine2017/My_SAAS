#include "bh1750fvi.h"
#include "i2c_2.h"
#include "AD.h"
//#include "file.h"
#include "rtc.h"
#include "math.h"






void   BH1750_init(void)
{
	
	//在此之前需要先初始化IIC
	//IIC2中的写函数：
	//Device_WriteBytes_2(uint8_t data,u8 Address, uint16_t _usAddress);
	//Address :芯片地址
	
	  Device_WriteBytes_2(BHPowOn ,BH1750_ADDRESS);     //发送启动命令  
    Device_WriteBytes_2(BHReset ,BH1750_ADDRESS);     //清除寄存器内容  
    Device_WriteBytes_2(BHSigModeH,BH1750_ADDRESS);      
   //delay_ms(180);           										 //设置完成后要有一段延迟，手册上说最大180ms就可以了 

//		bh_data_send(BHPowOn);    //发送启动命令  
//    bh_data_send(BHReset);    //清除寄存器内容  
//    bh_data_send(BHSigModeH);   //设置高精度为模式2  
//		
}




 float  get_data_BH1750()
{
	
	uint16_t temp;
	float datalx=0;
	Device_WriteBytes_2(BHPowOn ,BHAddWrite);  //针对一次性模式	
	Device_ReadBytes_2(&temp,BH1750_ADDRESS);
  Device_WriteBytes_2(BHPowDown ,BHAddWrite); 
//	Device_WriteBytes_2(BHReset ,BH1750_ADDRESS);
	
  	//bh_data_send(BHPowOn);    //发送启动命令
	 //datalx = bh_data_read()/1.2;
		datalx = temp/1.2;
	 return datalx;
	
}

uint8_t Check_BH1750()
{
	uint8_t ten;	
	Device_WriteBytes_2(BHPowOn ,BHAddWrite);
	ten = i2c_CheckDevice_2(BH1750_ADDRESS);
	Device_WriteBytes_2(BHPowDown ,BHAddWrite); 
	return ten;
	
	
}

