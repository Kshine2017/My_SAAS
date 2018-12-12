#include "DHT11.h"
#include "systerm_time.h"

static void DHT11_Rst(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;         
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_7);						 //PC7 输出高

	
	DHT11_OUT();
  DHT11_DATA_OUT(0);
  delay_ms(20);
  DHT11_DATA_OUT(1); 
	delay_us(40);
}

/*
 检测DHT11是否存在
 返回0：存在
 返回1：不存在
*/
static u8 DHT11_Check(void)
{
	u32 retry = 0;
	DHT11_IN();  
	while( DHT11_DATA_IN == 0 ){
		retry++;
		delay_us(1);	
		if(retry>= 100){
			return 1;
		}
	}
  retry = 0;
	while (DHT11_DATA_IN == 1)
	{
		retry++;
		delay_us(1);
		if(retry>=100){
			return 1;
		}
	}
	return 0;
}

//从DHT11读取一个位
//返回值：1/0
static u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT11_DATA_IN && retry<100)//等待变为低电平
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DATA_IN && retry<100)//等待变高电平
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//等待40us
	if(DHT11_DATA_IN)return 1;
	else return 0;		   
}


//从DHT11读取一个字节
//返回值：读到的数据
static u8 DHT11_Read_Byte(void)    
{        
   u8 i,dat= 0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
  }						    
  return dat;
}


//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
u8 DHT11_Read_Data(t_Evironment_Data*data)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			data->Humidity=buf[0];
			data->Tempeture=buf[2];
		}
	}else return 1;
	return 0;	    
}


//初始化DHT11的IO口 DQ 同时检测DHT11的存在
//返回1:不存在
//返回0:存在    	 
u8 DHT11_Init(void)
{	 		    
	DHT11_Rst();  //复位DHT11
	return DHT11_Check();//等待DHT11的回应
} 
