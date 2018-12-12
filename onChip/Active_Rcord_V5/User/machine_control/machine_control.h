#ifndef __MACHINE_CONTROL_H
#define __MACHINE_CONTROL_H


/****状态枚举***/
enum state{
  OK = 0,				/* (0) Succeeded */
	WARING,					/* (1) A hard error */
	ERR,			/* (2) 正常但有警告 */
};

/*     机器电源是否开机       */
enum power_state{
	OFF,        /*    关机状态     */      
	ON,         /*      上电状态   */
};


/*各模块的状态*/
typedef struct module_state{
	volatile unsigned char machine_state; //设备状态
	volatile unsigned char power_state;   //电源状态
	volatile unsigned char flash_state;   //flash卡状态
	volatile unsigned char usb_state;     //usb_device模块状态
	volatile unsigned char active_state;  //加速度器模块状态
	volatile unsigned char light_state;   //光线模块状态
	volatile unsigned char temp_state;    //温度模块状态
	volatile unsigned char FATFS_state;   //文件系统状态
	volatile unsigned char power_capacity; //电源容量
	volatile unsigned char run;            //设备是否运行
}module_state;

extern  module_state module; //实体化一个组件状态; //实体化一个组件状态
extern void machine_state_init(module_state* state);





#endif
