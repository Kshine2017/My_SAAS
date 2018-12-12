#include "Flash.h"
#include "systerm_time.h"


static void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_SectorErase(u32 SectorAddr);
const u8 Device_ID_Name = 0x18;    			//器件ID
const u32 Produce_ID_Name = 0x4019;    	//Flash厂家ID

/*读取flash的ID一个字节*/
static u32 SPI_FLASH_ReadDeviceID(void)
{
  u32 Temp = 0;
  SPI_FLASH_CS_LOW();
	
  SPI_SendByte(W25X_DeviceID);
  SPI_SendByte(0XFF); //随意数值
  SPI_SendByte(0XFF);
  SPI_SendByte(0XFF);

	
  Temp = SPI_SendByte(0xff);
  SPI_FLASH_CS_HIGH();
  return Temp;
}

/*读取厂家ID  是三个字节*/
static u32 SPI_FLASH_ReadID(void)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
  SPI_FLASH_CS_LOW();
  SPI_SendByte(W25X_JedecDeviceID);
  Temp0 = SPI_SendByte(0xff);
  Temp1 = SPI_SendByte(0xff);
  Temp2 = SPI_SendByte(0xff);
  SPI_FLASH_CS_HIGH();
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
  return Temp;
}

//进入掉电模式
void SPI_Flash_PowerDown(void)   
{ 
  SPI_FLASH_CS_LOW();
  SPI_SendByte(W25X_PowerDown);
  SPI_FLASH_CS_HIGH();
} 

//唤醒
void SPI_Flash_WAKEUP(void)   
{
  SPI_FLASH_CS_LOW();
  SPI_SendByte(W25X_ReleasePowerDown);
  SPI_FLASH_CS_HIGH();                   //等待TRES1
}   


/*读取状态寄存器  等待FLASH不忙*/
void SPI_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;
  SPI_FLASH_CS_LOW();
  SPI_SendByte(W25X_ReadStatusReg);
  do
  {
    FLASH_Status = SPI_SendByte(0xff);	 
  }
  while ((FLASH_Status & WIP_Flag) == SET);  /*WIP 是 flash正在忙标志 */
  SPI_FLASH_CS_HIGH();
}


/*使能写*/
void SPI_FLASH_WriteEnable(void)
{
  SPI_FLASH_CS_LOW();
  SPI_SendByte(W25X_WriteEnable);
  SPI_FLASH_CS_HIGH();
}

/*读命令，后面是24位地址*/
void SPI_FLASH_StartReadSequence(u32 ReadAddr)
{
  SPI_FLASH_CS_LOW();
  SPI_SendByte(W25X_ReadData);
  SPI_SendByte((ReadAddr & 0xFF0000) >> 16);
  SPI_SendByte((ReadAddr& 0xFF00) >> 8);
  SPI_SendByte(ReadAddr & 0xFF);
}

/*******************************************************************************
 *** 擦除整个flash
*******************************************************************************/
void SPI_FLASH_BulkErase(void)
{
  SPI_FLASH_WriteEnable();
  SPI_FLASH_CS_LOW();
  SPI_SendByte(W25X_ChipErase);
  SPI_FLASH_CS_HIGH();
  SPI_FLASH_WaitForWriteEnd();
}

/*******************************************************************************
* 对一个扇区进行擦除
* 参数为扇区地址
*******************************************************************************/
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
	
  SPI_FLASH_WriteEnable();
  SPI_FLASH_WaitForWriteEnd();
  SPI_FLASH_CS_LOW();
  SPI_SendByte(W25X_SectorErase);
  SPI_SendByte((SectorAddr & 0xFF0000) >> 16);
  SPI_SendByte((SectorAddr & 0xFF00) >> 8);
  SPI_SendByte(SectorAddr & 0xFF);
  SPI_FLASH_CS_HIGH();
  SPI_FLASH_WaitForWriteEnd();
}

/*******************************************************************************
* Flash页写
*******************************************************************************/
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  SPI_FLASH_WriteEnable();
  SPI_FLASH_CS_LOW();
  SPI_SendByte(W25X_PageProgram);  //页写
	SPI_SendByte((WriteAddr & 0xFF0000) >> 16);
  SPI_SendByte((WriteAddr & 0xFF00) >> 8);
  SPI_SendByte(WriteAddr & 0xFF);
  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
  }
  while (NumByteToWrite--)
  {
    SPI_SendByte(*pBuffer);
    pBuffer++;
  }
  SPI_FLASH_CS_HIGH();
  SPI_FLASH_WaitForWriteEnd();  //等待写入结束
}
//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void SPI_FLASH_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	  
		SPI_FLASH_PageWrite(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};	    
}

