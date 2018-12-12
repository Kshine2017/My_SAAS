#include "rtc.h"
#include "time.h"
#include "err_control.h"
/* 秒中断标志，进入秒中断时置1，当时间被刷新之后清0 */
__IO uint32_t TimeDisplay = 0;
__IO uint32_t Sec_Store_flag = 0;
volatile create_flag  create_file_flag = {0};  		//打开文件标志
volatile t_run_struct_data run_time_data = {GetTempet_RunTime,ADXL345_RunTime,GetLight_RunTime,GetVoice_RunTime,POWER_CHECK_RunTime}; 	//

/*星期，生肖用文字ASCII码*/
uint8_t const *WEEK_STR[] = {"日", "一", "二", "三", "四", "五", "六"};

/*时间结构体*/
volatile struct rtc_time systmtime;
volatile struct rtc_time BeformSystmtime; //当前系统前一天的时间
volatile struct rtc_time AfterSystmtime; //当前系统后一天的时间

//设置北京时间
static void Time_set_beijingTime(uint32_t TimeVar,volatile struct rtc_time *tm);


/**********       将hour/min/sec转换成sec          *********/
unsigned int hour_sec(volatile struct rtc_time *time)
{
    return time->tm_sec + time->tm_min*60 + time->tm_hour*60*60;
}


/**延迟函数*/
void delayms(uint16_t count)
{
    u16 i = 0,j = 0;
    for( i = count ; count > 0; count --) {
        for(j = 1000; j> 0; j--) {}
    }
}


/***************************************************************************/


/*
 * 函数名：NVIC_Configuration
 * 描述  ：配置RTC秒中断的主中断优先级为1，次优先级为0
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
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
 * 函数名：RTC_Configuration
 * 描述  ：配置RTC
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
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
     * 因为RTC时钟是低速的，内环时钟是高速的，所以要同步
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
//*******函数名：RTC_Set()
//*******说  明：设置时间
//*******参  数：把输入的时钟转换为秒钟,以1970年1月1日为基准,1970~2099年为合法年份
//*******返回值：1:设置失败	0:设置成功
/***************************************************************************/
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	                                       //判断是不是合法年 
	if(smon > 12)return 1;				                                       //判断是不是合法月					
	if(sday > 31)return 1;				                                       //判断是不是合法日
	if(hour > 24)return 1;				                                       //判断是不是合法时
	if(min > 60)return 1; 				                                       //判断是不是合法分
	if(sec > 60)return 1; 				                                       //判断是不是合法秒
	
	for(t=1970;t<syear;t++)	               									   //把所有年份的秒钟相加
	{
		if(Is_Leap_Year(t))seccount+=31622400;								   //闰年的秒钟数
		else seccount+=31536000;			  								   //平年的秒钟数
	}
	smon-=1;	      		                                                   //0是有效位
	for(t=0;t<smon;t++)	      												   //把前面月份的秒钟数相加
	{
		seccount+=(u32)month_days[t]*86400;									   //月份秒钟数相加
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;                           //闰年2月份增加一天的秒钟数	   
	}
	seccount+=(u32)(sday-1)*86400;                                             //把前面日期的秒钟数相加 
	seccount+=(u32)hour*3600;                                                  //小时秒钟数
    seccount+=(u32)min*60;	                                                   //分钟秒钟数
	seccount+=sec;                                                             //最后的秒钟加上去
    RTC_EnterConfigMode();                                                //进入RTC配置模式											   //使能RTC和后备寄存器访问 				    
	RTC_SetCounter(seccount);												   //设置RTC计数器的值
	RTC_WaitForLastTask();	                                         //等待最近一次对RTC操作完成
	RTC_ExitConfigMode();	//等待最近一次对RTC寄存器的写操作完成  
	return 0;	    														   //设置成功
}


/*
 * 函数名：RTC_CheckAndConfig
 * 描述  ：检查并配置RTC
 * 输入  ：用于读取RTC时间的结构体指针
 * 输出  ：无
 * 调用  ：外部调用
 */
