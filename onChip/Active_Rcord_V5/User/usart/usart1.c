/**
  * @brief   重现c库printf函数到usart端口
  ******************************************************************************
  */

#include "usart1.h"


/********串口1参数***************************************/
usart1_t usart1 = {
	.send_over_flag = 0,
	.usart_rx_ok = 0,
	.receive_count = 0,
	.receive_over_time = RECEIVE_OVER_TIME,
	.pro_receiver_count = 0
};
/*****串口接收buf*******************************************************/
u8 usart1_buf[50] = { 0 };
/**
 * @brief  USART1 GPIO 配置,工作模式配置。115200 8-N-1
 * @param  无
 * @retval 无
 */
void USART1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* config USART1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    /* USART1 GPIO config */
    /* Configure USART1 Tx (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* USART1 mode config */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //接收中断
		USART_Cmd(USART1, ENABLE);		 //开启串口1
}



/*****************************************************/
u16 Calibration_data( u8 *buf)
{
	u8 length = 0;
	int i = 0;
	u32 calibrationData = 0;
	length = (((uint16_t)buf[7])<<8) | buf[8];
	for( i = 0; i < 7+length ;i++){
		calibrationData += buf[i];
	}
	return (u16)calibrationData;
}
/********串口1接收处理函数****************************/
void usart1_dealHandle( u8 *buf){
	u8 length = 0; 
	
	if( usart1.usart_rx_ok){
		printf("test");
		if( buf[0] == 0XA5 && buf[1] == 0x10 && buf[2] == 0xA5 && buf[3] == 0x10 ){
			length = (((uint16_t)buf[6])<<8) | buf[7];
			if( buf[ 10 + length] == 0x10 && buf[ 11 + length] == 0xAB && buf[ 12 + length] == 0x10 && buf[ 13 + length] == 0xAB){
				if( (u8)(Calibration_data( buf) >> 8 & 0xff) == buf[ 8+length] && (u8)(Calibration_data( buf) & 0xff) == buf[ 9+length]){
					if(buf[4] == 0x00 && buf[5] == 0x01){
						printf("%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x",0xA5,0x10,0xA5,0x10,0x40,0x01,0x00,0x00,0x01,0xAB,0x10,0xAB,0x10,0xAB);
					}
						
				}
			}
		}
		usart1.usart_rx_ok = 0;
    }
}

///重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
    /* 发送一个字节数据到USART1 */
    USART_SendData(USART1, (uint8_t) ch);

    /* 等待发送完毕 */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

    return (ch);
}




/*********************************************END OF FILE**********************/
