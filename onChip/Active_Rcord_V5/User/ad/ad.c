#include "ad.h"
#include "string.h"
#include "Environment.h"
u16 AD_Value[N][M] = {0}; //AD采集值保存单元
u16 Power_Value  =0 ;             //电源值
u16 ADC_VO[5]={0};

float VREF_VO=1.22;   //AD标准电压
/*AD采集GPIO口初始化*/
static void AD_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_1;  //PA4 ad_VCC   PA1 micVoice  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);			
	
}

/*AD模块采用配置*/
static void AD_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 																													/*PCLK2的8分频，即9MHz*/
	DMA_DeInit(DMA1_Channel1); 																																	//DMA1 CH1通道
  
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);																					//使能AHB外设时钟---DMA1
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);																				//使能AHB外设时钟---ADC1
  
	
	//==================================================================================================
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)(&(ADC1->DR));	 											//外设基地址，ADC地址  （== 0x0000 0100）
	DMA_InitStructure.DMA_MemoryBaseAddr 			= (u32)&AD_Value;																	//内存基地址，AD_Value[][]
	DMA_InitStructure.DMA_DIR 								= DMA_DIR_PeripheralSRC;													//外设作为数据传输的来源  （DMA_DIR_PeripheralDST外设作为数据传输的目的地  ）
	DMA_InitStructure.DMA_BufferSize 					= M*N;																						//DMA缓存空间大小
	
	DMA_InitStructure.DMA_PeripheralInc 			= DMA_PeripheralInc_Disable;											//外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc 					=  DMA_MemoryInc_Enable;  												//内存地址寄存器递增
	
	
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_HalfWord;								//外设数据宽度，16位
	DMA_InitStructure.DMA_MemoryDataSize 			= DMA_MemoryDataSize_HalfWord;										//内存数据宽度，16位
	
	DMA_InitStructure.DMA_Mode 								= DMA_Mode_Circular;															//设置CANde DMA模式，工作在 循环缓存模式
	DMA_InitStructure.DMA_Priority 						= DMA_Priority_High;															//高优先级
	DMA_InitStructure.DMA_M2M 								= DMA_M2M_Disable;																//关闭内存到内存的传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);																								//初始化，将上述设置装载进通道1
	DMA_Cmd(DMA1_Channel1, ENABLE);																															//使能DMA通道1
	//==================================================================================================
	
	ADC_InitStructure.ADC_Mode 								= ADC_Mode_Independent;														//独立ADC模式
	ADC_InitStructure.ADC_ScanConvMode 				= ENABLE ; 	 																			//禁止扫描模式，扫描模式用于多通道采集
	ADC_InitStructure.ADC_ContinuousConvMode 	= ENABLE;																					//开启连续转换模式，即不停地进行ADC转换
	ADC_InitStructure.ADC_ExternalTrigConv 		= ADC_ExternalTrigConv_None;											//不使用外部触发转换，转换由软件出发启动
	ADC_InitStructure.ADC_DataAlign 					= ADC_DataAlign_Right; 														//采集数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel 				= M;	 																						//要转换的通道数目  从  5  变成   4
	ADC_Init(ADC1, &ADC_InitStructure);
  ADC_TempSensorVrefintCmd(ENABLE);                                       										//开启内部温度传感器
	
	
#if 0																																				//=============  5  =================
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_4 ,1, ADC_SampleTime_239Cycles5 );  						//电源
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_6 ,2, ADC_SampleTime_239Cycles5 );  						//光线
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_8 ,3, ADC_SampleTime_239Cycles5 );  						//声音
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor ,4, ADC_SampleTime_239Cycles5 );		//温度
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_Vrefint ,5, ADC_SampleTime_239Cycles5 ); 			//参考电压
//	
#elif 1       																															//==============  4  ===============


