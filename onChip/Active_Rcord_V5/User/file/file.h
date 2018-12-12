#include "stdio.h"
#include "ff.h"
#include "machine_control.h"
#include "stm32f10x.h"
#include "string.h"
#include "rtc.h"
#include "systerm_time.h"
#include "ADXL345.h"
#ifndef __FILE_H
#define __FILE_H


#define DAY_SUM 10         //7天的数据

/*文件系统数据结构*/
typedef struct FS{
	 FATFS FS_Sysyem;  
	 volatile u8 FATFS_state;   //文件系统状态 0为已经挂载，非0为已经卸载
}filesystem;


typedef struct active{
	unsigned char res_state;	//文件当前是否处于打开正在读写
	char derectory[20];        //活动文件目录储存。
	FIL file;                 //文件对应文件结构体
}Active_File;


typedef struct DIR{
  DIR DIR;
  u8 Dir_restate;
}Dir_t;

extern filesystem File_System; 
extern Active_File struct_active_file;
extern UINT br,bw;
extern Dir_t DirName;        

extern void file_struct_init(module_state *state, filesystem* fs);
extern u8 FileSystem_ManegerProcess(filesystem *file_system,module_state* state);
extern u8 Flssk_file(volatile struct rtc_time* time,char *path,FILINFO *fno,Active_File *file);
extern FRESULT My_f_stat(volatile struct rtc_time* time,Active_File *file);
extern void File_Manage(volatile create_flag *flag);


extern u8 file_Lseek_flag;
void Write_Flag_for_Sleep(int flag,int position);

#define flag_When_readytosleep  	-2
#define flag_When_wake_up_now			-3
#define flag_When_power_on				-13
#define flag_When_power_off				-14
#define flag_When_usb_connect			-15
#define flag_When_usb_disconnect 	-16
#define flag_update_time_yes			1
#define flag_update_time_no				0
#define flag_flseek_yes						1
#define flag_flseek_no						0

void Write_Flag_ALL_Situation(int flag_When, int flag_update_time,int flag_flseek,int position);


#endif
