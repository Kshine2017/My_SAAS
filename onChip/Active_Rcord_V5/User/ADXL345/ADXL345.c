#include "ADXL345.h"
#include "i2c_1.h"
#include "AD.h"
#include "file.h"
#include "rtc.h"
#include "math.h"
#include "../BH1750fvi/bh1750fvi.h"
/*
	ADX���õ��ǲ����ߣ�����16g��Χ  �ֱ���Ϊ10λ
*/


ADXL345_Data_t AD345_strcut_Data = {
    0,0,0,0, ~0        //ADX345���ݲɼ���־
};  //ADX345���ݱ���
/*����ADXL345��ID�ţ��ж�ADXL345оƬ�Ƿ�OK */
unsigned char Check_ADXL345(void)
{
    u8 buf = 0;
    Device_ReadBytes(&buf, ADXL345_ADDRESS,ADXL_R_ID);
    if( buf == 0xe5) {
        return 0;
    } else return 1;
}



/*
	����ADXL345
*/
void ADXL345_init(void)
{
    /*

    	�Ĵ���0x31��DATA_FORMAT(��/д)
    	D7 							D6 						D5 						D4 	D3 					D2 					D1 D0
    	SELF_TEST 			SPI 					INT_INVERT 		0 	FULL_RES 		Justify 		Range
    	�Բ���ʹ��λ			1 3��ʽspi		0�жϸ���Ч				1 13�ֱ���		0�Ҷ���			��Χ
    									0	4��ʽspi		1�жϵ���Ч				0 10�ֱ���		1�����

    	//-------------------------------------------------------
    	0x2B
    	0								0							1							0	 	1						0						1		1��16g��
    																																					1		0��8g��
    																																					0		1	��4g��
    																																					0		0	��2g��
    */
    Device_WriteBytes(0x2B,ADXL345_ADDRESS,DATA_FORMAT);				//������Χ,����16g��ȫλģʽ
    /*
    	�Ĵ���0x2C��BW_RATE(��/д)
    	D7 	D6 	D5 	D4 										D3 	D2 	D1 		D0
    	0		0		0		LOW_POWER							(			����		)
    							0 ��������
    							1	�͹���(����������)
    	//-------------------------------------------------------
    	0		0		0		0											1010

    */
    Device_WriteBytes(0x0A,ADXL345_ADDRESS,BW_RATE);  					//���ó�����ģʽ��100HZ  //����50

    /*

    	�Ĵ���0x2D��POWER_CTL(��/д)
    	D7 	D6	D5		D4 					D3 		D2 		D1 D0
    	0		0		����	AUTO_SLEEP	����	����	(����ģʽ�µĶ�ȡƵ��)


    	//-------------------------------------------------------
    	0		0		0			0						1			1			0		0��8HZ��

    */
    Device_WriteBytes(0x0C,ADXL345_ADDRESS,POWER_CTL);         	//���ó�����ģʽ��������, ����ģʽ
}

/*
  �õ����ٶ���ֵ
*/
void Get_ADXL345_data(ADXL345_Data_t *data)
{
    uint8_t buf_1[6] = {0};
		
    short int buf_2[3] = {0};
    Device_ReadBytes_len(buf_1, ADXL345_ADDRESS,X_DATA_ADDRESS, 6);

    buf_2[0] = (short int )buf_1[0] | ((short int )buf_1[1]<<8);
    buf_2[1] = (short int )buf_1[2] | ((short int )buf_1[3]<<8);
    buf_2[2] = (short int )buf_1[4] | ((short int )buf_1[5]<<8);

    if(buf_2[0]& (1<<10))   //������ת��������
    {
        buf_2[0] = ~buf_2[0]+1;
    }

    if(buf_2[1]& (1<<10))
    {
        buf_2[1] = ~buf_2[1]+1;
    }

    if(buf_2[2]& (1<<10))
    {
        buf_2[2] = ~buf_2[2]+1;
    }
    data->x_axis = buf_2[0];
    data->y_axis = buf_2[1];
    data->z_axis = buf_2[2];


}

