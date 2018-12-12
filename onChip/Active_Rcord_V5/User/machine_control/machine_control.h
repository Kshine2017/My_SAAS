#ifndef __MACHINE_CONTROL_H
#define __MACHINE_CONTROL_H


/****״̬ö��***/
enum state{
  OK = 0,				/* (0) Succeeded */
	WARING,					/* (1) A hard error */
	ERR,			/* (2) �������о��� */
};

/*     ������Դ�Ƿ񿪻�       */
enum power_state{
	OFF,        /*    �ػ�״̬     */      
	ON,         /*      �ϵ�״̬   */
};


/*��ģ���״̬*/
typedef struct module_state{
	volatile unsigned char machine_state; //�豸״̬
	volatile unsigned char power_state;   //��Դ״̬
	volatile unsigned char flash_state;   //flash��״̬
	volatile unsigned char usb_state;     //usb_deviceģ��״̬
	volatile unsigned char active_state;  //���ٶ���ģ��״̬
	volatile unsigned char light_state;   //����ģ��״̬
	volatile unsigned char temp_state;    //�¶�ģ��״̬
	volatile unsigned char FATFS_state;   //�ļ�ϵͳ״̬
	volatile unsigned char power_capacity; //��Դ����
	volatile unsigned char run;            //�豸�Ƿ�����
}module_state;

extern  module_state module; //ʵ�廯һ�����״̬; //ʵ�廯һ�����״̬
extern void machine_state_init(module_state* state);





#endif
