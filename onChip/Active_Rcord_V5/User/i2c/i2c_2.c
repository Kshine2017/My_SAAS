#include "i2c_2.h"

//This is for BH1750fvi

#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */

#define MPU_ADRESS	(0X68<<1)   //MPU地址

#define GPIO_PORT_I2C	GPIOB			/* GPIO端口 */
#define I2C_SCL_PIN		GPIO_Pin_0			/* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN		GPIO_Pin_1			/* 连接到SDA数据线的GPIO */

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
static void i2c_Delay_2(u8 us)
{
    u8 i;
    for(i = us; i>0; i--) {
    }
}



/**i2c GPIO 管脚配置  这里I2C是模拟的**/
void i2c_GPIO_init_2(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);						 //PB0，PB1
}

/*****************i2c硬件配置**********************/

static void i2c_Start_2()
{
    /* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
    I2C_SDA_1();
    I2C_SCL_1();
    i2c_Delay_2(5);
    I2C_SDA_0();
    i2c_Delay_2(5);
    I2C_SCL_0();
    i2c_Delay_2(5);
}

/*SCL高电平 sda出现上跳变*/
void i2c_stop_2(void)
{
    I2C_SDA_0();
    I2C_SCL_1();
    i2c_Delay_2(5);
    I2C_SDA_1();
    i2c_Delay_2(5);
}

/**写一个字节*/
static void i2c_SendByte_2(u8 SendByte)
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

        i2c_Delay_2(4);
        I2C_SCL_1();
        i2c_Delay_2(20);
        I2C_SCL_0();
        if (i == 7)
        {
            I2C_SDA_1(); // 释放总线
        }
        SendByte <<= 1;	/* 左移一个bit */
        i2c_Delay_2(10);
    }
}
/**读一个字节**/
static u8 i2c_ReadByte_2(void)
{
    u8 i;
    u8 value;

    /* 读到第1个bit为数据的bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        I2C_SCL_1();
        i2c_Delay_2(5);
        if (I2C_SDA_READ())
        {
            value++;
        }
        I2C_SCL_0();
        i2c_Delay_2(5);
    }
    return value;
}

/*CPU产生一个时钟并读取应答*/
static uint8_t i2c_WaitAck_2(void)
{
    uint8_t re;

    I2C_SDA_1();					/* CPU释放SDA总线 */
    i2c_Delay_2(5);
    I2C_SCL_1();					/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
    i2c_Delay_2(8);
    if (I2C_SDA_READ())		/* CPU读取SDA口线状态 */
    {
        re = 1;  	//高电平为 无效
    }
    else
    {
        re = 0;   //低电平有效
    }
    I2C_SCL_0();
    i2c_Delay_2(5);
    return re;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*********************************************************************************************************
*/
static void i2c_Ack_2(void)
{
    I2C_SDA_0();	/* CPU驱动SDA = 0 */
    i2c_Delay_2(5);
    I2C_SCL_1();	/* CPU产生1个时钟 */
    i2c_Delay_2(5);
    I2C_SCL_0();
    i2c_Delay_2(5);
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
static void i2c_NAck_2(void)
{
    I2C_SDA_1();	/* CPU驱动SDA = 1 */
    i2c_Delay_2(5);
    I2C_SCL_1();	/* CPU产生1个时钟 */
    i2c_Delay_2(5);
    I2C_SCL_0();
    i2c_Delay_2(5);
}


/*
*********************************************************************************************************
*	函 数 名: i2c_CheckDevice_2
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
u8 i2c_CheckDevice_2(uint8_t _Address)
{
    uint8_t ucAck;

    i2c_Start_2();		/* 发送启动信号 */

    /* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
    i2c_SendByte_2(_Address | I2C_WR);
    ucAck = i2c_WaitAck_2();	/* 检测设备的ACK应答 */

    i2c_stop_2();			/* 发送停止信号 */

    return ucAck;
}







/*
*********************************************************************************************************
*	函 数 名: Device_ReadBytes_2
*	功能说明: 从指定地址处开始读取 一个字节
*	形    参：_usAddress : 起始地址
             Address :芯片地址
*			 _pReadBuf : 存放读到的数据的缓冲区指针
*	返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/

 /* 每读完1个字节后，需要发送Ack， 最后一个字节不需要Ack，发Nack */
/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
uint8_t Device_ReadBytes_2(uint16_t *_pReadBuf, u8 Address)
{

		uint16_t Highdata=0,Lowdata=0;
	
    i2c_Start_2();

    i2c_SendByte_2(Address | I2C_RD);	/* 此处是读指令 */

    if (i2c_WaitAck_2() != 0)goto cmd_fail;	/* EEPROM器件无应答 */;//
 
		Highdata= i2c_ReadByte_2();	/* 读高字节  15:8*/
		i2c_Ack_2();//继续读取
		Lowdata= i2c_ReadByte_2();	/* 读低字节 7:0*/
    i2c_NAck_2();	//结束读取	
    i2c_stop_2();
		//计算结果，并保存
		*_pReadBuf = ( Highdata <<8) + Lowdata / 1.2;
	
    return 1;	/* 执行成功 */
cmd_fail: 
    i2c_stop_2();
		//计算结果，并保存
		*_pReadBuf =0;
    return 0;
}

/**********************************************************************************************************
*	函 数 名: Device_WriteBytes_2
*	功能说明:
*	形    参
*	返 回 值: 0 表示失败，1表示成功
*********************************************************************************************************
*/
uint8_t Device_WriteBytes_2(uint8_t command, u8 Address)
{
    uint16_t m;
    i2c_stop_2();

	
    for (m = 0; m < 100; m++)
    {
        i2c_Start_2();
        i2c_SendByte_2(Address | I2C_WR);	
        if (i2c_WaitAck_2() == 0) break;   //得到0 表示应答有效   
    }
		
    if (m  == 100) goto cmd_fail;	/* 写超时 */

    i2c_SendByte_2(command);
    if (i2c_WaitAck_2() != 0) goto cmd_fail;	/* EEPROM器件无应答 */

    i2c_stop_2();
		
    return 1;

cmd_fail:
		
		i2c_stop_2();
		printf("i2c_WaitAck_2  应答无效！ ---285\n");
		
		
    return 0;
}


void bh_data_send(uint8_t command)  
{  
    do{  
    i2c_Start_2();                      //iic起始信号  
    i2c_SendByte_2(0x46);       //发送器件地址  
			//printf("i2c_WaitAck_2  应答无效！ ---299\n");
    }while(!i2c_WaitAck_2());           //等待从机应答  
    i2c_SendByte_2(command);          //发送指令  
    i2c_WaitAck_2();                   //等待从机应答  
    i2c_stop_2();                       //iic停止信号  
}   

uint16_t bh_data_read(void)  
{  
    uint16_t buf;  
    i2c_Start_2();                       //iic起始信号  
    i2c_SendByte_2(0x47);         //发送器件地址+读标志位  
    i2c_WaitAck_2();                     //等待从机应答  
	
    buf=i2c_ReadByte_2();              //读取数据  
    buf=buf<<8;    //读取并保存高八位数据 
		i2c_Ack_2();   //cpu发送ack 
	
    buf+=0x00ff&i2c_ReadByte_2();      //读取并保存第八位数据 
		i2c_NAck_2();	//结束读取	
    i2c_stop_2();                        //发送停止信号   
    return buf;   
}  






/******           endif             *****/
