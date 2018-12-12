#include "rtc.h"
#include "time.h"
#include "err_control.h"
/* ���жϱ�־���������ж�ʱ��1����ʱ�䱻ˢ��֮����0 */
__IO uint32_t TimeDisplay = 0;
__IO uint32_t Sec_Store_flag = 0;
volatile create_flag  create_file_flag = {0};  		//���ļ���־
volatile t_run_struct_data run_time_data = {GetTempet_RunTime,ADXL345_RunTime,GetLight_RunTime,GetVoice_RunTime,POWER_CHECK_RunTime}; 	//

/*���ڣ���Ф������ASCII��*/
uint8_t const *WEEK_STR[] = {"��", "һ", "��", "��", "��", "��", "��"};

/*ʱ��ṹ��*/
volatile struct rtc_time systmtime;
volatile struct rtc_time BeformSystmtime; //��ǰϵͳǰһ���ʱ��
volatile struct rtc_time AfterSystmtime; //��ǰϵͳ��һ���ʱ��

//���ñ���ʱ��
static void Time_set_beijingTime(uint32_t TimeVar,volatile struct rtc_time *tm);


/**********       ��hour/min/secת����sec          *********/
unsigned int hour_sec(volatile struct rtc_time *time)
{
    return time->tm_sec + time->tm_min*60 + time->tm_hour*60*60;
}


/**�ӳٺ���*/
void delayms(uint16_t count)
{
    u16 i = 0,j = 0;
    for( i = count ; count > 0; count --) {
        for(j = 1000; j> 0; j--) {}
    }
}


/***************************************************************************/


/*
 * ��������NVIC_Configuration
 * ����  ������RTC���жϵ����ж����ȼ�Ϊ1�������ȼ�Ϊ0
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void RTC_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* Enable the RTC Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * ��������RTC_Configuration
 * ����  ������RTC
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void RTC_Configuration(void)
{
    u8 count = 0;
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Reset Backup Domain */
    BKP_DeInit();

    /* Enable LSE */
    RCC_LSEConfig(RCC_LSE_ON);

    /* Wait till LSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {
		++count;
		delayms(10);
		if( count >= 200 ){
			break;
		}
	}

    /* Select LSE as RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Enable RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC registers synchronization
     * ��ΪRTCʱ���ǵ��ٵģ��ڻ�ʱ���Ǹ��ٵģ�����Ҫͬ��
     */
    RTC_WaitForSynchro();

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Set RTC prescaler: set RTC period to 1sec */
    RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) = 1HZ */

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
}


static void Delayms(vu16 nms)
{
    vu16 i;
    while(nms--)
        for(i=0; i<2000; i++);
}



/***************************************************************************/
//*******��������RTC_Set()
//*******˵  ��������ʱ��
//*******��  �����������ʱ��ת��Ϊ����,��1970��1��1��Ϊ��׼,1970~2099��Ϊ�Ϸ����
//*******����ֵ��1:����ʧ��	0:���óɹ�
/***************************************************************************/
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	                                       //�ж��ǲ��ǺϷ��� 
	if(smon > 12)return 1;				                                       //�ж��ǲ��ǺϷ���					
	if(sday > 31)return 1;				                                       //�ж��ǲ��ǺϷ���
	if(hour > 24)return 1;				                                       //�ж��ǲ��ǺϷ�ʱ
	if(min > 60)return 1; 				                                       //�ж��ǲ��ǺϷ���
	if(sec > 60)return 1; 				                                       //�ж��ǲ��ǺϷ���
	
	for(t=1970;t<syear;t++)	               									   //��������ݵ��������
	{
		if(Is_Leap_Year(t))seccount+=31622400;								   //�����������
		else seccount+=31536000;			  								   //ƽ���������
	}
	smon-=1;	      		                                                   //0����Чλ
	for(t=0;t<smon;t++)	      												   //��ǰ���·ݵ����������
	{
		seccount+=(u32)month_days[t]*86400;									   //�·����������
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;                           //����2�·�����һ���������	   
	}
	seccount+=(u32)(sday-1)*86400;                                             //��ǰ�����ڵ���������� 
	seccount+=(u32)hour*3600;                                                  //Сʱ������
    seccount+=(u32)min*60;	                                                   //����������
	seccount+=sec;                                                             //�������Ӽ���ȥ
    RTC_EnterConfigMode();                                                //����RTC����ģʽ											   //ʹ��RTC�ͺ󱸼Ĵ������� 				    
	RTC_SetCounter(seccount);												   //����RTC��������ֵ
	RTC_WaitForLastTask();	                                         //�ȴ����һ�ζ�RTC�������
	RTC_ExitConfigMode();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������  
	return 0;	    														   //���óɹ�
}


