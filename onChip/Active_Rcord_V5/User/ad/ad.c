#include "ad.h"
#include "string.h"
#include "Environment.h"
u16 AD_Value[N][M] = {0}; //AD�ɼ�ֵ���浥Ԫ
u16 Power_Value  =0 ;             //��Դֵ
u16 ADC_VO[5]={0};

float VREF_VO=1.22;   //AD��׼��ѹ
/*AD�ɼ�GPIO�ڳ�ʼ��*/
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

/*ADģ���������*/
static void AD_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 																													/*PCLK2��8��Ƶ����9MHz*/
	DMA_DeInit(DMA1_Channel1); 																																	//DMA1 CH1ͨ��
  
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);																					//ʹ��AHB����ʱ��---DMA1
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);																				//ʹ��AHB����ʱ��---ADC1
  
	
	//==================================================================================================
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)(&(ADC1->DR));	 											//�������ַ��ADC��ַ  ��== 0x0000 0100��
	DMA_InitStructure.DMA_MemoryBaseAddr 			= (u32)&AD_Value;																	//�ڴ����ַ��AD_Value[][]
	DMA_InitStructure.DMA_DIR 								= DMA_DIR_PeripheralSRC;													//������Ϊ���ݴ������Դ  ��DMA_DIR_PeripheralDST������Ϊ���ݴ����Ŀ�ĵ�  ��
	DMA_InitStructure.DMA_BufferSize 					= M*N;																						//DMA����ռ��С
	
	DMA_InitStructure.DMA_PeripheralInc 			= DMA_PeripheralInc_Disable;											//�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc 					=  DMA_MemoryInc_Enable;  												//�ڴ��ַ�Ĵ�������
	
	
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_HalfWord;								//�������ݿ�ȣ�16λ
	DMA_InitStructure.DMA_MemoryDataSize 			= DMA_MemoryDataSize_HalfWord;										//�ڴ����ݿ�ȣ�16λ
	
	DMA_InitStructure.DMA_Mode 								= DMA_Mode_Circular;															//����CANde DMAģʽ�������� ѭ������ģʽ
	DMA_InitStructure.DMA_Priority 						= DMA_Priority_High;															//�����ȼ�
	DMA_InitStructure.DMA_M2M 								= DMA_M2M_Disable;																//�ر��ڴ浽�ڴ�Ĵ���
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);																								//��ʼ��������������װ�ؽ�ͨ��1
	DMA_Cmd(DMA1_Channel1, ENABLE);																															//ʹ��DMAͨ��1
	//==================================================================================================
	
	ADC_InitStructure.ADC_Mode 								= ADC_Mode_Independent;														//����ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode 				= ENABLE ; 	 																			//��ֹɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
	ADC_InitStructure.ADC_ContinuousConvMode 	= ENABLE;																					//��������ת��ģʽ������ͣ�ؽ���ADCת��
	ADC_InitStructure.ADC_ExternalTrigConv 		= ADC_ExternalTrigConv_None;											//��ʹ���ⲿ����ת����ת���������������
	ADC_InitStructure.ADC_DataAlign 					= ADC_DataAlign_Right; 														//�ɼ������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel 				= M;	 																						//Ҫת����ͨ����Ŀ  ��  5  ���   4
	ADC_Init(ADC1, &ADC_InitStructure);
  ADC_TempSensorVrefintCmd(ENABLE);                                       										//�����ڲ��¶ȴ�����
	
	
#if 0																																				//=============  5  =================
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_4 ,1, ADC_SampleTime_239Cycles5 );  						//��Դ
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_6 ,2, ADC_SampleTime_239Cycles5 );  						//����
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_8 ,3, ADC_SampleTime_239Cycles5 );  						//����
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor ,4, ADC_SampleTime_239Cycles5 );		//�¶�
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_Vrefint ,5, ADC_SampleTime_239Cycles5 ); 			//�ο���ѹ
//	
#elif 1       																															//==============  4  ===============


