
#include "STM32F10x.h"
#include "SPI.h"
#include "machine_control.h"

#ifndef __FLASH_H
#define __FLASH_H

//#define SPI_FLASH_SPI                           SPI1
//#define SPI_FLASH_SPI_CLK                       RCC_APB2Periph_SPI1
//#define SPI_FLASH_SPI_SCK_PIN                   GPIO_Pin_5                  /* PA.05 */
//#define SPI_FLASH_SPI_SCK_GPIO_PORT             GPIOA                       /* GPIOA */
//#define SPI_FLASH_SPI_SCK_GPIO_CLK              RCC_APB2Periph_GPIOA
//#define SPI_FLASH_SPI_MISO_PIN                  GPIO_Pin_6                  /* PA.06 */
//#define SPI_FLASH_SPI_MISO_GPIO_PORT            GPIOA                       /* GPIOA */
//#define SPI_FLASH_SPI_MISO_GPIO_CLK             RCC_APB2Periph_GPIOA
//#define SPI_FLASH_SPI_MOSI_PIN                  GPIO_Pin_7                  /* PA.07 */
//#define SPI_FLASH_SPI_MOSI_GPIO_PORT            GPIOA                       /* GPIOA */
//#define SPI_FLASH_SPI_MOSI_GPIO_CLK             RCC_APB2Periph_GPIOA
//#define SPI_FLASH_CS_PIN                        GPIO_Pin_4                  /* PA.04 */
//#define SPI_FLASH_CS_GPIO_PORT                  GPIOA                       /* GPIOA */
//#define SPI_FLASH_CS_GPIO_CLK                   RCC_APB2Periph_GPIOA



/* Private define ------------------------------------------------------------*/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 


#define W25X_ReadStatusReg		    0x05 			//S7-S0
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadStatusReg2		    0x35 			//S15-S8
#define W25X_WriteStatusReg2		  0x31 
#define W25X_ReadStatusReg3		    0x15 			//S23-S16
#define W25X_WriteStatusReg3		  0x11 


#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F 



#define WIP_Flag  0x01  /* flash正在忙标志 */
/*扇区大小定义*/
#define SPI_FLASH_SECTORSize 4096     //扇区大小
/*页大小定义*/
#define SPI_FLASH_PageSize      				256
#define SPI_FLASH_PerWritePageSize      256

#define FS_SPI_FLASH 0  //SPI Flash储存设备号

u8 Flash_Init(void);
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
#endif




