/**
  ******************************************************************************
  * @file    Active_Rcord
  * @author  xiefei
  * @version V1.0
  * @date    2016-07-16
  * @brief   usb2.0forSD
  ******************************************************************************
  **/

#include "stm32f10x.h"
#include "usart1.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "machine_control.h"
#include "err_control.h"
#include "file.h"
#include "rtc.h"
#include "key.h"
#include "time.h"
#include "i2c_1.h"
#include "i2c_2.h"
#include "ad.h"
#include "systerm_time.h"
#include "ADXL345.h"
#include "Flash.h"
#include "usb.h"
#include "stm32f10x_iwdg.h"
#include "../BH1750fvi/bh1750fvi.h"
#include "stm32f10x_it.h"



/*独立看门狗初始化*/
static void IWDG_Init(u8 prer,u16 reload)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //允许写 IWDG
    IWDG_SetPrescaler(prer);                      //设置分频
    IWDG_SetReload(reload);                       //设置reload
    IWDG_ReloadCounter();                         //重载值
    IWDG_Enable();                                //使能IDWG
}

/*独立看门狗喂狗*/
void IWDG_Feed(void)
{
    IWDG_ReloadCounter();
}


/*
系统设置，当前系统时钟设置为48MHz
*/
static __INLINE void system_init(void)
{
    delay_init();                           //系统定时器初始化
    USART1_Config();
    machine_state_init(&module);  			//模块状态初始化
    KEY_init();								/*按键初始化*/
    LED_GPIO_init();
    USB_Control_GPIO_Init();
//		PowerKEY_Control(&module);              //按键处理  /*led pin init*/
    RTC_NVIC_Config();		                /* 实时时钟中断配置  */
	
		
    RTC_CheckAndConfig(&systmtime);   		/*检测RTC时钟是否需要更新*/
		
    //IWDG_Init(IWDG_Prescaler_64,625);       //看门狗溢出时间为1s  :40000/64/625

    if(Flash_Init())
    {   //检测flash芯片是否已经焊接
        module.flash_state = ERR;
    }
    //file_struct_init( &module,&File_System);	//挂载根文件系统和文件夹初始化

    /* USB_device config*/
    Set_System();                           /*USB模拟U盘初始化*/

    /*设置USB时钟为48M*/
    Set_USBClock();

    /*配置USB中断(包括SDIO中断)*/
    USB_Interrupts_Config();

    /*USB初始化*/
    USB_Init();
    i2c_GPIO_init();                    		//i2c模块管脚  PB6 PB7
    AD_InitConfig();                 				//ADC1模块初始化
    TIM3_Int_Init(TIME3_BAST_TIME,TIME3_PSR);//定时器3初始化  50ms中断一次

    //ADXL345_init();                         //ADX初始化设置
    ADXL345_init_sleep_wakeup();

    if(Check_ADXL345())
    {
        module.active_state = ERR;
				printf("ADXL345 ERROR！");
    }

    i2c_GPIO_init_2();	//配置IIC引脚 PB0 PB1
    BH1750_init();
    if(Check_BH1750()) //如果通信失败 ，返回1
    {
        module.light_state = ERR;
       printf("BH1750 IIC 通信失败！");
    }

    USB_Bat_state();                        //充电管理

//下面代码开机
    POWER_ON;
    if(Usb_State == !Connect)
    {   //USB线没有插入
        module.run = ON;
        file_struct_init( &module,&File_System);//挂载根文件系统和文件夹初始化
				printf("Usb_State = disConnect ，模块运行！");
    } 
		else
		{
        module.run = OFF;
				printf("Usb_State = Connect ，模块不运行！");
		}
    module.power_state = ON;
    GREEN_LED_ON;
    printf("\n\n\n 开机=====>>>>>  初始化完成  <<<<< \n\n\n");
		//USART_Cmd(USART1,DISABLE);
}


int main(void)
{
    FRESULT res;
		int flag_weekup=0;
    system_init(); //系统初始化
		Write_Flag_ALL_Situation(flag_When_power_on,flag_update_time_yes,flag_flseek_yes,-1);
		if(stop_flag)stop_flag=0;//清除开机时，adxl发来的错误信号
    while(1)
    {
        if(stop_flag && Usb_State == Disconnect) //静止   usb未连接
        {
            printf("\n\n main --->  stop mode \n\n");

            if(struct_active_file.res_state != 0) //如果当前有文件打开状态
						{  
							
								//Write_Flag_for_Sleep(-6,1);
								Write_Flag_ALL_Situation(flag_When_readytosleep,flag_update_time_no,flag_flseek_yes,1);
							
                res=f_close( &struct_active_file.file);
                if( res != FR_OK)
                {
                    printf("-----------------------------> 【失败】  关闭文件		struct_active_file.file \n");
                    GREEN_LED_ON;
                }
                else
                {   printf("-----------------------------> 【成功】  关闭文件		struct_active_file.file \n");
                    struct_active_file.res_state =0;
                    GREEN_LED_OFF;
                }
            }
						
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); //开电源管理时钟PWR_Regulator_LowPower
            //MOST_GPIO_AnalogIn();
            //PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI|PWR_STOPEntry_WFE);//进入停机
            PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);
            SystemInit();
            //Reset_ThoseGPIO();
            stop_flag=0;
            printf("\n\n out of stop mode! now TimeDisplay = %d\n\n",TimeDisplay);
            //TimeDisplay=1;		//至位，刷新系统时间
            //file_Lseek_flag=1;//表明需要重新定位文件夹，文件，和行
						//Write_Flag_ALL_Situation(flag_When_wake_up_now,flag_update_time_yes,flag_flseek_yes,-1);//如果文件没打开，则会临时打开，写入数据后，关闭 //重新定位文件夹，文件和行，并写入标志
						flag_weekup=1;
        }
        usart1_dealHandle( usart1_buf);  //串口处理函数
        //IWDG_Feed();	                    //喂狗，避免复位
        //PowerKEY_Control(&module);        //按键处理(采用跳变检测，效果不好，执行开机和关机任务)
        DealKey(&module);                   //按键处理(采用延时消斗检测，执行关机任务)
	      USB_Bat_state();                    //检测充电引脚和满电引脚，修改USB连接状态
        USB_Control_File();                 //USB连接状态时，关闭文件系统管理
        Power_Manage(&module);              //根据电源电量状态，决定是否报警和关机
				if(flag_weekup)
				{
					flag_weekup=0;
					Write_Flag_ALL_Situation(flag_When_wake_up_now,flag_update_time_yes,flag_flseek_yes,-1);
				}			
        err_control(&module,led_display);   //根据设备状态，控制指示灯的亮灭
        creat_file_control();
        File_Manage(&create_file_flag);
        Collect_data(&AD345_strcut_Data,&Environment_data,&module);/*采集一次活动数据，有定时器3控制*/
    }
}
/* -------------------------------------end of file -------------------------------------------- */