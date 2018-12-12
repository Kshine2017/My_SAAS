#include "bh1750fvi.h"
#include "i2c_2.h"
#include "AD.h"
//#include "file.h"
#include "rtc.h"
#include "math.h"






void   BH1750_init(void)
{
	
	//�ڴ�֮ǰ��Ҫ�ȳ�ʼ��IIC
	//IIC2�е�д������
	//Device_WriteBytes_2(uint8_t data,u8 Address, uint16_t _usAddress);
	//Address :оƬ��ַ
	
	  Device_WriteBytes_2(BHPowOn ,BH1750_ADDRESS);     //������������  
    Device_WriteBytes_2(BHReset ,BH1750_ADDRESS);     //����Ĵ�������  
    Device_WriteBytes_2(BHSigModeH,BH1750_ADDRESS);      
   //delay_ms(180);           										 //������ɺ�Ҫ��һ���ӳ٣��ֲ���˵���180ms�Ϳ����� 

//		bh_data_send(BHPowOn);    //������������  
//    bh_data_send(BHReset);    //����Ĵ�������  
//    bh_data_send(BHSigModeH);   //���ø߾���Ϊģʽ2  
//		
}




 float  get_data_BH1750()
{
	
	uint16_t temp;
	float datalx=0;
	Device_WriteBytes_2(BHPowOn ,BHAddWrite);  //���һ����ģʽ	
	Device_ReadBytes_2(&temp,BH1750_ADDRESS);
  Device_WriteBytes_2(BHPowDown ,BHAddWrite); 
//	Device_WriteBytes_2(BHReset ,BH1750_ADDRESS);
	
  	//bh_data_send(BHPowOn);    //������������
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