void RTC_CheckAndConfig(volatile struct rtc_time *tm)
{
    /*在启动时检查备份寄存器BKP_DR1，如果内容不是0xB5B5,
    则需重新配置时间并询问用户调整时间*/
    u8 i=0;
	    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);
	
    if (BKP_ReadBackupRegister(BKP_DR1) != 0xB5B5)
    {
				//指示灯 闪烁几次后，常量
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

        Time_set_beijingTime(RTC_GetCounter(),&systmtime); //当前系统时间
        printf("当前时间 = %d:%d:%d:%d:%d:%d\r\n",systmtime.tm_year,systmtime.tm_mon,systmtime.tm_mday,systmtime.tm_hour,systmtime.tm_min,systmtime.tm_sec);
        Time_set_beijingTime(RTC_GetCounter()-86400,&BeformSystmtime);//当前系统前一天的数据。
        printf("前天当前时间 = %d:%d:%d:%d:%d:%d\r\n",BeformSystmtime.tm_year,BeformSystmtime.tm_mon,BeformSystmtime.tm_mday,BeformSystmtime.tm_hour,BeformSystmtime.tm_min,BeformSystmtime.tm_sec);
        Time_set_beijingTime(RTC_GetCounter()+86400,&AfterSystmtime);//当前系统前一天的数据。
        printf("后天当前时间 = %d:%d:%d:%d:%d:%d\r\n",AfterSystmtime.tm_year,AfterSystmtime.tm_mon,AfterSystmtime.tm_mday,AfterSystmtime.tm_hour,AfterSystmtime.tm_min,AfterSystmtime.tm_sec);

        printf("设置当前的时间戳 = %d\r\n",mktimev(&systmtime));
    }
    else
    {
        /*启动无需设置新时钟*/
        /*检查是否掉电重启*/
        if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
        {
            printf("\r\n\r\n Power On Reset occurred....");
        }
        /*检查是否Reset复位*/
        else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
        {
            printf("\r\n\r\n External Reset occurred....");
        }

        printf("\r\n No need to configure RTC....");

        /*等待寄存器同步*/
        RTC_WaitForSynchro();

        /*允许RTC秒中断*/
        RTC_ITConfig(RTC_IT_SEC, ENABLE);

        /*等待上次RTC寄存器写操作完成*/
        RTC_WaitForLastTask();
    }

    /* Clear reset flags */
    RCC_ClearFlag();

    Time_set_beijingTime(RTC_GetCounter(),&systmtime); //当前系统时间
    printf("当前时间 = %d:%d:%d:%d:%d:%d\r\n",systmtime.tm_year,systmtime.tm_mon,systmtime.tm_mday,systmtime.tm_hour,systmtime.tm_min,systmtime.tm_sec);
    Time_set_beijingTime(RTC_GetCounter()-86400,&BeformSystmtime);//当前系统前一天的数据。
    printf("前天当前时间 = %d:%d:%d:%d:%d:%d\r\n",BeformSystmtime.tm_year,BeformSystmtime.tm_mon,BeformSystmtime.tm_mday,BeformSystmtime.tm_hour,BeformSystmtime.tm_min,BeformSystmtime.tm_sec);
    Time_set_beijingTime(RTC_GetCounter()+86400,&AfterSystmtime);//当前系统前一天的数据。
    printf("后天当前时间 = %d:%d:%d:%d:%d:%d\r\n",AfterSystmtime.tm_year,AfterSystmtime.tm_mon,AfterSystmtime.tm_mday,AfterSystmtime.tm_hour,AfterSystmtime.tm_min,AfterSystmtime.tm_sec);

}


/***         把RTC计数值转换成标准北京时间      ******/
static void Time_set_beijingTime(uint32_t TimeVar,volatile struct rtc_time *tm)
{
    static u16 daycnt=0;
    u32 timecount=0;
    u32 temp=0;
    u16 temp1=0;
    timecount=TimeVar;
    temp=timecount/86400;   //得到天数(秒钟数对应的)
    if(daycnt!=temp)//超过一天了
    {
        daycnt=temp;
        temp1=1970;	//从1970年开始
        while(temp>=365)
        {
            if(Is_Leap_Year(temp1))//是闰年
            {
                if(temp>=366)temp-=366;//闰年的秒钟数
                else
                {
                    temp1++;
                    break;
                }
            }
            else temp-=365;	  //平年
            temp1++;
        }
        tm->tm_year =temp1;//得到年份
        temp1=0;
        while(temp>=28)//超过了一个月
        {
            if(Is_Leap_Year(tm->tm_year)&&temp1==1)//当年是不是闰年/2月份
            {
                if(temp>=29)temp-=29;//闰年的秒钟数
                else break;
            }
            else
            {
                if(temp>=month_days[temp1])temp -= month_days[temp1];//平年
                else break;
            }
            temp1++;
        }
        tm->tm_mon =temp1+1;	//得到月份
        tm->tm_mday =temp+1;  	//得到日期
    }
    temp=timecount%86400;     		//得到秒钟数
    tm->tm_hour =temp/3600;     	//小时
    tm->tm_min=(temp%3600)/60; 	//分钟
    tm->tm_sec=(temp%3600)%60; 	//秒钟
}



/**						根据时间创建文件	，前提条件是系统运行了。		****/
static void create_file(volatile struct rtc_time *time,volatile create_flag *flag,module_state*state)
{
    static int i =0;
    unsigned int temp = 0;
    if(state->run ) {

        if( (12 == time->tm_hour) && (0 == time->tm_min) && (0 ==time->tm_sec) ) { //在每天中午12时进行天文件打开
            flag->Open_ActDerec_flag = ~0;  																				//打开一天的目录文件夹
        }
        if( (0 == time->tm_min) && (0 ==time->tm_sec) ) { 													//在每个小时00:00时进EXCEL文件打开
            if( 12 == time->tm_hour ) {
                if(flag->Open_ActDerec_flag) {
                    flag->Open_ActiveExcel_flag = ~0;  															//打开一天的目录文件夹
                }
            } else flag->Open_ActiveExcel_flag = ~0;  															//打开一天的目录文件夹
        }
        if(struct_active_file.res_state) {   																				//如果文件是打开状态
            flag->Write_Data_flag = ~0;   																					//写数据到表中
        }
    }
}

/***创建文件系统文件夹	1s运行一次***/
void creat_file_control(void) {
    if (TimeDisplay == 1) {
        // printf("module.run = %d\r\n",module.run);
        if( module.run) {              //如果机器开始运行
            Time_set_beijingTime(RTC_GetCounter(),&systmtime); //当前系统时间
            Time_set_beijingTime(RTC_GetCounter()-86400,&BeformSystmtime);//当前系统前一天的数据。
            Time_set_beijingTime(RTC_GetCounter()+86400,&AfterSystmtime);//当前系统前一天的数据。
            create_file(&systmtime,&create_file_flag,&module);  //创建/写数据文件管理

        }
        TimeDisplay = 0;
    }
}



void refresh_system_time()
{
    Time_set_beijingTime(RTC_GetCounter(),&systmtime); //当前系统时间
    Time_set_beijingTime(RTC_GetCounter()-86400,&BeformSystmtime);//当前系统前一天的数据。
    Time_set_beijingTime(RTC_GetCounter()+86400,&AfterSystmtime);//当前系统前一天的数据。
}
/************************END OF FILE***************************************/
