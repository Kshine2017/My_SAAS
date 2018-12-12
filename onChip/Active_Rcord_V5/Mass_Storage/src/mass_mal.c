/**
  ******************************************************************************
  * @file    mass_mal.c
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    29-June-2012
  * @brief   Medium Access Layer interface
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "FLASH.h"
#include "mass_mal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
long long Mass_Memory_Size[MAX_LUN+1];
u32 Mass_Block_Size[MAX_LUN+1];
u32 Mass_Block_Count[MAX_LUN+1];
__IO uint32_t Status = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : MAL_Init
* Description    : Initializes the Media on the STM32
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_Init(uint8_t lun)
{
  uint16_t res;
  switch (lun)
  {
    case FS_SPI_FLASH:
			Flash_Init();
			res =  MAL_OK;
		//printf("**********************FLASH初始化成功\r\n");
			break;
    
		default:
			res = MAL_FAIL;
  }
  return res;
}
/*******************************************************************************
* Function Name  : MAL_Write
* Description    : Write sectors
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_Write(uint8_t lun, uint32_t Memory_Offset, uint32_t *Writebuff, uint16_t Transfer_Length)
{
  uint16_t res;
  switch (lun)
  {
    case 0:	
			SPI_FLASH_BufferWrite((u8* )Writebuff, Memory_Offset, Transfer_Length);
			res =	MAL_OK ;    
      break;
		default:
      res =  MAL_FAIL;
  }
  return res;
}

/*******************************************************************************
* Function Name  : MAL_Read
* Description    : Read sectors
* Input          : None
* Output         : None
* Return         : Buffer pointer
*******************************************************************************/
uint16_t MAL_Read(uint8_t lun, uint32_t Memory_Offset, uint32_t *Readbuff, uint16_t Transfer_Length)
{
  uint16_t res;
  switch (lun)
  {
    case 0:
			SPI_FLASH_BufferRead((u8*) Readbuff,  Memory_Offset , Transfer_Length);
			res = MAL_OK;
      break;
    case 1:break;
		default:
      res = MAL_FAIL;
  }
  return res;
}

/*******************************************************************************
* Function Name  : MAL_GetStatus
* Description    : Get status
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_GetStatus (uint8_t lun)
{
  uint16_t res;
//	uint32_t DeviceSizeMul = 0, NumberOfBlocks = 0;
  switch (lun)
  {
		case 0: 
			Mass_Memory_Size[0] = 4096 * 16 * 512 ;
			Mass_Block_Size[0]  = 4096 ;
			Mass_Block_Count[0] = Mass_Memory_Size[0] / Mass_Block_Size[0];
      res=  MAL_OK;
    break;
    case 1:break;
		default:res =  MAL_FAIL;
  }
  return res;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

