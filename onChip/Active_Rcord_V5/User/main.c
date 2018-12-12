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



/*�������Ź���ʼ��*/
static void IWDG_Init(u8 prer,u16 reload)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //����д IWDG
    IWDG_SetPrescaler(prer);                      //���÷�Ƶ
    IWDG_SetReload(reload);                       //����reload
    IWDG_ReloadCounter();                         //����ֵ
    IWDG_Enable();                                //ʹ��IDWG
}

/*�������Ź�ι��*/
void IWDG_Feed(void)
{
    IWDG_ReloadCounter();
}


/*
ϵͳ���ã���ǰϵͳʱ������Ϊ48MHz
*/
static __INLINE void system_init(void)
{
    delay_init();                           //ϵͳ��ʱ����ʼ��
    USART1_Config();
    machine_state_init(&module);  			//ģ��״̬��ʼ��
    KEY_init();								/*������ʼ��*/
    LED_GPIO_init();
    USB_Control_GPIO_Init();
//		PowerKEY_Control(&module);              //��������  /*led pin init*/
    RTC_NVIC_Config();		                /* ʵʱʱ���ж�����  */
	
		
    RTC_CheckAndConfig(&systmtime);   		/*���RTCʱ���Ƿ���Ҫ����*/
		
    //IWDG_Init(IWDG_Prescaler_64,625);       //���Ź����ʱ��Ϊ1s  :40000/64/625

    if(Flash_Init())
    {   //���flashоƬ�Ƿ��Ѿ�����
        module.flash_state = ERR;
    }
    //file_struct_init( &module,&File_System);	//���ظ��ļ�ϵͳ���ļ��г�ʼ��

    /* USB_device config*/
    Set_System();                           /*USBģ��U�̳�ʼ��*/

    /*����USBʱ��Ϊ48M*/
    Set_USBClock();

    /*����USB�ж�(����SDIO�ж�)*/
    USB_Interrupts_Config();

    /*USB��ʼ��*/
    USB_Init();
    i2c_GPIO_init();                    		//i2cģ��ܽ�  PB6 PB7
    AD_InitConfig();                 				//ADC1ģ���ʼ��
    TIM3_Int_Init(TIME3_BAST_TIME,TIME3_PSR);//��ʱ��3��ʼ��  50ms�ж�һ��

    //ADXL345_init();                         //ADX��ʼ������
    ADXL345_init_sleep_wakeup();

    if(Check_ADXL345())
    {
        module.active_state = ERR;
				printf("ADXL345 ERROR��");
    }

    i2c_GPIO_init_2();	//����IIC���� PB0 PB1
    BH1750_init();
    if(Check_BH1750()) //���ͨ��ʧ�� ������1
    {
        module.light_state = ERR;
       printf("BH1750 IIC ͨ��ʧ�ܣ�");
    }

    USB_Bat_state();                        //������

//������뿪��
    POWER_ON;
    if(Usb_State == !Connect)
    {   //USB��û�в���
        module.run = ON;
        file_struct_init( &module,&File_System);//���ظ��ļ�ϵͳ���ļ��г�ʼ��
				printf("Usb_State = disConnect ��ģ�����У�");
    } 
		else
		{
        module.run = OFF;
				printf("Usb_State = Connect ��ģ�鲻���У�");
		}
    module.power_state = ON;
    GREEN_LED_ON;
    printf("\n\n\n ����=====>>>>>  ��ʼ�����  <<<<< \n\n\n");
		//USART_Cmd(USART1,DISABLE);
}


int main(void)
{
    FRESULT res;
		int flag_weekup=0;
    system_init(); //ϵͳ��ʼ��
		Write_Flag_ALL_Situation(flag_When_power_on,flag_update_time_yes,flag_flseek_yes,-1);
		if(stop_flag)stop_flag=0;//�������ʱ��adxl�����Ĵ����ź�
    while(1)
    {
        if(stop_flag && Usb_State == Disconnect) //��ֹ   usbδ����
        {
            printf("\n\n main --->  stop mode \n\n");

            if(struct_active_file.res_state != 0) //�����ǰ���ļ���״̬
						{  
							
								//Write_Flag_for_Sleep(-6,1);
								Write_Flag_ALL_Situation(flag_When_readytosleep,flag_update_time_no,flag_flseek_yes,1);
							
                res=f_close( &struct_active_file.file);
                if( res != FR_OK)
                {
                    printf("-----------------------------> ��ʧ�ܡ�  �ر��ļ�		struct_active_file.file \n");
                    GREEN_LED_ON;
                }
                else
                {   printf("-----------------------------> ���ɹ���  �ر��ļ�		struct_active_file.file \n");
                    struct_active_file.res_state =0;
                    GREEN_LED_OFF;
                }
            }
						
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); //����Դ����ʱ��PWR_Regulator_LowPower
            //MOST_GPIO_AnalogIn();
            //PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI|PWR_STOPEntry_WFE);//����ͣ��
            PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);
            SystemInit();
            //Reset_ThoseGPIO();
            stop_flag=0;
            printf("\n\n out of stop mode! now TimeDisplay = %d\n\n",TimeDisplay);
            //TimeDisplay=1;		//��λ��ˢ��ϵͳʱ��
            //file_Lseek_flag=1;//������Ҫ���¶�λ�ļ��У��ļ�������
						//Write_Flag_ALL_Situation(flag_When_wake_up_now,flag_update_time_yes,flag_flseek_yes,-1);//����ļ�û�򿪣������ʱ�򿪣�д�����ݺ󣬹ر� //���¶�λ�ļ��У��ļ����У���д���־
						flag_weekup=1;
        }
        usart1_dealHandle( usart1_buf);  //���ڴ�����
        //IWDG_Feed();	                    //ι�������⸴λ
        //PowerKEY_Control(&module);        //��������(���������⣬Ч�����ã�ִ�п����͹ػ�����)
        DealKey(&module);                   //��������(������ʱ������⣬ִ�йػ�����)
	      USB_Bat_state();                    //��������ź��������ţ��޸�USB����״̬
        USB_Control_File();                 //USB����״̬ʱ���ر��ļ�ϵͳ����
        Power_Manage(&module);              //���ݵ�Դ����״̬�������Ƿ񱨾��͹ػ�
				if(flag_weekup)
				{
					flag_weekup=0;
					Write_Flag_ALL_Situation(flag_When_wake_up_now,flag_update_time_yes,flag_flseek_yes,-1);
				}			
        err_control(&module,led_display);   //�����豸״̬������ָʾ�Ƶ�����
        creat_file_control();
        File_Manage(&create_file_flag);
        Collect_data(&AD345_strcut_Data,&Environment_data,&module);/*�ɼ�һ�λ���ݣ��ж�ʱ��3����*/
    }
}
/* -------------------------------------end of file -------------------------------------------- */