/*
ADC1（PA0～PA7，PB0～PB1，PC0～PC3）
ADC2（PA0～PA7，PB0～PB1，PC0～PC3）
ADC3（PA0～PA3，PC0～PC3）
*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4 ,					1, ADC_SampleTime_239Cycles5 );  		//电源   										PA4引脚 对应ADC通道4
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1 ,					2, ADC_SampleTime_239Cycles5 );  		//声音											PA1 引脚 对应ADC通道1
	ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor ,	3, ADC_SampleTime_239Cycles5 ); 		//温度 温度采用内部温度模块 		无引脚		对应通道
	ADC_RegularChannelConfig(ADC1, ADC_Channel_Vrefint ,		4, ADC_SampleTime_239Cycles5 ); 		//参考电压											无引脚
#endif

	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	  
	ADC_ResetCalibration(ADC1);																																	//复位ADC1的校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));																									//循环等待，得到ADC复位校准寄存器的状态

	ADC_StartCalibration(ADC1);																																	//开启ADC1的自动校准
	while(ADC_GetCalibrationStatus(ADC1));																											//循环等待，得到ADC校准状态
	 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);																											//使能 软件转换启动功能
}




/*
AD采集数据滤波，冒泡排序之后减去最小值和最大值之后取平均

data: 缓冲数组
num :DMA输出的数组下标  0：电源 1，光线 2，声音 3，温度4，内部参考电压
num :DMA输出的数组下标  0：电源 1，声音 2，温度3，内部参考电压
返回值：滤波后的返回值
*/
static u16 Bubble_Sort(u16 (*data)[M],u8 num)
{
	unsigned short temp;
	unsigned char i, j ,k;
	u32 count = 0;        				//临时值
	for (i = 0; i< 7-1; i++){
		for (j = 0; j < 7-i-1; j++)
		{
			if (*(*(data + j) + num) > *(*(data + j+1) + num)){
				temp = *(*(data + j+1) + num );
				*(*(data + j+1) + num) = *(*(data + j) + num);
				*(*(data + j) + num) = temp;
			}
		}
	}
	for(k = 1;k<N-1;k++){
		count += *(*(data+k)+num);
	}	
	return (u16)( count/ (N-2) );
}


/*AD采集端口配置*/
void AD_InitConfig(void)
{
	AD_GPIOInit();
	AD_Config();
}




/*********************************************************************************************
函数功能 ：计算当前电池的的剩余电量
     data： DMA数据
    value:  结果储存,电量百分比放大了100倍
**********************************************************************************************/
void P_Get_Value(u16 (*data)[M],u8 *value)
{
	u16 AD[N][M] = {0};
	volatile float templ =0;
	memcpy(AD,data,sizeof(AD));  
	
	
	
	//templ=VREF_VO*Bubble_Sort(AD,0)/Bubble_Sort(AD,4);// 0：电源 1，声音 2，温度  3，内部参考电压
		templ=VREF_VO*Bubble_Sort(AD,0)/Bubble_Sort(AD,3);
//  printf("templ = %f \r\n",templ);
//电源分压电阻10k：10k  测试电压按照锂电池3.7V为标志100%电量，AD电压则为1.85V对应AD值为2296
//电源低压值为1.65V。对应电池电压为3.3V AD值为2047
 // tempo = (tempo *3.3 /4096 ) *2 *100;
	templ = templ*2*100 ;
  if((templ ) >= 370){
    templ  = 370;
  }
   if((templ ) <= 330){
    templ = 330;
  }  
	*value = (u8)((templ- 330)*100  / (370-330) );
  
 // if( *value >= 100)  *value = 100;
}

/*********************************************************************************************
函数功能 ： 获取声音的AD值
     data： DMA数据
     num ： 对应AD转换序号
     value:  结果储存
**********************************************************************************************/
void V_Get_Value(u16 (*data)[M],u8 *v_v)
{
	u16 AD[N][M] = {0};
	memcpy(AD,data,sizeof(AD));  
	
	
	
	//*v_v = (u8)(VREF_VO*Bubble_Sort(AD,2)/Bubble_Sort(AD,4)*10);
	*v_v = (u8)(VREF_VO*Bubble_Sort(AD,1)/Bubble_Sort(AD,3)*40);// 0：电源 1，声音 2，温度  3，内部参考电压
	if(*v_v > 255) *v_v = 255;
}

/*********************************************************************************************
函数功能: 得到温度值
**        温度计算公式： {(V25 - VSENSE) / Avg_Slope} + 25   V25典型值为1.43V Avg_Slope= 4.3mv/℃
     data： DMA数据
     num ： 对应AD转换序号
     value:  结果储存
**********************************************************************************************/
void T_Get_Value(u16 (*data)[M],u8 *valuet)
{
	u16 AD[N][M] = {0};
	float tempt =0;
	memcpy(AD,data,sizeof(AD)); 


	
	//tempt = VREF_VO*Bubble_Sort(AD,3)/Bubble_Sort(AD,4);// 0：电源 1，声音 2，温度  3，内部参考电压
	tempt = VREF_VO*Bubble_Sort(AD,2)/Bubble_Sort(AD,3);
  //*valuet=(1.43-tempt)/0.0043+25;
	*valuet =(u8)((1775-tempt)*100 / 53)/10 +25 ;   //1775是1.43的AD值  53是4.3mv扩大10对应的AD值
}




