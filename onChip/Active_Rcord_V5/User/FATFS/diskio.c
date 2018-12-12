/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "stm32f10x.h"
#include "rtc.h"
#include "Flash.h"

//#define BLOCK_SIZE           4096 *8 /* Block Size in Bytes */
#define SPI_FLASH 0    //flash�̷�


#define FLASH_SECTOR_SIZE 	4096	 //������С4096
#define FLASH_BLOCK_COUNT 512   //������
//����W25Q128
//ǰ12M�ֽڸ�fatfs��,12M�ֽں�,���ڴ���ֿ�,�ֿ�ռ��3.09M.	ʣ�ಿ��,���ͻ��Լ���	 			    
u32	 FLASH_SECTOR_COUNT= (16 * FLASH_BLOCK_COUNT );	
#define FLASH_BLOCK_SIZE   	512     	//ÿ��BLOCK��8������
/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
	switch( drv ){	/* Supports only single drive */
    case FS_SPI_FLASH:
      Flash_Init();
      return RES_OK;
  }
  return STA_NOINIT;

}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
  DSTATUS stat;
  switch( drv ){
    case FS_SPI_FLASH:
      stat = RES_OK;;
      return stat;
  }
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
){
  switch (drv){
    case FS_SPI_FLASH:
      if (!count)return RES_PARERR;//count���ܵ���0�����򷵻ز�������
    
      for(;count>0;count--)
      {
        SPI_FLASH_BufferRead((u8*)buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
        sector++;
        buff+=FLASH_SECTOR_SIZE;
      }
      return RES_OK;
  }
  return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
  switch (drv){
    case FS_SPI_FLASH:
      if (!count)return RES_PARERR;//count���ܵ���0�����򷵻ز�������
      for(;count>0;count--)
      {										    
        SPI_FLASH_BufferWrite((u8*)buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
        sector++;
        buff+=FLASH_SECTOR_SIZE;
      }
     return RES_OK;
  }
  return STA_NOINIT;
}

#endif /* _READONLY */


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{  
 DRESULT res;
 if(drv== 0)	//�ⲿFLASH  
	{
	    switch(cmd)
	    {
		    case CTRL_SYNC:
				res = RES_OK; 
		        break;	 
		    case GET_SECTOR_SIZE:
		        *(WORD*)buff = FLASH_SECTOR_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
            *(DWORD*)buff = FLASH_SECTOR_SIZE * 16;          
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = FLASH_SECTOR_COUNT;
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }
      return res;
	}else res=RES_ERROR;//�����Ĳ�֧��
}
							 
/*-----------------------------------------------------------------------*/
/* Get current time                                                      */
/*-----------------------------------------------------------------------*/ 

//���ʱ��
//User defined function to give a current time to fatfs module      */
//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */  
DWORD get_fattime(void)
{
	DWORD time = 0;
	time |= (systmtime.tm_year-1980)<<25 | systmtime.tm_mon<<21 |  systmtime.tm_mday <<16 | systmtime.tm_hour << 11 | systmtime.tm_min << 5 | systmtime.tm_sec;
 return time;

} 