/*从指定位置读取Num个数据储存到pBuffer*/
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
  SPI_FLASH_CS_LOW();
  SPI_SendByte(W25X_ReadData);
  SPI_SendByte((ReadAddr & 0xFF0000) >> 16);
  SPI_SendByte((ReadAddr& 0xFF00) >> 8);
  SPI_SendByte(ReadAddr & 0xFF);

  while (NumByteToRead--) 
  {
    *pBuffer = SPI_SendByte(0xff);
    pBuffer++;
  }
  SPI_FLASH_CS_HIGH();
}

u8 W25QXX_BUFFER[SPI_FLASH_SECTORSize];
/*flah从任意地址开始写入数据到flash*/
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25QXX_BUF;	  
  W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/SPI_FLASH_SECTORSize;  //扇区地址  
	secoff=WriteAddr%SPI_FLASH_SECTORSize;	//在扇区内的偏移
	secremain=SPI_FLASH_SECTORSize-secoff;	//扇区剩余空间大小   
 	if(NumByteToWrite<=secremain){
		secremain=NumByteToWrite;//不大于SPI_FLASH_PageSize个字节
	}
	while(1) 
	{	
		SPI_FLASH_BufferRead(W25QXX_BUF,secpos*SPI_FLASH_SECTORSize,SPI_FLASH_SECTORSize);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			SPI_FLASH_SectorErase(secpos*SPI_FLASH_SECTORSize);		//擦除这个扇区
			for(i=0;i<secremain;i++)	   		//复制
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	   //将需要写入的数据放入这个扇区
			}
			SPI_FLASH_Write_NoCheck(W25QXX_BUF,secpos*SPI_FLASH_SECTORSize,SPI_FLASH_SECTORSize);//写入整个扇区  

		}else SPI_FLASH_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 		
		
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		  pBuffer+=secremain;  				//指针偏移
			WriteAddr+=secremain;				//写地址偏移	   
		  NumByteToWrite-=secremain;			//字节数递减
			if(NumByteToWrite>SPI_FLASH_SECTORSize)secremain=SPI_FLASH_SECTORSize;//下一个扇区还是写不完
			else secremain=NumByteToWrite;		//下一个扇区可以写完了
		}	 
	};	 
}


//解锁 写保护
//根据手册，需要将 S7和S8至0
void W25Q256_Unlock_WP()
{
	//第一步，必须预先执行标准写使能（06H）指令,以使设备接受写状态寄存器指令（状态寄存器位WEL必须等于1）
	SPI_FLASH_WriteEnable();
	
	//第二步，驱动/CS低输入
	SPI_FLASH_CS_LOW();
	
	//第三步，发送指令代码“01H/3H/11H”
	SPI_SendByte(W25X_WriteStatusReg); //状态寄存器1
	
	//第四步，然后写入状态寄存器数据字节
	SPI_SendByte(0x00);
	
	
	//第五步，发送指令代码“01H/3H/11H”
	SPI_SendByte(W25X_WriteStatusReg2); //状态寄存器1
	
	//第六步，然后写入状态寄存器数据字节
	SPI_SendByte(0x00);

	//第七步，发送指令代码“01H/3H/11H”
	SPI_SendByte(W25X_WriteStatusReg3); //状态寄存器1
	
	//第八步，然后写入状态寄存器数据字节
	SPI_SendByte(0x00);
	//
	SPI_FLASH_CS_HIGH();
	//
	//SPI_FLASH_WriteDisable();
	
}


/* Flash初始化 同时检测Flash是否正常
 * 返回值：0：表示成功   1：表示失败
 */
u8 Flash_Init(void) 
{ 
	u8 Device_Id;
	u32 Produce_Id;
	SPI_FLASH_Init();
	//SPI_FLASH_BulkErase();
	//W25Q256_Unlock_WP();
	Device_Id = SPI_FLASH_ReadDeviceID();
	Produce_Id = SPI_FLASH_ReadID();		
	printf("Produce_Id = %x\r\n",Produce_Id);
	if(Device_Id != Device_ID_Name){	
		return 1;
	}
	return 0;
}