//���ٶȴ�������һ��ʱ���ڵ���ĳֵ���Զ�˯�ߣ�����ĳֵ�Զ�����
/*
ADXL345��һ��������ٶȼơ�
����+/-2g��+/-4g��+/-8g��+/-16g�ɱ�Ĳ�����Χ��
���13bit�ֱ��ʣ�
�̶���4mg/LSB�����ȣ�
3mm*5mm*1mm��С��װ��
40-145uA���͹��ģ�
��׼�� I2C��SPI���ֽӿڣ�
32��FIFO�洢��
�Լ��ڲ������˶�״̬���������жϷ�ʽ������
*/
void ADXL345_init_sleep_wakeup()
{
    unsigned char devid = 0, val = 0;
		u8 i=0;
		short offx=0,offy=0,offz=0;
		ADXL345_Data_t temp;
    delay_us(300);

    //I2C_ReadByte(ADXL345_ADDRESS, ADXL_R_ID, &devid);         //��ID   ��ÿ�ζ�д֮ǰ����Ҫ��ID
//		Device_ReadBytes( &devid,ADXL345_ADDRESS,ADXL_R_ID);
//    delay_us(300);

    /*

    �Ĵ���0x31��DATA_FORMAT(��/д)
    D7 							D6 						D5 						D4 	D3 					D2 					D1 D0
    SELF_TEST 			SPI 					INT_INVERT 		0 	FULL_RES 		Justify 		Range
    �Բ���ʹ��λ			1 3��ʽspi		0�жϸ���Ч				1 13�ֱ���		0�Ҷ���			��Χ
    								0	4��ʽspi		1�жϵ���Ч				0 10�ֱ���		1�����

    //-------------------------------------------------------
    0x2B
    0								0							0							1	 	1						0						1		1��16g��
    																																				1		0��8g��
    																																				0		1	��4g��
    																																				0		0	��2g��
    */
    //val = 0x1B;
    //I2C_WriteByte(ADXL345_ADDRESS, DATA_FORMAT, &val);  			//�͵�ƽ�ж����,13λȫ�ֱ���,��������Ҷ���,16g����
    Device_WriteBytes(0x1B,ADXL345_ADDRESS,DATA_FORMAT);
    delay_us(50);

    /*
    	�Ĵ���0x2C��BW_RATE(��/д)
    	D7 	D6 	D5 	D4 										D3 	D2 	D1 		D0
    	0		0		0		LOW_POWER							(			����		)
    							0 ��������
    							1	�͹���(����������)
    	//-------------------------------------------------------
    	0		0		0		0											1010

    */
    //val = 0x0A;
    //I2C_WriteByte(ADXL345_ADDRESS, BW_RATE, &val);          		//��������ٶ�Ϊ100Hz
    Device_WriteBytes(0x0A,ADXL345_ADDRESS,BW_RATE);
    delay_us(50);
    /*

    	�Ĵ���0x2D��POWER_CTL(��/д)
    	D7 	D6	D5		D4 					D3 		D2 		D1 D0
    	0		0		����	AUTO_SLEEP	����	����	(����ģʽ�µĶ�ȡƵ��)


    	//-------------------------------------------------------
    	0		0		0			0						1			1			0		0��8HZ��

    */
    //val = 0x38;                                         				//�Զ�����ģʽ������ʱ��8HZ��Ƶ�ʲ���
    //I2C_WriteByte(ADXL345_ADDRESS, POWER_CTL, &val);
    Device_WriteBytes(0x38,ADXL345_ADDRESS,POWER_CTL);
    delay_us(50);

    /*
    �Ĵ���0x2E��INT_ENABLE(��/д)
    D7 					D6 				D5 					D4 				D3 					D2 				D1 				D0
    DATA_READY SINGLE_TAP DOUBLE_TAP 	Activity 	Inactivity 	FREE_FALL Watermark Overrun

    */
//		val = 0;
//    I2C_WriteByte(ADXL345_ADDRESS, INT_ENABLE, &val);       	//�ȹ��ж�
    Device_WriteBytes(0,ADXL345_ADDRESS,INT_ENABLE);



    //�޷��ţ���������62.5mg/LSB
//    val = 0x10;                                        					//˯�߼���activity��ֵ�����������ֵ��ʱ���ѣ�����10����1g  16*62.5=1000mg
//    I2C_WriteByte(ADXL345_ADDRESS, THRESH_ACT, &val);
    Device_WriteBytes(0x0E,ADXL345_ADDRESS,THRESH_ACT);//0x12  18

//    val = 0x02;  // val = 0x10;                       	 				//˯�߿�ʼinactivity��ֵ����С�����ֵ��ʱ��˯�ߣ�����02����0.2g
//    I2C_WriteByte(ADXL345_ADDRESS, THRESH_INACT, &val);
    Device_WriteBytes(0x02,ADXL345_ADDRESS,THRESH_INACT);

    //��������Ϊ1 sec /LSB
//    val = 0x02;                                      						//��С��inactivityֵʱ�䳬�����ֵ��ʱ�����˯�ߣ�����02����2��
//    I2C_WriteByte(ADXL345_ADDRESS, TIME_INACT, &val);
    Device_WriteBytes(0x05,ADXL345_ADDRESS,TIME_INACT);


//    val = 0xCC;                                          				//ֱ�������������ã�XYZʹ�ܴ������ã��˴�ѡ��X��������

//    I2C_WriteByte(ADXL345_ADDRESS, ACT_INACT_CTL, &val);
    Device_WriteBytes(0xFF,ADXL345_ADDRESS,ACT_INACT_CTL);

    /*
    		D7 					D6 				D5 					D4 				D3 					D2 				D1 				D0
    		DATA_READY SINGLE_TAP DOUBLE_TAP 	Activity 	Inactivity 	FREE_FALL Watermark Overrun
    */
//    val = 0x10;                                      	 					//�ж�����ѡ�񣬴˴����ǽ�activityӳ�䵽INT2
//    I2C_WriteByte(ADXL345_ADDRESS, INT_MAP, &val);
    Device_WriteBytes(0x10,ADXL345_ADDRESS,INT_MAP);
    delay_us(50);


//  val = 0xFF;                                         				//���ж�
//  I2C_WriteByte(ADXL345_ADDRESS, INT_ENABLE, &val);
		
    Device_WriteBytes(0x18,ADXL345_ADDRESS,INT_ENABLE);
    delay_ms(20);

    //���ƫ��У׼�����ϵ�ʱΪƽ��״̬��x=0��y=0��z=1=====================

//		for(i=0; i<10; i++)
//    {
//				Get_ADXL345_data(&temp);
//        offx+=temp.x_axis;
//        offy+=temp.y_axis;
//        offz+=temp.z_axis;
//    }

//		offx/=10;
//    offy/=10;
//    offz/=10;
//		Device_WriteBytes(-offx/4,ADXL345_ADDRESS,OFSX);
//		Device_WriteBytes(-offy/4,ADXL345_ADDRESS,OFSY);
//		Device_WriteBytes(-(offz-256)/4,ADXL345_ADDRESS,OFSZ);

}


