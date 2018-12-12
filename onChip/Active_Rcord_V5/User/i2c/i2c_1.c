#include "i2c_1.h"
#include "stm32f10x_exti.h"
//This is for ADXL345

#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */

#define MPU_ADRESS	(0X68<<1)   //MPU��ַ

#define GPIO_PORT_I2C	GPIOB			/* GPIO�˿� */
#define I2C_SCL_PIN		GPIO_Pin_6			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define I2C_SDA_PIN		GPIO_Pin_7			/* ���ӵ�SDA�����ߵ�GPIO */

#define I2C_SCL_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 1 */
#define I2C_SCL_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 0 */
	
#define I2C_SDA_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 1 */
#define I2C_SDA_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 0 */
#define I2C_SDA_READ()  GPIO_ReadInputDataBit(GPIO_PORT_I2C, I2C_SDA_PIN)	/* ��SDA����״̬ */

/***mpu i2cͨѶ�ӳٺ���**/
//		CPU��Ƶ72MHzʱ�����ڲ�Flash����, MDK���̲��Ż�
//		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
//		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
//	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
static void i2c_Delay (u8 us) 
{
	u8 i;
	for(i = us;i>0;i--){
	}
}



/**i2c GPIO �ܽ�����  ����I2C��ģ���**/
void i2c_GPIO_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);			//ʹ�ܸ��ù���ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);						 		//PB6��PB7����
	
	
	//INT1��INT2 �����ش����жϣ�����EXTI15_10_IRQHandler
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//GPIO_Mode_IPU;			//��������	������ int���� �󲻶ࣩ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
    //�жϳ�ʼ������   �����ش���
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);

  	EXTI_InitStructure.EXTI_Line		=			EXTI_Line10;					//PB10  INT1
  	EXTI_InitStructure.EXTI_Mode 		= 		EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = 		EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = 		ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 												//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource11);
  	EXTI_InitStructure.EXTI_Line		=			EXTI_Line11;
//	EXTI_InitStructure.EXTI_Mode 		= 		EXTI_Mode_Interrupt;	
//  EXTI_InitStructure.EXTI_Trigger = 		EXTI_Trigger_Rising;
  	EXTI_Init(&EXTI_InitStructure);	  											//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;									//ʹ���ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01;					//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority 				= 0x00;					//�����ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;												//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 

}


/*****************i2cӲ������**********************/
static void i2c_Start(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
	I2C_SDA_1();
	I2C_SCL_1();
	i2c_Delay(5);
	I2C_SDA_0();
	i2c_Delay(5);
	I2C_SCL_0();
	i2c_Delay(5);
}


/*SCL�ߵ�ƽ sda����������*/
void i2c_stop(void)
{
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay(5);
	I2C_SDA_1();
	i2c_Delay(5);
}

/**дһ���ֽ�*/
static void i2c_SendByte(u8 SendByte)
{
	uint8_t i;

	/* �ȷ����ֽڵĸ�λbit7 */
	for (i = 0; i < 8; i++)
	{		
		if (SendByte & 0x80)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		
		i2c_Delay(4);
		I2C_SCL_1();
		i2c_Delay(20);	
		I2C_SCL_0();
		if (i == 7)
		{
			 I2C_SDA_1(); // �ͷ�����
		}
		SendByte <<= 1;	/* ����һ��bit */
		i2c_Delay(10);
	}
}
/**��һ���ֽ�**/
static u8 i2c_ReadByte(void)
{
	u8 i;
	u8 value;

	/* ������1��bitΪ���ݵ�bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_1();
		i2c_Delay(5);
		if (I2C_SDA_READ())
		{
			value++;
		}
		I2C_SCL_0();
		i2c_Delay(5);
	}
	return value;
}
 
/*CPU����һ��ʱ�Ӳ���ȡӦ��*/
static uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	I2C_SDA_1();	/* CPU�ͷ�SDA���� */
	i2c_Delay(5);
	I2C_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	i2c_Delay(8);
	if (I2C_SDA_READ())	/* CPU��ȡSDA����״̬ */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_0();
	i2c_Delay(5);
	return re;
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Ack
*	����˵��: CPU����һ��ACK�ź�
*********************************************************************************************************
*/
static void i2c_Ack(void)
{
	I2C_SDA_0();	/* CPU����SDA = 0 */
	i2c_Delay(5);
	I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay(5);
	I2C_SCL_0();
	i2c_Delay(5);
	I2C_SDA_1();	/* CPU�ͷ�SDA���� */
}


/*
*********************************************************************************************************
*	�� �� ��: i2c_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void i2c_NAck(void)
{
	I2C_SDA_1();	/* CPU����SDA = 1 */
	i2c_Delay(5);
	I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay(5);
	I2C_SCL_0();
	i2c_Delay(5);	
}


/*
*********************************************************************************************************
*	�� �� ��: i2c_CheckDevice
*	����˵��: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    �Σ�_Address���豸��I2C���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
*********************************************************************************************************
*/
u8 i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;
	
	i2c_Start();		/* ���������ź� */

	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	i2c_SendByte(_Address | I2C_WR);
	ucAck = i2c_WaitAck();	/* ����豸��ACKӦ�� */

	i2c_stop();			/* ����ֹͣ�ź� */

	return ucAck;
}




