#include "i2c_2.h"

//This is for BH1750fvi

#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */

#define MPU_ADRESS	(0X68<<1)   //MPU��ַ

#define GPIO_PORT_I2C	GPIOB			/* GPIO�˿� */
#define I2C_SCL_PIN		GPIO_Pin_0			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define I2C_SDA_PIN		GPIO_Pin_1			/* ���ӵ�SDA�����ߵ�GPIO */

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
static void i2c_Delay_2(u8 us)
{
    u8 i;
    for(i = us; i>0; i--) {
    }
}



/**i2c GPIO �ܽ�����  ����I2C��ģ���**/
void i2c_GPIO_init_2(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);						 //PB0��PB1
}

/*****************i2cӲ������**********************/

static void i2c_Start_2()
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
    I2C_SDA_1();
    I2C_SCL_1();
    i2c_Delay_2(5);
    I2C_SDA_0();
    i2c_Delay_2(5);
    I2C_SCL_0();
    i2c_Delay_2(5);
}

/*SCL�ߵ�ƽ sda����������*/
void i2c_stop_2(void)
{
    I2C_SDA_0();
    I2C_SCL_1();
    i2c_Delay_2(5);
    I2C_SDA_1();
    i2c_Delay_2(5);
}

/**дһ���ֽ�*/
static void i2c_SendByte_2(u8 SendByte)
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

        i2c_Delay_2(4);
        I2C_SCL_1();
        i2c_Delay_2(20);
        I2C_SCL_0();
        if (i == 7)
        {
            I2C_SDA_1(); // �ͷ�����
        }
        SendByte <<= 1;	/* ����һ��bit */
        i2c_Delay_2(10);
    }
}
/**��һ���ֽ�**/
static u8 i2c_ReadByte_2(void)
{
    u8 i;
    u8 value;

    /* ������1��bitΪ���ݵ�bit7 */
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

/*CPU����һ��ʱ�Ӳ���ȡӦ��*/
static uint8_t i2c_WaitAck_2(void)
{
    uint8_t re;

    I2C_SDA_1();					/* CPU�ͷ�SDA���� */
    i2c_Delay_2(5);
    I2C_SCL_1();					/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
    i2c_Delay_2(8);
    if (I2C_SDA_READ())		/* CPU��ȡSDA����״̬ */
    {
        re = 1;  	//�ߵ�ƽΪ ��Ч
    }
    else
    {
        re = 0;   //�͵�ƽ��Ч
    }
    I2C_SCL_0();
    i2c_Delay_2(5);
    return re;
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Ack
*	����˵��: CPU����һ��ACK�ź�
*********************************************************************************************************
*/
static void i2c_Ack_2(void)
{
    I2C_SDA_0();	/* CPU����SDA = 0 */
    i2c_Delay_2(5);
    I2C_SCL_1();	/* CPU����1��ʱ�� */
    i2c_Delay_2(5);
    I2C_SCL_0();
    i2c_Delay_2(5);
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
static void i2c_NAck_2(void)
{
    I2C_SDA_1();	/* CPU����SDA = 1 */
    i2c_Delay_2(5);
    I2C_SCL_1();	/* CPU����1��ʱ�� */
    i2c_Delay_2(5);
    I2C_SCL_0();
    i2c_Delay_2(5);
}


/*
*********************************************************************************************************
*	�� �� ��: i2c_CheckDevice_2
*	����˵��: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    �Σ�_Address���豸��I2C���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
*********************************************************************************************************
*/
u8 i2c_CheckDevice_2(uint8_t _Address)
{
    uint8_t ucAck;

    i2c_Start_2();		/* ���������ź� */

    /* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
    i2c_SendByte_2(_Address | I2C_WR);
    ucAck = i2c_WaitAck_2();	/* ����豸��ACKӦ�� */

    i2c_stop_2();			/* ����ֹͣ�ź� */

    return ucAck;
}







/*
*********************************************************************************************************
*	�� �� ��: Device_ReadBytes_2
*	����˵��: ��ָ����ַ����ʼ��ȡ һ���ֽ�
*	��    �Σ�_usAddress : ��ʼ��ַ
             Address :оƬ��ַ
*			 _pReadBuf : ��Ŷ��������ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
*********************************************************************************************************
*/

 /* ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack */
/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
uint8_t Device_ReadBytes_2(uint16_t *_pReadBuf, u8 Address)
{

		uint16_t Highdata=0,Lowdata=0;
	
    i2c_Start_2();

    i2c_SendByte_2(Address | I2C_RD);	/* �˴��Ƕ�ָ�� */

    if (i2c_WaitAck_2() != 0)goto cmd_fail;	/* EEPROM������Ӧ�� */;//
 
		Highdata= i2c_ReadByte_2();	/* �����ֽ�  15:8*/
		i2c_Ack_2();//������ȡ
		Lowdata= i2c_ReadByte_2();	/* �����ֽ� 7:0*/
    i2c_NAck_2();	//������ȡ	
    i2c_stop_2();
		//��������������
		*_pReadBuf = ( Highdata <<8) + Lowdata / 1.2;
	
    return 1;	/* ִ�гɹ� */
cmd_fail: 
    i2c_stop_2();
		//��������������
		*_pReadBuf =0;
    return 0;
}

/**********************************************************************************************************
*	�� �� ��: Device_WriteBytes_2
*	����˵��:
*	��    ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
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
        if (i2c_WaitAck_2() == 0) break;   //�õ�0 ��ʾӦ����Ч   
    }
		
    if (m  == 100) goto cmd_fail;	/* д��ʱ */

    i2c_SendByte_2(command);
    if (i2c_WaitAck_2() != 0) goto cmd_fail;	/* EEPROM������Ӧ�� */

    i2c_stop_2();
		
    return 1;

cmd_fail:
		
		i2c_stop_2();
		printf("i2c_WaitAck_2  Ӧ����Ч�� ---285\n");
		
		
    return 0;
}


void bh_data_send(uint8_t command)  
{  
    do{  
    i2c_Start_2();                      //iic��ʼ�ź�  
    i2c_SendByte_2(0x46);       //����������ַ  
			//printf("i2c_WaitAck_2  Ӧ����Ч�� ---299\n");
    }while(!i2c_WaitAck_2());           //�ȴ��ӻ�Ӧ��  
    i2c_SendByte_2(command);          //����ָ��  
    i2c_WaitAck_2();                   //�ȴ��ӻ�Ӧ��  
    i2c_stop_2();                       //iicֹͣ�ź�  
}   

uint16_t bh_data_read(void)  
{  
    uint16_t buf;  
    i2c_Start_2();                       //iic��ʼ�ź�  
    i2c_SendByte_2(0x47);         //����������ַ+����־λ  
    i2c_WaitAck_2();                     //�ȴ��ӻ�Ӧ��  
	
    buf=i2c_ReadByte_2();              //��ȡ����  
    buf=buf<<8;    //��ȡ������߰�λ���� 
		i2c_Ack_2();   //cpu����ack 
	
    buf+=0x00ff&i2c_ReadByte_2();      //��ȡ������ڰ�λ���� 
		i2c_NAck_2();	//������ȡ	
    i2c_stop_2();                        //����ֹͣ�ź�   
    return buf;   
}  






/******           endif             *****/
