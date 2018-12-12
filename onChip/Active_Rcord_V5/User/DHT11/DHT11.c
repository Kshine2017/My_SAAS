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
	GPIO_SetBits(GPIOC,GPIO_Pin_7);						 //PC7 �����

	
	DHT11_OUT();
  DHT11_DATA_OUT(0);
  delay_ms(20);
  DHT11_DATA_OUT(1); 
	delay_us(40);
}

/*
 ���DHT11�Ƿ����
 ����0������
 ����1��������
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

//��DHT11��ȡһ��λ
//����ֵ��1/0
static u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT11_DATA_IN && retry<100)//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DATA_IN && retry<100)//�ȴ���ߵ�ƽ
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//�ȴ�40us
	if(DHT11_DATA_IN)return 1;
	else return 0;		   
}


//��DHT11��ȡһ���ֽ�
//����ֵ������������
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


//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
u8 DHT11_Read_Data(t_Evironment_Data*data)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
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


//��ʼ��DHT11��IO�� DQ ͬʱ���DHT11�Ĵ���
//����1:������
//����0:����    	 
u8 DHT11_Init(void)
{	 		    
	DHT11_Rst();  //��λDHT11
	return DHT11_Check();//�ȴ�DHT11�Ļ�Ӧ
} 
