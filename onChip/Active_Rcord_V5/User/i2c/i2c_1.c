#include "i2c_1.h"
#include "stm32f10x_exti.h"
//This is for ADXL345

#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */

#define MPU_ADRESS	(0X68<<1)   //MPU地址

#define GPIO_PORT_I2C	GPIOB			/* GPIO端口 */
#define I2C_SCL_PIN		GPIO_Pin_6			/* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN		GPIO_Pin_7			/* 连接到SDA数据线的GPIO */

#define I2C_SCL_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 1 */
#define I2C_SCL_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 0 */
	
#define I2C_SDA_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 1 */
#define I2C_SDA_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 0 */
#define I2C_SDA_READ()  GPIO_ReadInputDataBit(GPIO_PORT_I2C, I2C_SDA_PIN)	/* 读SDA口线状态 */

/***mpu i2c通讯延迟函数**/
//		CPU主频72MHz时，在内部Flash运行, MDK工程不优化
//		循环次数为10时，SCL频率 = 205KHz 
//		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
//	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
static void i2c_Delay (u8 us) 
{
	u8 i;
	for(i = us;i>0;i--){
	}
}



/**i2c GPIO 管脚配置  这里I2C是模拟的**/
void i2c_GPIO_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);			//使能复用功能时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);						 		//PB6，PB7至高
	
	
	//INT1和INT2 上升沿触发中断，，，EXTI15_10_IRQHandler
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//GPIO_Mode_IPU;			//下拉电阻	（外设 int引脚 大不多）
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
    //中断初始化配置   上升沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);

  	EXTI_InitStructure.EXTI_Line		=			EXTI_Line10;					//PB10  INT1
  	EXTI_InitStructure.EXTI_Mode 		= 		EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = 		EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = 		ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 												//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource11);
  	EXTI_InitStructure.EXTI_Line		=			EXTI_Line11;
//	EXTI_InitStructure.EXTI_Mode 		= 		EXTI_Mode_Interrupt;	
//  EXTI_InitStructure.EXTI_Trigger = 		EXTI_Trigger_Rising;
  	EXTI_Init(&EXTI_InitStructure);	  											//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;									//使能外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 0x01;					//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority 				= 0x00;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;												//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 

}


/*****************i2c硬件配置**********************/
static void i2c_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	I2C_SDA_1();
	I2C_SCL_1();
	i2c_Delay(5);
	I2C_SDA_0();
	i2c_Delay(5);
	I2C_SCL_0();
	i2c_Delay(5);
}


/*SCL高电平 sda出现上跳变*/
void i2c_stop(void)
{
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay(5);
	I2C_SDA_1();
	i2c_Delay(5);
}

/**写一个字节*/
static void i2c_SendByte(u8 SendByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
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
			 I2C_SDA_1(); // 释放总线
		}
		SendByte <<= 1;	/* 左移一个bit */
		i2c_Delay(10);
	}
}
/**读一个字节**/
static u8 i2c_ReadByte(void)
{
	u8 i;
	u8 value;

	/* 读到第1个bit为数据的bit7 */
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
 
/*CPU产生一个时钟并读取应答*/
static uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	I2C_SDA_1();	/* CPU释放SDA总线 */
	i2c_Delay(5);
	I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	i2c_Delay(8);
	if (I2C_SDA_READ())	/* CPU读取SDA口线状态 */
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
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*********************************************************************************************************
*/
static void i2c_Ack(void)
{
	I2C_SDA_0();	/* CPU驱动SDA = 0 */
	i2c_Delay(5);
	I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay(5);
	I2C_SCL_0();
	i2c_Delay(5);
	I2C_SDA_1();	/* CPU释放SDA总线 */
}


/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void i2c_NAck(void)
{
	I2C_SDA_1();	/* CPU驱动SDA = 1 */
	i2c_Delay(5);
	I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay(5);
	I2C_SCL_0();
	i2c_Delay(5);	
}


/*
*********************************************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
u8 i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;
	
	i2c_Start();		/* 发送启动信号 */

	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte(_Address | I2C_WR);
	ucAck = i2c_WaitAck();	/* 检测设备的ACK应答 */

	i2c_stop();			/* 发送停止信号 */

	return ucAck;
}




