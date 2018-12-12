#include "Flash.h"
#include "systerm_time.h"


static void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_SectorErase(u32 SectorAddr);
const u8 Device_ID_Name = 0x18;    			//����ID
const u32 Produce_ID_Name = 0x4019;    	//Flash����ID

/*��ȡflash��IDһ���ֽ�*/
static u32 SPI_FLASH_ReadDeviceID(void)
{
  u32 Temp = 0;
  SPI_FLASH_CS_LOW();
	
  SPI_SendByte(W25X_DeviceID);
  SPI_SendByte(0XFF); //������ֵ
  SPI_SendByte(0XFF);
  SPI_SendByte(0XFF);

	
  Temp = SPI_SendByte(0xff);
  SPI_FLASH_CS_HIGH();
  return Temp;
}

/*��ȡ����ID  �������ֽ�*/
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

//�������ģʽ
void SPI_Flash_PowerDown(void)   
{ 
  SPI_FLASH_CS_LOW();
  SPI_SendByte(W25X_PowerDown);
  SPI_FLASH_CS_HIGH();
} 

//����
void SPI_Flash_WAKEUP(void)   
{
  SPI_FLASH_CS_LOW();
  SPI_SendByte(W25X_ReleasePowerDown);
  SPI_FLASH_CS_HIGH();                   //�ȴ�TRES1
}   


/*��ȡ״̬�Ĵ���  �ȴ�FLASH��æ*/
void SPI_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;
  SPI_FLASH_CS_LOW();
  SPI_SendByte(W25X_ReadStatusReg);
  do
  {
    FLASH_Status = SPI_SendByte(0xff);	 
  }
  while ((FLASH_Status & WIP_Flag) == SET);  /*WIP �� flash����æ��־ */
  SPI_FLASH_CS_HIGH();
}


/*ʹ��д*/
void SPI_FLASH_WriteEnable(void)
{
  SPI_FLASH_CS_LOW();
  SPI_SendByte(W25X_WriteEnable);
  SPI_FLASH_CS_HIGH();
}

/*�����������24λ��ַ*/
void SPI_FLASH_StartReadSequence(u32 ReadAddr)
{
  SPI_FLASH_CS_LOW();
  SPI_SendByte(W25X_ReadData);
  SPI_SendByte((ReadAddr & 0xFF0000) >> 16);
  SPI_SendByte((ReadAddr& 0xFF00) >> 8);
  SPI_SendByte(ReadAddr & 0xFF);
}

/*******************************************************************************
 *** ��������flash
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
* ��һ���������в���
* ����Ϊ������ַ
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
* Flashҳд
*******************************************************************************/
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  SPI_FLASH_WriteEnable();
  SPI_FLASH_CS_LOW();
  SPI_SendByte(W25X_PageProgram);  //ҳд
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
  SPI_FLASH_WaitForWriteEnd();  //�ȴ�д�����
}
//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void SPI_FLASH_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	  
		SPI_FLASH_PageWrite(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
}

/*��ָ��λ�ö�ȡNum�����ݴ��浽pBuffer*/
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
/*flah�������ַ��ʼд�����ݵ�flash*/
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25QXX_BUF;	  
  W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/SPI_FLASH_SECTORSize;  //������ַ  
	secoff=WriteAddr%SPI_FLASH_SECTORSize;	//�������ڵ�ƫ��
	secremain=SPI_FLASH_SECTORSize-secoff;	//����ʣ��ռ��С   
 	if(NumByteToWrite<=secremain){
		secremain=NumByteToWrite;//������SPI_FLASH_PageSize���ֽ�
	}
	while(1) 
	{	
		SPI_FLASH_BufferRead(W25QXX_BUF,secpos*SPI_FLASH_SECTORSize,SPI_FLASH_SECTORSize);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			SPI_FLASH_SectorErase(secpos*SPI_FLASH_SECTORSize);		//�����������
			for(i=0;i<secremain;i++)	   		//����
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	   //����Ҫд������ݷ����������
			}
			SPI_FLASH_Write_NoCheck(W25QXX_BUF,secpos*SPI_FLASH_SECTORSize,SPI_FLASH_SECTORSize);//д����������  

		}else SPI_FLASH_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 		
		
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		  pBuffer+=secremain;  				//ָ��ƫ��
			WriteAddr+=secremain;				//д��ַƫ��	   
		  NumByteToWrite-=secremain;			//�ֽ����ݼ�
			if(NumByteToWrite>SPI_FLASH_SECTORSize)secremain=SPI_FLASH_SECTORSize;//��һ����������д����
			else secremain=NumByteToWrite;		//��һ����������д����
		}	 
	};	 
}


//���� д����
//�����ֲᣬ��Ҫ�� S7��S8��0
void W25Q256_Unlock_WP()
{
	//��һ��������Ԥ��ִ�б�׼дʹ�ܣ�06H��ָ��,��ʹ�豸����д״̬�Ĵ���ָ�״̬�Ĵ���λWEL�������1��
	SPI_FLASH_WriteEnable();
	
	//�ڶ���������/CS������
	SPI_FLASH_CS_LOW();
	
	//������������ָ����롰01H/3H/11H��
	SPI_SendByte(W25X_WriteStatusReg); //״̬�Ĵ���1
	
	//���Ĳ���Ȼ��д��״̬�Ĵ��������ֽ�
	SPI_SendByte(0x00);
	
	
	//���岽������ָ����롰01H/3H/11H��
	SPI_SendByte(W25X_WriteStatusReg2); //״̬�Ĵ���1
	
	//��������Ȼ��д��״̬�Ĵ��������ֽ�
	SPI_SendByte(0x00);

	//���߲�������ָ����롰01H/3H/11H��
	SPI_SendByte(W25X_WriteStatusReg3); //״̬�Ĵ���1
	
	//�ڰ˲���Ȼ��д��״̬�Ĵ��������ֽ�
	SPI_SendByte(0x00);
	//
	SPI_FLASH_CS_HIGH();
	//
	//SPI_FLASH_WriteDisable();
	
}


/* Flash��ʼ�� ͬʱ���Flash�Ƿ�����
 * ����ֵ��0����ʾ�ɹ�   1����ʾʧ��
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