/*
*********************************************************************************************************
*	�� �� ��: Device_ReadBytes_len
*	����˵��: ��ָ����ַ����ʼ��ȡ��������
*	��    �Σ�_usAddress : ��ʼ��ַ
           Address :оƬ��ַ
*			     _usSize : ���ݳ��ȣ���λΪ�ֽ�
*			     _pReadBuf : ��Ŷ��������ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t Device_ReadBytes_len(uint8_t *_pReadBuf, u8 Address,uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i;	
	/* ��1��������I2C���������ź� */
	i2c_Start();
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte(Address | I2C_WR);	/* �˴���дָ�� */
	
	/* ��3��������ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/*MPU������Ӧ�� */
	}

	/* ��4���������ֽڵ�ַ*/
	i2c_SendByte((uint8_t)_usAddress);
	
	/* ��5��������ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	
	/* ��6������������I2C���� */
	i2c_Start();
	
	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte(Address | I2C_RD);	/* �˴��Ƕ�ָ�� */
	
	/* ��8��������ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}	
	
	/* ��9����ѭ����ȡ���� */
	for (i = 0; i < _usSize; i++)
	{
		_pReadBuf[i] = i2c_ReadByte();	/* ��1���ֽ� */
		
		/* ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack */
		if (i != _usSize - 1)
		{
			i2c_Ack();	/* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */
		}
		else
		{
			i2c_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
		}
	}
	/* ����I2C����ֹͣ�ź� */
	i2c_stop();
	return 1;	/* ִ�гɹ� */

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	i2c_stop();
	return 0;
}


#if 0
/*
*********************************************************************************************************
*	�� �� ��: Device_WriteBytes_len
*	����˵��: 
*	��    �Σ�_usAddress : ��ʼ��ַ
             Address :оƬ��ַ
*			      _usSize : ���ݳ��ȣ���λΪ�ֽ�
*			      _pWriteBuf : ��Ŷ��������ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t Device_WriteBytes_len(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i,m;
	uint16_t usAddr;
	
	usAddr = _usAddress;	
	for (i = 0; i < _usSize; i++)
	{
			/*���ڣ�������ֹͣ�źţ������ڲ�д������*/
			i2c_stop();

			for (m = 0; m < 100; m++)
			{				
				/* ��1��������I2C���������ź� */
				i2c_Start();
				
				/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
				i2c_SendByte(Address | I2C_WR);	/* �˴���дָ�� */
				
				/* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
				if (i2c_WaitAck() == 0)
				{
					break;
				}
			}
			if (m  == 1000)
			{
				goto cmd_fail;	/* д��ʱ */
			}

			i2c_SendByte((uint8_t)usAddr);
			
			/* ��5��������ACK */
			if (i2c_WaitAck() != 0)
			{
				goto cmd_fail;	/* EEPROM������Ӧ�� */
			}
	
		/* ��6������ʼд������ */
		i2c_SendByte(_pWriteBuf[i]);
	
		/* ��7��������ACK */
		if (i2c_WaitAck() != 0)
		{
			goto cmd_fail;	/* EEPROM������Ӧ�� */
		}

		usAddr++;	/* ��ַ��1 */		
	}
	
	/* ����ִ�гɹ�������I2C����ֹͣ�ź� */
	i2c_stop();
	return 1;

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	i2c_stop();
	return 0;
}
#endif
/*
*********************************************************************************************************
*	�� �� ��: Device_ReadBytes
*	����˵��: ��ָ����ַ����ʼ��ȡ һ���ֽ�
*	��    �Σ�_usAddress : ��ʼ��ַ
             Address :оƬ��ַ
*			 _pReadBuf : ��Ŷ��������ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t Device_ReadBytes(uint8_t *_pReadBuf, u8 Address,uint16_t _usAddress)
{

	/* ��1��������I2C���������ź� */
	i2c_Start();
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte(Address | I2C_WR);	/* �˴���дָ�� */
	
	/* ��3��������ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/*MPU������Ӧ�� */
	}

	/* ��4���������ֽڵ�ַ*/
	i2c_SendByte((uint8_t)_usAddress);
	
	/* ��5��������ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	
	/* ��6������������I2C���� */
	i2c_Start();
	
	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte(Address | I2C_RD);	/* �˴��Ƕ�ָ�� */
	
	/* ��8��������ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}	
	
	/* ��9����ѭ����ȡ���� */
		*_pReadBuf= i2c_ReadByte();	/* ��1���ֽ� */
		
		/* ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack */

		i2c_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */

		/* ����I2C����ֹͣ�ź� */
		i2c_stop();
		return 1;	/* ִ�гɹ� */

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
		i2c_stop();
		return 0;
}

/**********************************************************************************************************
*	�� �� ��: Device_WriteBytes
*	����˵��: 
*	��    �Σ�_usAddress : ��ʼ��ַ
          Address :оƬ��ַ
*			 _usSize : ���ݳ��ȣ���λΪ�ֽ�
*			 _pWriteBuf : ��Ŷ��������ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t Device_WriteBytes(uint8_t data, u8 Address,uint16_t _usAddress)
{
	uint16_t m;
	uint16_t usAddr;
	
	usAddr = _usAddress;	
			i2c_stop();

			for (m = 0; m < 100; m++)
			{				
				/* ��1��������I2C���������ź� */
				i2c_Start();
				
				/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
				i2c_SendByte(Address | I2C_WR);	/* �˴���дָ�� */
				
				/* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
				if (i2c_WaitAck() == 0)
				{
					break;
				}
			}
			if (m  == 1000)
			{
				goto cmd_fail;	/* д��ʱ */
			}

			i2c_SendByte((uint8_t)usAddr);
			
			/* ��5��������ACK */
			if (i2c_WaitAck() != 0)
			{
				goto cmd_fail;	/* EEPROM������Ӧ�� */
			}
	
		/* ��6������ʼд������ */
		i2c_SendByte(data);
	
		/* ��7��������ACK */
		if (i2c_WaitAck() != 0)
		{
			goto cmd_fail;	/* EEPROM������Ӧ�� */
		}	
	/* ����ִ�гɹ�������I2C����ֹͣ�ź� */
		i2c_stop();
		return 1;

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	i2c_stop();
	return 0;
}





/******           endif             *****/