/*
*********************************************************************************************************
*	函 数 名: Device_ReadBytes_len
*	功能说明: 从指定地址处开始读取若干数据
*	形    参：_usAddress : 起始地址
           Address :芯片地址
*			     _usSize : 数据长度，单位为字节
*			     _pReadBuf : 存放读到的数据的缓冲区指针
*	返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/
uint8_t Device_ReadBytes_len(uint8_t *_pReadBuf, u8 Address,uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i;	
	/* 第1步：发起I2C总线启动信号 */
	i2c_Start();
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(Address | I2C_WR);	/* 此处是写指令 */
	
	/* 第3步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/*MPU器件无应答 */
	}

	/* 第4步：发送字节地址*/
	i2c_SendByte((uint8_t)_usAddress);
	
	/* 第5步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	
	/* 第6步：重新启动I2C总线 */
	i2c_Start();
	
	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(Address | I2C_RD);	/* 此处是读指令 */
	
	/* 第8步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}	
	
	/* 第9步：循环读取数据 */
	for (i = 0; i < _usSize; i++)
	{
		_pReadBuf[i] = i2c_ReadByte();	/* 读1个字节 */
		
		/* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */
		if (i != _usSize - 1)
		{
			i2c_Ack();	/* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */
		}
		else
		{
			i2c_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
		}
	}
	/* 发送I2C总线停止信号 */
	i2c_stop();
	return 1;	/* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_stop();
	return 0;
}


#if 0
/*
*********************************************************************************************************
*	函 数 名: Device_WriteBytes_len
*	功能说明: 
*	形    参：_usAddress : 起始地址
             Address :芯片地址
*			      _usSize : 数据长度，单位为字节
*			      _pWriteBuf : 存放读到的数据的缓冲区指针
*	返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/
uint8_t Device_WriteBytes_len(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i,m;
	uint16_t usAddr;
	
	usAddr = _usAddress;	
	for (i = 0; i < _usSize; i++)
	{
			/*　第０步：发停止信号，启动内部写操作　*/
			i2c_stop();

			for (m = 0; m < 100; m++)
			{				
				/* 第1步：发起I2C总线启动信号 */
				i2c_Start();
				
				/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
				i2c_SendByte(Address | I2C_WR);	/* 此处是写指令 */
				
				/* 第3步：发送一个时钟，判断器件是否正确应答 */
				if (i2c_WaitAck() == 0)
				{
					break;
				}
			}
			if (m  == 1000)
			{
				goto cmd_fail;	/* 写超时 */
			}

			i2c_SendByte((uint8_t)usAddr);
			
			/* 第5步：发送ACK */
			if (i2c_WaitAck() != 0)
			{
				goto cmd_fail;	/* EEPROM器件无应答 */
			}
	
		/* 第6步：开始写入数据 */
		i2c_SendByte(_pWriteBuf[i]);
	
		/* 第7步：发送ACK */
		if (i2c_WaitAck() != 0)
		{
			goto cmd_fail;	/* EEPROM器件无应答 */
		}

		usAddr++;	/* 地址增1 */		
	}
	
	/* 命令执行成功，发送I2C总线停止信号 */
	i2c_stop();
	return 1;

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_stop();
	return 0;
}
#endif
/*
*********************************************************************************************************
*	函 数 名: Device_ReadBytes
*	功能说明: 从指定地址处开始读取 一个字节
*	形    参：_usAddress : 起始地址
             Address :芯片地址
*			 _pReadBuf : 存放读到的数据的缓冲区指针
*	返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/
uint8_t Device_ReadBytes(uint8_t *_pReadBuf, u8 Address,uint16_t _usAddress)
{

	/* 第1步：发起I2C总线启动信号 */
	i2c_Start();
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(Address | I2C_WR);	/* 此处是写指令 */
	
	/* 第3步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/*MPU器件无应答 */
	}

	/* 第4步：发送字节地址*/
	i2c_SendByte((uint8_t)_usAddress);
	
	/* 第5步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	
	/* 第6步：重新启动I2C总线 */
	i2c_Start();
	
	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(Address | I2C_RD);	/* 此处是读指令 */
	
	/* 第8步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}	
	
	/* 第9步：循环读取数据 */
		*_pReadBuf= i2c_ReadByte();	/* 读1个字节 */
		
		/* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */

		i2c_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */

		/* 发送I2C总线停止信号 */
		i2c_stop();
		return 1;	/* 执行成功 */

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
		i2c_stop();
		return 0;
}

/**********************************************************************************************************
*	函 数 名: Device_WriteBytes
*	功能说明: 
*	形    参：_usAddress : 起始地址
          Address :芯片地址
*			 _usSize : 数据长度，单位为字节
*			 _pWriteBuf : 存放读到的数据的缓冲区指针
*	返 回 值: 0 表示失败，1表示成功
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
				/* 第1步：发起I2C总线启动信号 */
				i2c_Start();
				
				/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
				i2c_SendByte(Address | I2C_WR);	/* 此处是写指令 */
				
				/* 第3步：发送一个时钟，判断器件是否正确应答 */
				if (i2c_WaitAck() == 0)
				{
					break;
				}
			}
			if (m  == 1000)
			{
				goto cmd_fail;	/* 写超时 */
			}

			i2c_SendByte((uint8_t)usAddr);
			
			/* 第5步：发送ACK */
			if (i2c_WaitAck() != 0)
			{
				goto cmd_fail;	/* EEPROM器件无应答 */
			}
	
		/* 第6步：开始写入数据 */
		i2c_SendByte(data);
	
		/* 第7步：发送ACK */
		if (i2c_WaitAck() != 0)
		{
			goto cmd_fail;	/* EEPROM器件无应答 */
		}	
	/* 命令执行成功，发送I2C总线停止信号 */
		i2c_stop();
		return 1;

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_stop();
	return 0;
}





/******           endif             *****/
