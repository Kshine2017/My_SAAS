#ifndef __I2C_2_H
#define __I2C_2_H

#include "stm32f10x.h"





extern void i2c_GPIO_init_2(void);
extern uint8_t Device_ReadBytes_2(uint16_t *_pReadBuf, u8 Address);
extern uint8_t Device_WriteBytes_2(uint8_t command,u8 Address);
extern u8 		  i2c_CheckDevice_2(uint8_t _Address);
extern void i2c_stop_2(void);



void bh_data_send(uint8_t command);
uint16_t bh_data_read(void);
#endif
