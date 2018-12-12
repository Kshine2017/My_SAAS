#include "key.h"
#include "systerm_time.h"
#include "err_control.h"
#include "usb.h"
#include "file.h"
#include "stm32f10x_exti.h"
extern void IWDG_Feed(void);
/**********************************************************
函数名       ：SysTick_init
函数功能描述 ：Ms级延时函数
				延时nms
				注意nms的范围
				SysTick->LOAD为24位寄存器,所以,最大延时为:
				nms<=0xffffff*8*1000/SYSCLK
				SYSCLK单位为Hz,nms单位为ms
				对72M条件下,nms<=1864
**********************************************************/
void Delayms(vu16 nms)
{
    vu16 i;
    while(nms--)
        for(i=0; i<2000; i++);
    IWDG_Feed();
}


void KEY_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //电源键
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;  //电源管理芯片使能管脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;  //USB充电检测管脚:电量充满 pin5:STDBY    正在充电：pin9:CHRG
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);



    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
    EXTI_InitStructure.EXTI_Line		=			EXTI_Line4;					//PB4
    EXTI_InitStructure.EXTI_Mode 		= 		EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = 		EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = 		ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 												//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


    //中断初始化配置   上升沿触发
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource9);

    EXTI_InitStructure.EXTI_Line		=			EXTI_Line9;					//PB9
    EXTI_InitStructure.EXTI_Mode 		= 		EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = 		EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = 		ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 												//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource8);
    EXTI_InitStructure.EXTI_Line		=			EXTI_Line8;					//PB8
    EXTI_InitStructure.EXTI_Mode 		= 		EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = 		EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = 		ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 												//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;										//使能外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x02;					//抢占优先级2，
    NVIC_InitStructure.NVIC_IRQChannelSubPriority 				= 0x04;					//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;												//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel =EXTI4_IRQn;										//使能外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x02;					//抢占优先级2，
    NVIC_InitStructure.NVIC_IRQChannelSubPriority 				= 0x05;					//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;												//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);

}

/**
函数功能：按键按下是否有效，返回传入参数的按键有效值。
函数传入参数：需要检测的按键管脚输入值
注意:按键按下电平为低
**/

__INLINE static uint8_t Powerkey_Scan(uint8_t keyvalue)
{
    static u8 count_down = 1;
    static u8 Key_press_flag = 0;
    Key_press_flag = keyvalue & (keyvalue ^ count_down);
    count_down = keyvalue;
    return Key_press_flag ;
}


/******按键检测函数*************************************************************************/
void DealKey(module_state *Module_State)
{
    FRESULT res;
    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4) == 0)
    {
        Delayms(1000);
        if( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4) == 0)
        {

            while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4) == 0)
            {
                GREEN_LED_ON;;
                Delayms(50);
                GREEN_LED_OFF;
                Delayms(50);
            }
            if( ON == Module_State->power_state )
            {
                Module_State->run = OFF;                        //设备当前停止运行
                GREEN_LED_ON;
                if(struct_active_file.res_state != 0) //如果当前有文件打开状态
                {
                    Write_Flag_ALL_Situation(flag_When_power_off,flag_update_time_yes,flag_flseek_yes,1);
                    res=f_close( &struct_active_file.file);
                    if(res==FR_OK)
                    {
                        struct_active_file.res_state=0;
                        printf("-----------------------------> 【成功】 关闭文件 \r\n");
                    }
                    else
                    {
                        printf("-----------------------------> 【失败】 关闭文件 \r\n");
                        return;
                    }

                }
                if( DirName.Dir_restate!= 0)
                {
                    res=f_closedir(&DirName.DIR);
                    if(res==FR_OK)
                    {
                        DirName.Dir_restate=0;
                        printf("-----------------------------> 【成功】 关闭文件夹 0：/Active Data\r\n");
                    }
                    else
                    {
                        printf("-----------------------------> 【失败】 关闭文件夹 0：/Active Data\r\n");
                        return;
                    }
                }

                f_mount(NULL,"0:",1);            //卸载文件系统
                File_System.FATFS_state = 0;
                Module_State->power_state = OFF;

                GREEN_LED_OFF;
                printf( "run = %d power_state = %d\r\n",Module_State->run,Module_State->power_state);
                POWER_OFF;    //关机
            }

        }
    }
}
/***
	电源管理控制 PB.0 电源使能脚 PB.1电源按键检测脚 PB.2 USB充电检测管脚
***/

void PowerKEY_Control(module_state *Module_State)
{
    uint8_t PowerKey_Flag = 0;
    PowerKey_Flag = Powerkey_Scan(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4));
    if(PowerKey_Flag) {
        if( ON == Module_State->power_state ) {
            Module_State->run = OFF;                        //设备当前停止运行
            GREEN_LED_ON;
            if(struct_active_file.res_state != 0) {  //如果当前有文件打开状态
                f_close( &struct_active_file.file);
            }
            if( DirName.Dir_restate!= 0) {
                f_closedir(&DirName.DIR);
            }
            f_mount(NULL,"0:",1);            //卸载文件系统
            File_System.FATFS_state = 0;
            Module_State->power_state = OFF;

            GREEN_LED_OFF;
            printf( "run = %d power_state = %d\r\n",Module_State->run,Module_State->power_state);
            POWER_OFF;    //关机
        } else {
            POWER_ON;
            if(Usb_State == !Connect) { //USB线没有插入
                Module_State->run = ON;
            }
            Module_State->power_state = ON;
            GREEN_LED_ON;
            printf( "run = %d power_state = %d\r\n",Module_State->run,Module_State->power_state);
        }
    }
}


///*
//	根据按键决定设备是否开始工作，如果设备已经开始工作就停止工作。
//*/

//void key_control(module_state *state)
//{
//	Power_Control(state);  			//电源关机控制    开机要求2s钟，关机可以按下就可以
//  PauseKey_Control(state); 		//开始暂停键
//}


/**										endif               **/
