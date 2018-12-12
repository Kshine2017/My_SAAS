
#ifndef __RTC_H
#define	__RTC_H


#include "stm32f10x.h"
#include "date.h"
#include "usart1.h"
#include "machine_control.h"


typedef struct create_flag{
	u8 Open_ActiveExcel_flag;  	//�򿪻excle�ļ�
  u8 Open_ActDerec_flag; 			//���ļ�Ŀ¼��־
  u8 Write_Data_flag;    			//��д���ݵ�����
}create_flag;

typedef struct run_control{     //����ģ������ʱ����Ƶ�Ԫ
	u8 Temp_RunTime;           
	u8 ADXL345_RunTime;
	u8 Light_RunTime;
  u8 voice_RunTime;
  u8 Power_RunTime;
}t_run_struct_data;


extern volatile create_flag  create_file_flag;  //�����ļ���־
extern volatile t_run_struct_data run_time_data;
extern __IO uint32_t TimeDisplay ;
extern __IO uint32_t Sec_Store_flag ;  //1s�������ݱ�־
extern volatile struct rtc_time systmtime; 
extern volatile struct rtc_time BeformSystmtime;
extern volatile struct rtc_time AfterSystmtime; //��ǰϵͳ��һ���ʱ��

void Time_Regulate( volatile struct rtc_time *tm);
void Time_Adjust( volatile struct rtc_time *tm);
static uint8_t USART_Scanf(uint32_t value);
unsigned int hour_sec( volatile struct rtc_time *time);


extern void RTC_NVIC_Config(void);
void RTC_CheckAndConfig(volatile struct rtc_time *tm);
extern void creat_file_control(void);
extern void my_sprinf(volatile struct rtc_time *time,char*str);

void refresh_system_time();
void auto_set_time(volatile struct rtc_time *time);
#endif /* __XXX_H */