/*
 * ��������RTC_CheckAndConfig
 * ����  ����鲢����RTC
 * ����  �����ڶ�ȡRTCʱ��Ľṹ��ָ��
 * ���  ����
 * ����  ���ⲿ����
 */
void RTC_CheckAndConfig(volatile struct rtc_time *tm)
{
    /*������ʱ��鱸�ݼĴ���BKP_DR1��������ݲ���0xB5B5,
    ������������ʱ�䲢ѯ���û�����ʱ��*/
    u8 i=0;
	    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);
	
    if (BKP_ReadBackupRegister(BKP_DR1) != 0xB5B5)
    {
				//ָʾ�� ��˸���κ󣬳���
		for(i=0;i<10;i++)
		{	
			GREEN_LED_ON;
			RED_LED_OFF;
			Delayms(50);
			GREEN_LED_OFF;
			RED_LED_ON;
			Delayms(50);
		}
		GREEN_LED_ON;	
		//RED_LED_ON;						
        printf("\r\n\r\n RTC not yet configured....");

        /* RTC Configuration */
        RTC_Configuration();

        printf("\r\n\r\n RTC configured....");
         
		    /* Wait until last write operation on RTC registers has finished */
	    RTC_Set(2018,8,27,15,51,50);
       /* Get wday */
        GregorianDay(tm);
		
        BKP_WriteBackupRegister(BKP_DR1, 0xB5B5);

        Time_set_beijingTime(RTC_GetCounter(),&systmtime); //��ǰϵͳʱ��
        printf("��ǰʱ�� = %d:%d:%d:%d:%d:%d\r\n",systmtime.tm_year,systmtime.tm_mon,systmtime.tm_mday,systmtime.tm_hour,systmtime.tm_min,systmtime.tm_sec);
        Time_set_beijingTime(RTC_GetCounter()-86400,&BeformSystmtime);//��ǰϵͳǰһ������ݡ�
        printf("ǰ�쵱ǰʱ�� = %d:%d:%d:%d:%d:%d\r\n",BeformSystmtime.tm_year,BeformSystmtime.tm_mon,BeformSystmtime.tm_mday,BeformSystmtime.tm_hour,BeformSystmtime.tm_min,BeformSystmtime.tm_sec);
        Time_set_beijingTime(RTC_GetCounter()+86400,&AfterSystmtime);//��ǰϵͳǰһ������ݡ�
        printf("���쵱ǰʱ�� = %d:%d:%d:%d:%d:%d\r\n",AfterSystmtime.tm_year,AfterSystmtime.tm_mon,AfterSystmtime.tm_mday,AfterSystmtime.tm_hour,AfterSystmtime.tm_min,AfterSystmtime.tm_sec);

        printf("���õ�ǰ��ʱ��� = %d\r\n",mktimev(&systmtime));
    }
    else
    {
        /*��������������ʱ��*/
        /*����Ƿ��������*/
        if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
        {
            printf("\r\n\r\n Power On Reset occurred....");
        }
        /*����Ƿ�Reset��λ*/
        else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
        {
            printf("\r\n\r\n External Reset occurred....");
        }

        printf("\r\n No need to configure RTC....");

        /*�ȴ��Ĵ���ͬ��*/
        RTC_WaitForSynchro();

        /*����RTC���ж�*/
        RTC_ITConfig(RTC_IT_SEC, ENABLE);

        /*�ȴ��ϴ�RTC�Ĵ���д�������*/
        RTC_WaitForLastTask();
    }

    /* Clear reset flags */
    RCC_ClearFlag();

    Time_set_beijingTime(RTC_GetCounter(),&systmtime); //��ǰϵͳʱ��
    printf("��ǰʱ�� = %d:%d:%d:%d:%d:%d\r\n",systmtime.tm_year,systmtime.tm_mon,systmtime.tm_mday,systmtime.tm_hour,systmtime.tm_min,systmtime.tm_sec);
    Time_set_beijingTime(RTC_GetCounter()-86400,&BeformSystmtime);//��ǰϵͳǰһ������ݡ�
    printf("ǰ�쵱ǰʱ�� = %d:%d:%d:%d:%d:%d\r\n",BeformSystmtime.tm_year,BeformSystmtime.tm_mon,BeformSystmtime.tm_mday,BeformSystmtime.tm_hour,BeformSystmtime.tm_min,BeformSystmtime.tm_sec);
    Time_set_beijingTime(RTC_GetCounter()+86400,&AfterSystmtime);//��ǰϵͳǰһ������ݡ�
    printf("���쵱ǰʱ�� = %d:%d:%d:%d:%d:%d\r\n",AfterSystmtime.tm_year,AfterSystmtime.tm_mon,AfterSystmtime.tm_mday,AfterSystmtime.tm_hour,AfterSystmtime.tm_min,AfterSystmtime.tm_sec);

}


