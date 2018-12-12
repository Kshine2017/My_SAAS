#ifndef __I2C_1_H
#define __I2C_1_H

#include "stm32f10x.h"





extern void i2c_GPIO_init(void);
extern uint8_t Device_ReadBytes_len(uint8_t *_pReadBuf,u8 Address, uint16_t _usAddress, uint16_t _usSize);
extern uint8_t Device_ReadBytes(uint8_t *_pReadBuf, u8 Address,uint16_t _usAddress);
extern uint8_t Device_WriteBytes(uint8_t data,u8 Address, uint16_t _usAddress);
extern u8 i2c_CheckDevice(uint8_t _Address);
extern void i2c_stop(void);

#endif
