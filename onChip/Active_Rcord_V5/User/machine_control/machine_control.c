#include "machine_control.h"

/**
 状态初始化函数
**/
 module_state module; //实体化一个组件状态

void machine_state_init(module_state * state)
{
	state->machine_state = OK;
	state->active_state = OK;
	state->FATFS_state = OK;
	state->light_state = OK;
	state->power_state = OFF;
	state->temp_state = OK;
	state->usb_state = OK;
	state->power_capacity = OK;
	state ->run = OFF;
	state->flash_state = OK;
	return;
}








/******************endif*********************************/