/*
ADC1��PA0��PA7��PB0��PB1��PC0��PC3��
ADC2��PA0��PA7��PB0��PB1��PC0��PC3��
ADC3��PA0��PA3��PC0��PC3��
*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4 ,					1, ADC_SampleTime_239Cycles5 );  		//��Դ   										PA4���� ��ӦADCͨ��4
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1 ,					2, ADC_SampleTime_239Cycles5 );  		//����											PA1 ���� ��ӦADCͨ��1
	ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor ,	3, ADC_SampleTime_239Cycles5 ); 		//�¶� �¶Ȳ����ڲ��¶�ģ�� 		������		��Ӧͨ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_Vrefint ,		4, ADC_SampleTime_239Cycles5 ); 		//�ο���ѹ											������
#endif

	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	  
	ADC_ResetCalibration(ADC1);																																	//��λADC1��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));																									//ѭ���ȴ����õ�ADC��λУ׼�Ĵ�����״̬

	ADC_StartCalibration(ADC1);																																	//����ADC1���Զ�У׼
	while(ADC_GetCalibrationStatus(ADC1));																											//ѭ���ȴ����õ�ADCУ׼״̬
	 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);																											//ʹ�� ���ת����������
}




/*
AD�ɼ������˲���ð������֮���ȥ��Сֵ�����ֵ֮��ȡƽ��

data: ��������
num :DMA����������±�  0����Դ 1������ 2������ 3���¶�4���ڲ��ο���ѹ
num :DMA����������±�  0����Դ 1������ 2���¶�3���ڲ��ο���ѹ
����ֵ���˲���ķ���ֵ
*/
static u16 Bubble_Sort(u16 (*data)[M],u8 num)
{
	unsigned short temp;
	unsigned char i, j ,k;
	u32 count = 0;        				//��ʱֵ
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


/*AD�ɼ��˿�����*/
void AD_InitConfig(void)
{
	AD_GPIOInit();
	AD_Config();
}




/*********************************************************************************************
�������� �����㵱ǰ��صĵ�ʣ�����
     data�� DMA����
    value:  �������,�����ٷֱȷŴ���100��
**********************************************************************************************/
void P_Get_Value(u16 (*data)[M],u8 *value)
{
	u16 AD[N][M] = {0};
	volatile float templ =0;
	memcpy(AD,data,sizeof(AD));  
	
	
	
	//templ=VREF_VO*Bubble_Sort(AD,0)/Bubble_Sort(AD,4);// 0����Դ 1������ 2���¶�  3���ڲ��ο���ѹ
		templ=VREF_VO*Bubble_Sort(AD,0)/Bubble_Sort(AD,3);
//  printf("templ = %f \r\n",templ);
//��Դ��ѹ����10k��10k  ���Ե�ѹ����﮵��3.7VΪ��־100%������AD��ѹ��Ϊ1.85V��ӦADֵΪ2296
//��Դ��ѹֵΪ1.65V����Ӧ��ص�ѹΪ3.3V ADֵΪ2047
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
�������� �� ��ȡ������ADֵ
     data�� DMA����
     num �� ��ӦADת�����
     value:  �������
**********************************************************************************************/
void V_Get_Value(u16 (*data)[M],u8 *v_v)
{
	u16 AD[N][M] = {0};
	memcpy(AD,data,sizeof(AD));  
	
	
	
	//*v_v = (u8)(VREF_VO*Bubble_Sort(AD,2)/Bubble_Sort(AD,4)*10);
	*v_v = (u8)(VREF_VO*Bubble_Sort(AD,1)/Bubble_Sort(AD,3)*40);// 0����Դ 1������ 2���¶�  3���ڲ��ο���ѹ
	if(*v_v > 255) *v_v = 255;
}

/*********************************************************************************************
��������: �õ��¶�ֵ
**        �¶ȼ��㹫ʽ�� {(V25 - VSENSE) / Avg_Slope} + 25   V25����ֵΪ1.43V Avg_Slope= 4.3mv/��
     data�� DMA����
     num �� ��ӦADת�����
     value:  �������
**********************************************************************************************/
void T_Get_Value(u16 (*data)[M],u8 *valuet)
{
	u16 AD[N][M] = {0};
	float tempt =0;
	memcpy(AD,data,sizeof(AD)); 


	
	//tempt = VREF_VO*Bubble_Sort(AD,3)/Bubble_Sort(AD,4);// 0����Դ 1������ 2���¶�  3���ڲ��ο���ѹ
	tempt = VREF_VO*Bubble_Sort(AD,2)/Bubble_Sort(AD,3);
  //*valuet=(1.43-tempt)/0.0043+25;
	*valuet =(u8)((1775-tempt)*100 / 53)/10 +25 ;   //1775��1.43��ADֵ  53��4.3mv����10��Ӧ��ADֵ
}




#if 0
///*************************************************************************************************
//**�������ܣ� ��ʼ��ͨ��ADCx_INI16����ͨ����ȡоƬ�ڲ��������ĵ�ѹֵ
//**
//**************************************************************************************************/
//static void T_Ad_Init()
//{
//	ADC_InitTypeDef ADC_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
//	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 																				/*PCLK2��8��Ƶ����9MHz*/
//	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;												//����ADCģʽ
//	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 	 													//��ֹɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
//	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;												//��������ת��ģʽ������ͣ�ؽ���ADCת��
//	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;				//��ʹ���ⲿ����ת��
//	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 										//�ɼ������Ҷ���
//	ADC_InitStructure.ADC_NbrOfChannel = 1;	 																	//Ҫת����ͨ����Ŀ1
//	ADC_Init(ADC1, &ADC_InitStructure);
//	
//	ADC_TempSensorVrefintCmd(ENABLE);                                       //�����ڲ��¶ȴ�����
//	ADC_Cmd(ADC1,ENABLE);                                                   //ʹ��ADC2
//	ADC_ResetCalibration(ADC1);                                             //����ָ��ADC2�ĸ�λ�Ĵ���
//	while(ADC_GetResetCalibrationStatus(ADC1)) ;                            //���ADC2����У׼�Ĵ�����״̬������״̬��ȴ�
//	
//	ADC_StartCalibration(ADC1);
//	while(ADC_GetCalibrationStatus(ADC1));                                  //��ȡADC2��У׼��������״̬��ȴ�
//	
//}


///***********************************************************************************
//�������ܣ� ����һ��ת����������ADCֵ
//************************************************************************************/
//static u16 T_Get_Adc(u8 ch)
//{
//	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5); //����ͨ��ת��˳���ת��ʱ������
//	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
//	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
////	ADC_SoftwareStartConvCmd(ADC2,DISABLE);
//	return ADC_GetConversionValue(ADC1);
//}


///**************************************************************************************
//��������: ���õ����õĴ���֮��ֱ�ӷ���оƬ�¶�ֵ
//**        �¶ȼ��㹫ʽ�� {(V25 - VSENSE) / Avg_Slope} + 25   V25����ֵΪ1.43V Avg_Slope= 4.3mv/��
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
////  		  printf("************��ǰ���¶�ֵ��%d\r\n",tempo);
////		  *temperature =(u8)((1430-tempo) / 4.3 + 25);	  
//	 }
//}

#endif



/*********************************************************************************************
�������� �����ݵ�ǰAD�˲����ֵ��Ӧ���߽��в��  �ڰ�ʱ��3.119V  �չ�� 0.95V  ǿ�� 0.2V
           ����Խǿ���õ���ADֵԽ�󡣹�ǿ�ȼ�����ͨ��ad.h��maco�������á�
     data�� DMA����
    value:  �������
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