/***         ��RTC����ֵת���ɱ�׼����ʱ��      ******/
static void Time_set_beijingTime(uint32_t TimeVar,volatile struct rtc_time *tm)
{
    static u16 daycnt=0;
    u32 timecount=0;
    u32 temp=0;
    u16 temp1=0;
    timecount=TimeVar;
    temp=timecount/86400;   //�õ�����(��������Ӧ��)
    if(daycnt!=temp)//����һ����
    {
        daycnt=temp;
        temp1=1970;	//��1970�꿪ʼ
        while(temp>=365)
        {
            if(Is_Leap_Year(temp1))//������
            {
                if(temp>=366)temp-=366;//�����������
                else
                {
                    temp1++;
                    break;
                }
            }
            else temp-=365;	  //ƽ��
            temp1++;
        }
        tm->tm_year =temp1;//�õ����
        temp1=0;
        while(temp>=28)//������һ����
        {
            if(Is_Leap_Year(tm->tm_year)&&temp1==1)//�����ǲ�������/2�·�
            {
                if(temp>=29)temp-=29;//�����������
                else break;
            }
            else
            {
                if(temp>=month_days[temp1])temp -= month_days[temp1];//ƽ��
                else break;
            }
            temp1++;
        }
        tm->tm_mon =temp1+1;	//�õ��·�
        tm->tm_mday =temp+1;  	//�õ�����
    }
    temp=timecount%86400;     		//�õ�������
    tm->tm_hour =temp/3600;     	//Сʱ
    tm->tm_min=(temp%3600)/60; 	//����
    tm->tm_sec=(temp%3600)%60; 	//����
}



/**						����ʱ�䴴���ļ�	��ǰ��������ϵͳ�����ˡ�		****/
static void create_file(volatile struct rtc_time *time,volatile create_flag *flag,module_state*state)
{
    static int i =0;
    unsigned int temp = 0;
    if(state->run ) {

        if( (12 == time->tm_hour) && (0 == time->tm_min) && (0 ==time->tm_sec) ) { //��ÿ������12ʱ�������ļ���
            flag->Open_ActDerec_flag = ~0;  																				//��һ���Ŀ¼�ļ���
        }
        if( (0 == time->tm_min) && (0 ==time->tm_sec) ) { 													//��ÿ��Сʱ00:00ʱ��EXCEL�ļ���
            if( 12 == time->tm_hour ) {
                if(flag->Open_ActDerec_flag) {
                    flag->Open_ActiveExcel_flag = ~0;  															//��һ���Ŀ¼�ļ���
                }
            } else flag->Open_ActiveExcel_flag = ~0;  															//��һ���Ŀ¼�ļ���
        }
        if(struct_active_file.res_state) {   																				//����ļ��Ǵ�״̬
            flag->Write_Data_flag = ~0;   																					//д���ݵ�����
        }
    }
}

/***�����ļ�ϵͳ�ļ���	1s����һ��***/
void creat_file_control(void) {
    if (TimeDisplay == 1) {
        // printf("module.run = %d\r\n",module.run);
        if( module.run) {              //���������ʼ����
            Time_set_beijingTime(RTC_GetCounter(),&systmtime); //��ǰϵͳʱ��
            Time_set_beijingTime(RTC_GetCounter()-86400,&BeformSystmtime);//��ǰϵͳǰһ������ݡ�
            Time_set_beijingTime(RTC_GetCounter()+86400,&AfterSystmtime);//��ǰϵͳǰһ������ݡ�
            create_file(&systmtime,&create_file_flag,&module);  //����/д�����ļ�����

        }
        TimeDisplay = 0;
    }
}



void refresh_system_time()
{
    Time_set_beijingTime(RTC_GetCounter(),&systmtime); //��ǰϵͳʱ��
    Time_set_beijingTime(RTC_GetCounter()-86400,&BeformSystmtime);//��ǰϵͳǰһ������ݡ�
    Time_set_beijingTime(RTC_GetCounter()+86400,&AfterSystmtime);//��ǰϵͳǰһ������ݡ�
}
/************************END OF FILE***************************************/
