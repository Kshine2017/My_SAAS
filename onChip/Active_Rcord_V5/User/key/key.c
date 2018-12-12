#include "key.h"
#include "systerm_time.h"
#include "err_control.h"
#include "usb.h"
#include "file.h"
#include "stm32f10x_exti.h"
extern void IWDG_Feed(void);
/**********************************************************
������       ��SysTick_init
������������ ��Ms����ʱ����
				��ʱnms
				ע��nms�ķ�Χ
				SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
				nms<=0xffffff*8*1000/SYSCLK
				SYSCLK��λΪHz,nms��λΪms
				��72M������,nms<=1864
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

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //��Դ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;  //��Դ����оƬʹ�ܹܽ�
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;  //USB�����ܽ�:�������� pin5:STDBY    ���ڳ�磺pin9:CHRG
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);



    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
    EXTI_InitStructure.EXTI_Line		=			EXTI_Line4;					//PB4
    EXTI_InitStructure.EXTI_Mode 		= 		EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = 		EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = 		ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 												//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


    //�жϳ�ʼ������   �����ش���
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource9);

    EXTI_InitStructure.EXTI_Line		=			EXTI_Line9;					//PB9
    EXTI_InitStructure.EXTI_Mode 		= 		EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = 		EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = 		ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 												//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource8);
    EXTI_InitStructure.EXTI_Line		=			EXTI_Line8;					//PB8
    EXTI_InitStructure.EXTI_Mode 		= 		EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = 		EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = 		ENABLE;
    EXTI_Init(&EXTI_InitStructure);	 												//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;										//ʹ���ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x02;					//��ռ���ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority 				= 0x04;					//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;												//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel =EXTI4_IRQn;										//ʹ���ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x02;					//��ռ���ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority 				= 0x05;					//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;												//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);

}

/**
�������ܣ����������Ƿ���Ч�����ش�������İ�����Чֵ��
���������������Ҫ���İ����ܽ�����ֵ
ע��:�������µ�ƽΪ��
**/

__INLINE static uint8_t Powerkey_Scan(uint8_t keyvalue)
{
    static u8 count_down = 1;
    static u8 Key_press_flag = 0;
    Key_press_flag = keyvalue & (keyvalue ^ count_down);
    count_down = keyvalue;
    return Key_press_flag ;
}


/******������⺯��*************************************************************************/
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
                Module_State->run = OFF;                        //�豸��ǰֹͣ����
                GREEN_LED_ON;
                if(struct_active_file.res_state != 0) //�����ǰ���ļ���״̬
                {
                    Write_Flag_ALL_Situation(flag_When_power_off,flag_update_time_yes,flag_flseek_yes,1);
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
                Module_State->power_state = OFF;

                GREEN_LED_OFF;
                printf( "run = %d power_state = %d\r\n",Module_State->run,Module_State->power_state);
                POWER_OFF;    //�ػ�
            }

        }
    }
}
/***
	��Դ������� PB.0 ��Դʹ�ܽ� PB.1��Դ�������� PB.2 USB�����ܽ�
***/

void PowerKEY_Control(module_state *Module_State)
{
    uint8_t PowerKey_Flag = 0;
    PowerKey_Flag = Powerkey_Scan(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4));
    if(PowerKey_Flag) {
        if( ON == Module_State->power_state ) {
            Module_State->run = OFF;                        //�豸��ǰֹͣ����
            GREEN_LED_ON;
            if(struct_active_file.res_state != 0) {  //�����ǰ���ļ���״̬
                f_close( &struct_active_file.file);
            }
            if( DirName.Dir_restate!= 0) {
                f_closedir(&DirName.DIR);
            }
            f_mount(NULL,"0:",1);            //ж���ļ�ϵͳ
            File_System.FATFS_state = 0;
            Module_State->power_state = OFF;

            GREEN_LED_OFF;
            printf( "run = %d power_state = %d\r\n",Module_State->run,Module_State->power_state);
            POWER_OFF;    //�ػ�
        } else {
            POWER_ON;
            if(Usb_State == !Connect) { //USB��û�в���
                Module_State->run = ON;
            }
            Module_State->power_state = ON;
            GREEN_LED_ON;
            printf( "run = %d power_state = %d\r\n",Module_State->run,Module_State->power_state);
        }
    }
}


///*
//	���ݰ��������豸�Ƿ�ʼ����������豸�Ѿ���ʼ������ֹͣ������
//*/

//void key_control(module_state *state)
//{
//	Power_Control(state);  			//��Դ�ػ�����    ����Ҫ��2s�ӣ��ػ����԰��¾Ϳ���
//  PauseKey_Control(state); 		//��ʼ��ͣ��
//}


/**										endif               **/
