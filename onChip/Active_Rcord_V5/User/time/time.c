#include "time.h"
#include "ADXL345.h"
#include "rtc.h"
#include "err_control.h"
#include "usb.h"
#include "usart1.h"
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

    //��ʱ��TIM3��ʼ��
    TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(TIM3,ENABLE);
}


void TIM3_IRQHandler(void)   //TIM3�ж�
{
	static u8 led_counter=0;
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
    {

        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־
			
			
        if( run_time_data.Temp_RunTime )
        {
            run_time_data.Temp_RunTime--;
					
            if(run_time_data.Temp_RunTime == 0)
            {
                Environment_data.Tempet_run_flag = ~0;
                run_time_data.Temp_RunTime = GetTempet_RunTime;
            }
        }
        if( run_time_data.voice_RunTime )
        {
            run_time_data.voice_RunTime--;
            if(run_time_data.voice_RunTime == 0)
            {
                Environment_data.Voice_run_flag = ~0;
                run_time_data.voice_RunTime = GetVoice_RunTime;
            }
        }
		
        if(run_time_data.ADXL345_RunTime)
        {
            run_time_data.ADXL345_RunTime--;
            if(run_time_data.ADXL345_RunTime == 0)
            {
                AD345_strcut_Data.run_flag = ~0;
                run_time_data.ADXL345_RunTime = ADXL345_RunTime;
            }
        }
        if(run_time_data.Light_RunTime)
        {
            run_time_data.Light_RunTime--;
            if(run_time_data.Light_RunTime == 0)
            {
                Environment_data.Light_run_flag = ~0;
                run_time_data.Light_RunTime = GetLight_RunTime;
            }
        }

        if( run_time_data.Power_RunTime ) {
            run_time_data.Power_RunTime--;
            if( run_time_data.Power_RunTime == 0 ) {
                Environment_data.Power_check_flag = ~0;
                run_time_data.Power_RunTime = POWER_CHECK_RunTime;
            }
        }
        if(module.machine_state == OK && module.run) {
            if( GreenLed_BlinkTime ) {
                GreenLed_BlinkTime --;
                if(GreenLed_BlinkTime == 0) 
								{
										
											if(led_counter<=20)
											{
												led_counter++;
												GREEN_LED_TOGGLE;
											}
											if(led_counter==21)GREEN_LED_OFF;
									
                    GreenLed_BlinkTime = GREEN_BLINK_TIME;
                }
            }
        }
        if( Usb_State == Connect)//USB��û�в���)
        {
            GREEN_LED_OFF; //�̵���
            if( chongdian_flag == chong_state ) {
                if( RedLed_BlinkTime ) {
                    RedLed_BlinkTime --;
                    if(RedLed_BlinkTime == 0) {
                        RED_LED_TOGGLE;
                        RedLed_BlinkTime = RED_BLINK_TIME;
                    }
                }
            } else if( chongdian_flag == chong_finsh ) {
                GREEN_LED_ON; //�̵���
                RED_LED_OFF; //�����
            }
        }
		
		if(  usart1.receive_count ){
			if( usart1.receive_count != usart1.pro_receiver_count ){
				usart1.pro_receiver_count = usart1.receive_count;
			}
			else{
				usart1.receive_over_time--;
				if( usart1.receive_over_time == 0 ){
					usart1.usart_rx_ok = ~0;
					usart1.pro_receiver_count = 0;
					usart1.receive_count = 0;
				}
			}
		}
    }
}

