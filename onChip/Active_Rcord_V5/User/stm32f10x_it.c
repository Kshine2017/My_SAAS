/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include "rtc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_iwdg.h"
#include "systerm_time.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
    printf("NMI_Handler 报错\r\n");
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
        printf("Hard Fault_Handler 报错\r\n");
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
        printf("MemManage_Handler 报错\r\n");
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
        printf("BusFault_Handler 报错\r\n");
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
        printf("UsageFault_Handler 报错\r\n");
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
    printf("SVC_Handler 报错\r\n");
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
    printf("DebugMon_Handler 报错\r\n");
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
    printf("PendSV_Handler 报错\r\n");
}



/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */


/*
 * 函数名：USB_LP_CAN1_RX0_IRQHandler
 * 描述  ：USB低优先级中断请求
 * 输入  ：无
 * 输出  ：无
 */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
    USB_Istr();
}

/*
 * 函数名：USB_HP_CAN1_TX_IRQHandler
 * 描述  ：USB高优先级中断请求 或发送中断
 * 输入  ：无
 * 输出  ：无
 */
void USB_HP_CAN1_TX_IRQHandler(void)
{
    CTR_HP();
}


/**
  * @brief  This function handles RTC interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
    {
        /* Clear the RTC Second interrupt */
        RTC_ClearITPendingBit(RTC_IT_SEC);
//    printf("当前时间 = %d:%d:%d:%d:%d:%d\r\n",systmtime.tm_year,systmtime.tm_mon,systmtime.tm_mday,systmtime.tm_hour,systmtime.tm_min,systmtime.tm_sec);
        /* Enable time update */
        TimeDisplay = 1;
        Sec_Store_flag= ~0;
        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();
    }
}


/* 串口1接收中断 */
/* 串口1接收中断 */
void USART1_IRQHandler(void)
{

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
    { 
		usart1_buf[usart1.receive_count++]=USART_ReceiveData(USART1);
    }
	if( usart1.receive_count >= 50 )
	{
		usart1.receive_count = 0;
	}
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);		//清除串口接受完成标志
}


void EXTI4_IRQHandler()   //PB4  check
{
		if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
    {
				printf("\n\n EXTI4_IRQHandler 外部中断 \n\n");
				stop_flag = 0;
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
}

void EXTI9_5_IRQHandler()   //PB4  check
{
		if(EXTI_GetITStatus(EXTI_Line9)!=RESET)
    {
				printf("\n\n EXTI9_5_IRQHandler 外部中断 EXTI_Line9 \n\n");
				stop_flag = 0;
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
		
		if(EXTI_GetITStatus(EXTI_Line8)!=RESET)
    {
				printf("\n\n EXTI9_5_IRQHandler 外部中断 EXTI_Line8 \n\n");
				stop_flag = 0;
        EXTI_ClearITPendingBit(EXTI_Line8);
    }

}



/*
void PWR_EnterSleepMode(uint8_t PWR_SLEEPEntry); //睡眠模式
void PWR_EnterSTOPMode(uint32_t PWR_Regulator, uint8_t PWR_STOPEntry);  //停机模式
void PWR_EnterSTANDBYMode(void);    //待机模式

#define PWR_STOPEntry_WFI              //中断唤醒
#define PWR_STOPEntry_WFE              //事件唤醒
*/

void Standby()
{

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); //开电源管理时钟PWR_Regulator_LowPower
    PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI|PWR_STOPEntry_WFE);//进入停机
    //PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);

}


//adxl345 的INT1(PB10) 和INT2(PB11) 上升沿触发
//INT1表示进入休眠
//INT2表示开始工作
u8 stop_flag=0;
void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line10)!=RESET)
    {
			//	printf("\n\n没有活动，准备进入休眠！！！\n");
        //Standby();
				stop_flag = 1;
        EXTI_ClearITPendingBit(EXTI_Line10);
    }
		if(EXTI_GetITStatus(EXTI_Line11)!=RESET)
    {
			//	printf("\n\n\n============检测到活动==============\n\n\n");
				//SystemInit();
				//stop_flag = 0;不用写
        EXTI_ClearITPendingBit(EXTI_Line11);
    }

//		printf("EXTI15_10_IRQHandler starts now!\n");
//    if(EXTI_GetITStatus(EXTI_Line10)!=RESET  &&  EXTI_GetITStatus(EXTI_Line11) ==RESET)//判断某个线上的中断是否发生
//    {
//				printf("10  ===  正常标志！\n\n");
//				//EXTI_ClearITPendingBit(EXTI_Line10);
////				RTC_ClearITPendingBit(RTC_IT_SEC);RTC_WaitForLastTask();
////				USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//        //delay_ms(500);
//        printf("\n\n没有活动，进入休眠！！！\n\n");
//        Standby();
//				SystemInit();
//				EXTI_ClearITPendingBit(EXTI_Line10);
//				EXTI_ClearITPendingBit(EXTI_Line11);
//    }else if(EXTI_GetITStatus(EXTI_Line10)==RESET  &&  EXTI_GetITStatus(EXTI_Line11)!=RESET)//判断某个线上的中断是否发生
//    {
//				printf("01  ===  正常标志！\n\n");
//        EXTI_ClearITPendingBit(EXTI_Line11);   //清除 LINE 上的中断标志位
//				//EXTI_ClearITPendingBit(EXTI_Line10);
//        printf("\n\n检测到活动，开始工作！！！\n\n");
//        //IWDG_Enable(); //还有个电源errr管理被注释掉了
//    }else if(EXTI_GetITStatus(EXTI_Line10)!=RESET && EXTI_GetITStatus(EXTI_Line11) != RESET)
//    {
//        printf("11  ===  标志异常，正在恢复！\n\n");
//        EXTI_ClearITPendingBit(EXTI_Line10);   //清除 LINE 上的中断标志位
//        EXTI_ClearITPendingBit(EXTI_Line11);   //清除 LINE 上的中断标志位
//    }else if(EXTI_GetITStatus(EXTI_Line10)==RESET && EXTI_GetITStatus(EXTI_Line11) == RESET)
//    {
//        printf("00  ===  异常进入！（唤醒后可能出现的情况）\n\n");
//    }
//		printf("EXTI15_10_IRQHandler is over!\n");

}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
