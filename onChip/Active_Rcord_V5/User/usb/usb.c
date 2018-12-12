
#include "usb.h"
#include "file.h"
#include "err_control.h"
#include "key.h"
USB_State Usb_State = Disconnect;      //USB接口的状态
u8 chongdian_flag;


/*************************************************************************************/

/*************************************************************************
**函数功能： USB管脚初始化
*****************************************************************************/
void USB_Control_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;       //USB充电管理管脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIOB->BSRR |= GPIO_Pin_8 | GPIO_Pin_9 ;
}
//检测有没有充电
void USB_Bat_state(void)
{
    if(BAT1_Charg_State == 0 ) 
		{
        Usb_State = Connect;
        chongdian_flag = chong_state;
    }
    else if(BAT1_Charg_Finish == 0) 
		{
        chongdian_flag = chong_finsh;
        Usb_State = Connect;
    }
    else {
        chongdian_flag = No_chongdian;

        Usb_State = Disconnect;
    }
}
/**
  * USB线插入之后需要管理文件，或者USB线拔出之后的管理
**/
void USB_Control_File(void)
{
		FRESULT res;
    if(Usb_State == Connect) 
		{
				//Write_Flag_ALL_Situation(flag_When_usb_connect,flag_update_time_no,flag_flseek_no,1);
			
        module.run = OFF;                        //设备当前停止运行
        if(struct_active_file.res_state != 0) //如果当前有文件打开状态
				{  
						//Write_Flag_ALL_Situation(flag_When_usb_connect,flag_update_time_no,flag_flseek_no,1);
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
						
    }

    //如果模块已停止运行，且 USB断开连接
    if(module.run == OFF  &&  Usb_State == Disconnect)
    {
        //关机或者继续运行（发现继续运行会导致 无法写入数据 情况的发生）
				//Write_Flag_ALL_Situation(flag_When_usb_disconnect,flag_update_time_yes,flag_flseek_yes,1);
        POWER_OFF;
//			module.run = ON;
//			if( File_System.FATFS_state == 0)
//					file_struct_init( &module,&File_System);//挂载根文件系统和文件夹初始化


    }


}

/*电源管理，计算电池电量*/
//修改：解决 出现电量警告之后 不能消除警告的问题
void Power_Manage(module_state *state)
{
    if(Environment_data.Power_data > 5)				// 电量在6~100范围内
    {

					state->power_capacity = OK;


//				//如果文件系统已经被关闭，则需要重新打开
//
//
    }
    else if( Environment_data.Power_data >0)	// 1~5
    {
        state->power_capacity = WARING;

    }
    else																		//    0~0
    {

        state->run = OFF;
        state->power_capacity = ERR;
        if(struct_active_file.res_state != 0)
        {   //如果当前有文件打开状态
            f_close( &struct_active_file.file);
        }
        if( DirName.Dir_restate!= 0)
        {
            f_closedir(&DirName.DIR);
        }

        f_mount(NULL,"0:",1);            //卸载文件系统
        File_System.FATFS_state = 0;
        POWER_OFF;
    }
}
/*
usb_device 控制状态,暂时不用
*/
static void usb_device_control(module_state*state)
{
	
    static u8 sem_i = 1;
    static u8 sem_j = 1;
    if(!state ->run) 
			{
        if(sem_i) 					{
            Set_USBClock();									/*设置USB时钟为48M*/
            USB_Interrupts_Config();				/*配置USB中断(包括SDIO中断)*/
            USB_Init();											/*USB初始化*/
            USB_Cable_Config (ENABLE); //打开USB
            sem_i = 0;
            sem_j = 1;
        }
    } 
			else 
			{
			if(sem_j ) 
			{
            USB_Cable_Config (DISABLE); //关闭USB
            sem_i = 1;
            sem_j = 0;
      }
    }

}