/*������˲�ð��
 *����ֵ����ȥ���ֵ����Сֵ��ȥȡƽ��
 */
static ADXL345_Data_t Active_Bubble_Sort(ADXL345_Data_t  *data)
{
    u8 i = 0,j = 0;
    u16 temp = 0;
    ADXL345_Data_t ADX_Temp = {0};
    static ADXL345_Data_t ADX_Data = {0};
// X��   5�βɼ���X��ֵ ð������
    for(i = 0 ; i < ( GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE ) ; i++) {
        for(j = 0; j <( GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE )-i-1 ; j++) {
            if((data+j)->x_axis > (data+j+1)->x_axis) {
                temp = (data+j+1)->x_axis;
                (data+j+1)->x_axis = (data+j)->x_axis;
                (data+j)->x_axis = temp;
            }
        }
    }
//Y��
    for(i = 0 ; i < ( GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE ) ; i++) {
        for(j = 0; j <( GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE )-i-1 ; j++) {
            if((data+j)->y_axis > (data+j+1)->y_axis) {
                temp = (data+j+1)->y_axis;
                (data+j+1)->y_axis = (data+j)->y_axis;
                (data+j)->y_axis = temp;
            }
        }
    }
//Z��
    for(i = 0 ; i < ( GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE ) ; i++) {
        for(j = 0; j <( GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE )-i-1 ; j++) {
            if((data+j)->z_axis > (data+j+1)->z_axis) {
                temp = (data+j+1)->z_axis;
                (data+j+1)->z_axis = (data+j)->z_axis;
                (data+j)->z_axis = temp;
            }
        }
    }
// X Y Z����ȡƽ��ֵ
    temp = 0;
    for(i  = 1; i< (GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE)-1; i++) {
        temp += (data+i)->x_axis;
    }
    ADX_Temp.x_axis =  temp / (GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE - 2);//ȥ������С�����
    //ADX_Temp.x_axis =  temp / (GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE);

    temp = 0;
    for(i  = 1; i< (GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE)-1; i++) {
        temp += (data+i)->y_axis;
    }
    ADX_Temp.y_axis =  temp / (GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE - 2);//ȥ������С�����
    //ADX_Temp.y_axis =  temp / (GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE);
    temp = 0;
    for(i  = 1; i< (GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE)-1; i++) {
        temp += (data+i)->z_axis;
    }
    ADX_Temp.z_axis =  temp / (GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE - 2);//ȥ������С�����
    //ADX_Temp.z_axis =  temp / (GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE);
		
    if(	ADX_Temp.x_axis-ADX_Data.x_axis > X_D_VALUE ||
            ADX_Data.x_axis - ADX_Temp.x_axis > X_D_VALUE ||
            ADX_Temp.y_axis-ADX_Data.y_axis > Y_D_VALUE ||
            ADX_Data.y_axis - ADX_Temp.y_axis > Y_D_VALUE ||
            ADX_Temp.z_axis-ADX_Data.z_axis > Z_D_VALUE ||
            ADX_Data.z_axis - ADX_Temp.z_axis > Z_D_VALUE )
    {
        ADX_Data.x_axis = ADX_Temp.x_axis;
        ADX_Data.y_axis = ADX_Temp.y_axis;
        ADX_Data.z_axis = ADX_Temp.z_axis;
        ADX_Temp.sum_count = (short int)sqrt( ADX_Temp.x_axis * ADX_Temp.x_axis + ADX_Temp.y_axis * ADX_Temp.y_axis + ADX_Temp.z_axis * ADX_Temp.z_axis);

        if(ADX_Temp.sum_count > ADXDATA_MAX) {
            ADX_Temp.sum_count = ADXDATA_MAX;
        }
    } else ADX_Temp.sum_count = 0;

    return  ADX_Temp;
}

