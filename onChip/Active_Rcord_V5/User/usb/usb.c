
#include "usb.h"
#include "file.h"
#include "err_control.h"
#include "key.h"
USB_State Usb_State = Disconnect;      //USB�ӿڵ�״̬
u8 chongdian_flag;


/*************************************************************************************/

/*************************************************************************
**�������ܣ� USB�ܽų�ʼ��
*****************************************************************************/
void USB_Control_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;       //USB������ܽ�
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIOB->BSRR |= GPIO_Pin_8 | GPIO_Pin_9 ;
}
//�����û�г��
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
  * USB�߲���֮����Ҫ�����ļ�������USB�߰γ�֮��Ĺ���
**/
void USB_Control_File(void)
{
		FRESULT res;
    if(Usb_State == Connect) 
		{
				//Write_Flag_ALL_Situation(flag_When_usb_connect,flag_update_time_no,flag_flseek_no,1);
			
        module.run = OFF;                        //�豸��ǰֹͣ����
        if(struct_active_file.res_state != 0) //�����ǰ���ļ���״̬
				{  
						//Write_Flag_ALL_Situation(flag_When_usb_connect,flag_update_time_no,flag_flseek_no,1);
            res=f_close( &struct_active_file.file);
						if(res==FR_OK)
						{
								struct_active_file.res_state=0;
								printf("-----------------------------> ���ɹ��� �ر��ļ� \r\n");
						}
						else
						{
								printf("-----------------------------> ��ʧ�ܡ� �ر��ļ� \r\n");
								return;
						}		
						
        }
        if( DirName.Dir_restate!= 0) 
				{
            res=f_closedir(&DirName.DIR);
						if(res==FR_OK)
						{
								DirName.Dir_restate=0;
								printf("-----------------------------> ���ɹ��� �ر��ļ��� 0��/Active Data\r\n");
						}
						else
						{
								printf("-----------------------------> ��ʧ�ܡ� �ر��ļ��� 0��/Active Data\r\n");
								return;
						}		
        }

				f_mount(NULL,"0:",1);            //ж���ļ�ϵͳ
				File_System.FATFS_state = 0;
						
    }

    //���ģ����ֹͣ���У��� USB�Ͽ�����
    if(module.run == OFF  &&  Usb_State == Disconnect)
    {
        //�ػ����߼������У����ּ������лᵼ�� �޷�д������ ����ķ�����
				//Write_Flag_ALL_Situation(flag_When_usb_disconnect,flag_update_time_yes,flag_flseek_yes,1);
        POWER_OFF;
//			module.run = ON;
//			if( File_System.FATFS_state == 0)
//					file_struct_init( &module,&File_System);//���ظ��ļ�ϵͳ���ļ��г�ʼ��


    }


}

/*��Դ���������ص���*/
//�޸ģ���� ���ֵ�������֮�� �����������������
void Power_Manage(module_state *state)
{
    if(Environment_data.Power_data > 5)				// ������6~100��Χ��
    {

					state->power_capacity = OK;


//				//����ļ�ϵͳ�Ѿ����رգ�����Ҫ���´�
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
        {   //�����ǰ���ļ���״̬
            f_close( &struct_active_file.file);
        }
        if( DirName.Dir_restate!= 0)
        {
            f_closedir(&DirName.DIR);
        }

        f_mount(NULL,"0:",1);            //ж���ļ�ϵͳ
        File_System.FATFS_state = 0;
        POWER_OFF;
    }
}
/*
usb_device ����״̬,��ʱ����
*/
static void usb_device_control(module_state*state)
{
	
    static u8 sem_i = 1;
    static u8 sem_j = 1;
    if(!state ->run) 
			{
        if(sem_i) 					{
            Set_USBClock();									/*����USBʱ��Ϊ48M*/
            USB_Interrupts_Config();				/*����USB�ж�(����SDIO�ж�)*/
            USB_Init();											/*USB��ʼ��*/
            USB_Cable_Config (ENABLE); //��USB
            sem_i = 0;
            sem_j = 1;
        }
    } 
			else 
			{
			if(sem_j ) 
			{
            USB_Cable_Config (DISABLE); //�ر�USB
            sem_i = 1;
            sem_j = 0;
      }
    }

}


