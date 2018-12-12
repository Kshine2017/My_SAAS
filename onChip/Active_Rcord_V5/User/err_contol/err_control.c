#include "err_control.h"
#include "usb.h"


u32 RedLed_BlinkTime = RED_BLINK_TIME;  //500ms   //����˸ʱ��
u32 GreenLed_BlinkTime = GREEN_BLINK_TIME;  //500ms
/*
**LED��GPIO�˿�����
**/
void LED_GPIO_init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
  
		GPIO_SetBits(GPIOB,  GPIO_Pin_2 | GPIO_Pin_3);  //LED�ƹر�
    
}


/*    ������ƺ�LED��ʾ����   
**    ������κ��쳣�������쳣�������Զ���������
*/
void err_control(module_state *state,void (*function)( unsigned char ))
{
	if( (state->flash_state || state->usb_state || state->active_state ||state->FATFS_state ||state->power_capacity) ){
		if( ERR == state->flash_state )
		{
			state->machine_state = ERR;
      goto err_handler;
    }
		if( ERR == state->usb_state ){
			state->machine_state = ERR;
      goto err_handler;
		}	
		if( ERR == state->active_state ){
			state->machine_state = ERR;
			goto err_handler;
		}		
		if( ERR == state->FATFS_state ){
			state->machine_state = ERR;
			goto err_handler;
		}
    switch(state->power_capacity){
      case  ERR :
			state->machine_state = ERR;   
        break;
      case WARING: 
        state->machine_state = WARING;	
        break;	
    }
	}else state->machine_state = OK;
    
  err_handler:	function(state->machine_state);  //led����ʾ���Ƴ���
}

/******led����ʾ����*********************/
void led_display(unsigned char state)
{ 
  if( Usb_State != Connect){
		switch(state){
			case ERR:	 			
				RED_LED_ON;				
				GREEN_LED_OFF;		
				break;
				
			case OK:
        
				RED_LED_OFF;
			break;
				
			case WARING:
				GREEN_LED_ON;	
				RED_LED_ON;
			break;
		}
  }
}