/*
 *�ɼ�mpu����,ֻ�й���ʱ��ִ�У���ȡ1s�вɼ�������
**/
void Collect_data(ADXL345_Data_t *Active_data,Environment_t * Envir_data,module_state* state)
{
    static u8 count = 0;
    static ADXL345_Data_t temp[GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE] = {0}; //200ms�ɼ�һ�Σ�1����1000ms
		u8 intsource=0;

    if(state->run)  																					//����״̬��
    {
        if(Active_data->run_flag)															//�Ƿ�����嶯
        {
            if(count >= GET_ADXLDATA_CYCLE/ADXL_SAMPLE_CYCLE)	//1000ms/200ms(duty)��ʱ�䵽
            {
               // printf("���ݴ���ǰ���˶�ֵ��X=  %hd \t Y=  %hd \t Z=  %hd      \n ",temp[0].x_axis,temp[0].y_axis,temp[0].z_axis);
                //����PB10��11����------------

               // printf("�ж�����״����INT1��PB10��:    %d  , INT2(PB11):    %d   \n\n",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10),GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11));
		
								Device_ReadBytes( &intsource,ADXL345_ADDRESS,INT_SOURCE);
								//printf("INT_SOURCE=== %b  \n\n",intsource);

                *Active_data = Active_Bubble_Sort(temp);  		//�õ�����3���ƽ��ֵ
                count = 0;
                //printf("�˶�ֵ��X=  %hd \t Y=  %hd \t Z=  %hd      \n ",Active_data->x_axis,Active_data->y_axis,Active_data->z_axis );

            }
            Get_ADXL345_data(&temp[count]);										//��ʱ����temp��
            count ++;                         								//�ڼ��μ���ֵҪ����
            Active_data->run_flag = 0;
						//printf("R");

        }
        if(Envir_data->Light_run_flag)
        {
            //L_Get_Value(AD_Value,&Environment_data.Light_data);
            Environment_data.Light_data = get_data_BH1750(); 	//��ʱģ��Ӧ���Ѿ��ر��� ������ʱ��120ms ��
            Envir_data->Light_run_flag = 0;

            //printf("��ǿֵ�� %.3f     ",Environment_data.Light_data);
						//printf("L");
            BH1750_init(); 																		//�������� ģ�飬ģ�������֮�󣬻��Զ��ر�
        }
        if(Envir_data->Tempet_run_flag)         							//�¶Ȳɼ�
        {
            //DMA����adc�ɼ�������  ��AD_Value��ַ���ڵ��ڴ���
            T_Get_Value(AD_Value,&Environment_data.Temperature_data);
            Envir_data->Tempet_run_flag = 0;
            //printf("�¶�ֵ�� %d       ",Environment_data.Temperature_data);
						//printf("T");
        }
        if(Envir_data->Voice_run_flag)        							 	//�����ɼ�
        {
            V_Get_Value(AD_Value,&Environment_data.Voice_data);
            Envir_data->Voice_run_flag = 0;
            //printf("����ֵ�� %d  ",Environment_data.Voice_data);
						//printf("V");
        }
        if(Envir_data->Power_check_flag )
        {
            P_Get_Value(AD_Value,&Environment_data.Power_data);
            //printf("����ֵ�� %d  \n",Environment_data.Power_data);
            Envir_data->Power_check_flag  = 0;
						//printf("P");
        }
    }
}


/******************endif***************************/