#if 0
///*************************************************************************************************
//**函数功能： 初始化通道ADCx_INI16输入通道读取芯片内部传感器的电压值
//**
//**************************************************************************************************/
//static void T_Ad_Init()
//{
//	ADC_InitTypeDef ADC_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
//	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 																				/*PCLK2的8分频，即9MHz*/
//	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;												//独立ADC模式
//	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 	 													//禁止扫描模式，扫描模式用于多通道采集
//	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;												//开启连续转换模式，即不停地进行ADC转换
//	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;				//不使用外部触发转换
//	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 										//采集数据右对齐
//	ADC_InitStructure.ADC_NbrOfChannel = 1;	 																	//要转换的通道数目1
//	ADC_Init(ADC1, &ADC_InitStructure);
//	
//	ADC_TempSensorVrefintCmd(ENABLE);                                       //开启内部温度传感器
//	ADC_Cmd(ADC1,ENABLE);                                                   //使能ADC2
//	ADC_ResetCalibration(ADC1);                                             //重置指定ADC2的复位寄存器
//	while(ADC_GetResetCalibrationStatus(ADC1)) ;                            //获得ADC2重置校准寄存器的状态，设置状态则等待
//	
//	ADC_StartCalibration(ADC1);
//	while(ADC_GetCalibrationStatus(ADC1));                                  //获取ADC2的校准程序，设置状态则等待
//	
//}


///***********************************************************************************
//函数功能： 启动一次转换，并返回ADC值
//************************************************************************************/
//static u16 T_Get_Adc(u8 ch)
//{
//	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5); //设置通过转换顺序和转换时间周期
//	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
//	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
////	ADC_SoftwareStartConvCmd(ADC2,DISABLE);
//	return ADC_GetConversionValue(ADC1);
//}


///**************************************************************************************
//函数功能: 调用到设置的次数之后直接返回芯片温度值
//**        温度计算公式： {(V25 - VSENSE) / Avg_Slope} + 25   V25典型值为1.43V Avg_Slope= 4.3mv/℃
//***************************************************************************************/
//void T_Get_Adc_Average(u8 ch, u8 count,u8* temperature)
//{
//	 static u32 num = 0;
//	 static u8 times = 0;
//	 u16 tempo =0;
//	 num += T_Get_Adc(ch);
//	 times++;
//	 if( times == count){
//				tempo = num / times;
//				num = 0;
//				times = 0;
////				tempo = 1200 *tempo/4096;
////  		  printf("************当前的温度值是%d\r\n",tempo);
////		  *temperature =(u8)((1430-tempo) / 4.3 + 25);	  
//	 }
//}

#endif



/*********************************************************************************************
函数功能 ：根据当前AD滤波后的值对应光线进行查表  黑暗时，3.119V  日光灯 0.95V  强光 0.2V
           光线越强，得到的AD值越大。光强等级可以通过ad.h的maco进行设置。
     data： DMA数据
    value:  结果储存
**********************************************************************************************/
void L_Get_Value(u16 (*data)[M],u8 *value)
{
//	u16 AD[N][M] = {0};
//	volatile   float tempo =0;
//	memcpy(AD,data,sizeof(AD));  
//	tempo = VREF_VO*Bubble_Sort(AD,1)/Bubble_Sort(AD,4);
//	
//		if(tempo >= 3.2)
//			*value = Light_Eight;
//		else if( tempo >= 2.8 &&tempo< 3.2 )
//			*value =Light_Seven;
//		else if( tempo >= 2.4 && tempo< 2.8 )
//			*value =  Light_Five; 
//		else if( tempo >= 2.0 && tempo< 2.4 )
//			*value =Light_Four;
//		else if( tempo >= 1.6 && tempo< 2.0 )
//			*value = Light_Six;
//		else if( tempo >= 1.2 && tempo< 1.6 )
//			*value = Light_Three;
//		else if( tempo >= 0.8 && tempo< 1.2 )
//			*value =  Light_Two;
//		else if( tempo >= 0.4 && tempo< 0.8 )
//			*value = Light_One;
}
