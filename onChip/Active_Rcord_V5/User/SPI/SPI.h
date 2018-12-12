#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x.h"

#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(GPIOB, GPIO_Pin_12)



void SPI_FLASH_Init(void);
u8 SPI_ReadByte(void);
u16 SPI_SendHalfWord(u16 HalfWord);
u8 SPI_SendByte(u8 byte);

#endif





