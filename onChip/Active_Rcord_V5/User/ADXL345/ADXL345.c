#include "ADXL345.h"
#include "i2c_1.h"
#include "AD.h"
#include "file.h"
#include "rtc.h"
#include "math.h"
#include "../BH1750fvi/bh1750fvi.h"
/*
	ADX采用的是不休眠，正负16g范围  分辨率为10位
*/


ADXL345_Data_t AD345_strcut_Data = {
    0,0,0,0, ~0        //ADX345数据采集标志
};  //ADX345数据变量
/*对于ADXL345的ID号，判断ADXL345芯片是否OK */
unsigned char Check_ADXL345(void)
{
    u8 buf = 0;
    Device_ReadBytes(&buf, ADXL345_ADDRESS,ADXL_R_ID);
    if( buf == 0xe5) {
        return 0;
    } else return 1;
}



/*
	设置ADXL345
*/
void ADXL345_init(void)
{
    /*

    	寄存器0x31—DATA_FORMAT(读/写)
    	D7 							D6 						D5 						D4 	D3 					D2 					D1 D0
    	SELF_TEST 			SPI 					INT_INVERT 		0 	FULL_RES 		Justify 		Range
    	自测力使能位			1 3线式spi		0中断高有效				1 13分辨率		0右对齐			范围
    									0	4线式spi		1中断低有效				0 10分辨率		1左对齐

    	//-------------------------------------------------------
    	0x2B
    	0								0							1							0	 	1						0						1		1（16g）
    																																					1		0（8g）
    																																					0		1	（4g）
    																																					0		0	（2g）
    */
    Device_WriteBytes(0x2B,ADXL345_ADDRESS,DATA_FORMAT);				//测量范围,正负16g，全位模式
    /*
    	寄存器0x2C—BW_RATE(读/写)
    	D7 	D6 	D5 	D4 										D3 	D2 	D1 		D0
    	0		0		0		LOW_POWER							(			速率		)
    							0 正常操作
    							1	低功耗(噪声有增加)
    	//-------------------------------------------------------
    	0		0		0		0											1010

    */
    Device_WriteBytes(0x0A,ADXL345_ADDRESS,BW_RATE);  					//设置成正常模式，100HZ  //带宽50

    /*

    	寄存器0x2D—POWER_CTL(读/写)
    	D7 	D6	D5		D4 					D3 		D2 		D1 D0
    	0		0		链接	AUTO_SLEEP	测量	休眠	(休眠模式下的读取频率)


    	//-------------------------------------------------------
    	0		0		0			0						1			1			0		0（8HZ）

    */
    Device_WriteBytes(0x0C,ADXL345_ADDRESS,POWER_CTL);         	//设置成正常模式，不休眠, 测量模式
}

/*
  得到加速度数值
*/
void Get_ADXL345_data(ADXL345_Data_t *data)
{
    uint8_t buf_1[6] = {0};
		
    short int buf_2[3] = {0};
    Device_ReadBytes_len(buf_1, ADXL345_ADDRESS,X_DATA_ADDRESS, 6);

    buf_2[0] = (short int )buf_1[0] | ((short int )buf_1[1]<<8);
    buf_2[1] = (short int )buf_1[2] | ((short int )buf_1[3]<<8);
    buf_2[2] = (short int )buf_1[4] | ((short int )buf_1[5]<<8);

    if(buf_2[0]& (1<<10))   //将负数转换成正数
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

//加速度传感器，一段时间内低于某值，自动睡眠，高于某值自动苏醒
/*
ADXL345是一种三轴加速度计。
具有+/-2g，+/-4g，+/-8g，+/-16g可变的测量范围；
最高13bit分辨率；
固定的4mg/LSB灵敏度；
3mm*5mm*1mm超小封装；
40-145uA超低功耗；
标准的 I2C或SPI数字接口；
32级FIFO存储；
以及内部多种运动状态检测和灵活的中断方式等特性
*/
void ADXL345_init_sleep_wakeup()
{
    unsigned char devid = 0, val = 0;
		u8 i=0;
		short offx=0,offy=0,offz=0;
		ADXL345_Data_t temp;
    delay_us(300);

    //I2C_ReadByte(ADXL345_ADDRESS, ADXL_R_ID, &devid);         //读ID   且每次读写之前都需要读ID
//		Device_ReadBytes( &devid,ADXL345_ADDRESS,ADXL_R_ID);
//    delay_us(300);

    /*

    寄存器0x31—DATA_FORMAT(读/写)
    D7 							D6 						D5 						D4 	D3 					D2 					D1 D0
    SELF_TEST 			SPI 					INT_INVERT 		0 	FULL_RES 		Justify 		Range
    自测力使能位			1 3线式spi		0中断高有效				1 13分辨率		0右对齐			范围
    								0	4线式spi		1中断低有效				0 10分辨率		1左对齐

    //-------------------------------------------------------
    0x2B
    0								0							0							1	 	1						0						1		1（16g）
    																																				1		0（8g）
    																																				0		1	（4g）
    																																				0		0	（2g）
    */
    //val = 0x1B;
    //I2C_WriteByte(ADXL345_ADDRESS, DATA_FORMAT, &val);  			//低电平中断输出,13位全分辨率,输出数据右对齐,16g量程
    Device_WriteBytes(0x1B,ADXL345_ADDRESS,DATA_FORMAT);
    delay_us(50);

    /*
    	寄存器0x2C—BW_RATE(读/写)
    	D7 	D6 	D5 	D4 										D3 	D2 	D1 		D0
    	0		0		0		LOW_POWER							(			速率		)
    							0 正常操作
    							1	低功耗(噪声有增加)
    	//-------------------------------------------------------
    	0		0		0		0											1010

    */
    //val = 0x0A;
    //I2C_WriteByte(ADXL345_ADDRESS, BW_RATE, &val);          		//数据输出速度为100Hz
    Device_WriteBytes(0x0A,ADXL345_ADDRESS,BW_RATE);
    delay_us(50);
    /*

    	寄存器0x2D—POWER_CTL(读/写)
    	D7 	D6	D5		D4 					D3 		D2 		D1 D0
    	0		0		链接	AUTO_SLEEP	测量	休眠	(休眠模式下的读取频率)


    	//-------------------------------------------------------
    	0		0		0			0						1			1			0		0（8HZ）

    */
    //val = 0x38;                                         				//自动休眠模式，休眠时以8HZ的频率采样
    //I2C_WriteByte(ADXL345_ADDRESS, POWER_CTL, &val);
    Device_WriteBytes(0x38,ADXL345_ADDRESS,POWER_CTL);
    delay_us(50);

    /*
    寄存器0x2E—INT_ENABLE(读/写)
    D7 					D6 				D5 					D4 				D3 					D2 				D1 				D0
    DATA_READY SINGLE_TAP DOUBLE_TAP 	Activity 	Inactivity 	FREE_FALL Watermark Overrun

    */
//		val = 0;
//    I2C_WriteByte(ADXL345_ADDRESS, INT_ENABLE, &val);       	//先关中断
    Device_WriteBytes(0,ADXL345_ADDRESS,INT_ENABLE);



    //无符号，比例因子62.5mg/LSB
//    val = 0x10;                                        					//睡眠激活activity阈值，当大于这个值的时候唤醒，其中10代表1g  16*62.5=1000mg
//    I2C_WriteByte(ADXL345_ADDRESS, THRESH_ACT, &val);
    Device_WriteBytes(0x0E,ADXL345_ADDRESS,THRESH_ACT);//0x12  18

//    val = 0x02;  // val = 0x10;                       	 				//睡眠开始inactivity阈值，当小于这个值的时候睡眠，其中02代表0.2g
//    I2C_WriteByte(ADXL345_ADDRESS, THRESH_INACT, &val);
    Device_WriteBytes(0x02,ADXL345_ADDRESS,THRESH_INACT);

    //比例因子为1 sec /LSB
//    val = 0x02;                                      						//当小于inactivity值时间超过这个值的时候进入睡眠，其中02代表2秒
//    I2C_WriteByte(ADXL345_ADDRESS, TIME_INACT, &val);
    Device_WriteBytes(0x05,ADXL345_ADDRESS,TIME_INACT);


//    val = 0xCC;                                          				//直流交流触发配置，XYZ使能触发配置，此处选用X交流触发

//    I2C_WriteByte(ADXL345_ADDRESS, ACT_INACT_CTL, &val);
    Device_WriteBytes(0xFF,ADXL345_ADDRESS,ACT_INACT_CTL);

    /*
    		D7 					D6 				D5 					D4 				D3 					D2 				D1 				D0
    		DATA_READY SINGLE_TAP DOUBLE_TAP 	Activity 	Inactivity 	FREE_FALL Watermark Overrun
    */
//    val = 0x10;                                      	 					//中断引脚选择，此处我们将activity映射到INT2
//    I2C_WriteByte(ADXL345_ADDRESS, INT_MAP, &val);
    Device_WriteBytes(0x10,ADXL345_ADDRESS,INT_MAP);
    delay_us(50);


//  val = 0xFF;                                         				//打开中断
//  I2C_WriteByte(ADXL345_ADDRESS, INT_ENABLE, &val);
		
    Device_WriteBytes(0x18,ADXL345_ADDRESS,INT_ENABLE);
    delay_ms(20);

    //软件偏移校准，视上电时为平放状态，x=0，y=0，z=1=====================

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


/*活动数据滤波冒泡
 *返回值：是去最大值和最小值再去取平均
 */
static ADXL345_Data_t Active_Bubble_Sort(ADXL345_Data_t  *data)
{
    u8 i = 0,j = 0;
    u16 temp = 0;
    ADXL345_Data_t ADX_Temp = {0};
    static ADXL345_Data_t ADX_Data = {0};
// X轴   5次采集的X轴值 冒泡排序
    for(i = 0 ; i < ( GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE ) ; i++) {
        for(j = 0; j <( GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE )-i-1 ; j++) {
            if((data+j)->x_axis > (data+j+1)->x_axis) {
                temp = (data+j+1)->x_axis;
                (data+j+1)->x_axis = (data+j)->x_axis;
                (data+j)->x_axis = temp;
            }
        }
    }
//Y轴
    for(i = 0 ; i < ( GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE ) ; i++) {
        for(j = 0; j <( GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE )-i-1 ; j++) {
            if((data+j)->y_axis > (data+j+1)->y_axis) {
                temp = (data+j+1)->y_axis;
                (data+j+1)->y_axis = (data+j)->y_axis;
                (data+j)->y_axis = temp;
            }
        }
    }
//Z轴
    for(i = 0 ; i < ( GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE ) ; i++) {
        for(j = 0; j <( GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE )-i-1 ; j++) {
            if((data+j)->z_axis > (data+j+1)->z_axis) {
                temp = (data+j+1)->z_axis;
                (data+j+1)->z_axis = (data+j)->z_axis;
                (data+j)->z_axis = temp;
            }
        }
    }
// X Y Z三轴取平均值
    temp = 0;
    for(i  = 1; i< (GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE)-1; i++) {
        temp += (data+i)->x_axis;
    }
    ADX_Temp.x_axis =  temp / (GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE - 2);//去掉了最小和最大
    //ADX_Temp.x_axis =  temp / (GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE);

    temp = 0;
    for(i  = 1; i< (GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE)-1; i++) {
        temp += (data+i)->y_axis;
    }
    ADX_Temp.y_axis =  temp / (GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE - 2);//去掉了最小和最大
    //ADX_Temp.y_axis =  temp / (GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE);
    temp = 0;
    for(i  = 1; i< (GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE)-1; i++) {
        temp += (data+i)->z_axis;
    }
    ADX_Temp.z_axis =  temp / (GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE - 2);//去掉了最小和最大
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
 *采集mpu数据,只有工作时才执行，获取1s中采集的数字
**/
void Collect_data(ADXL345_Data_t *Active_data,Environment_t * Envir_data,module_state* state)
{
    static u8 count = 0;
    static ADXL345_Data_t temp[GET_ADXLDATA_CYCLE / ADXL_SAMPLE_CYCLE] = {0}; //200ms采集一次，1分钟1000ms
		u8 intsource=0;

    if(state->run)  																					//运行状态下
    {
        if(Active_data->run_flag)															//是否测量体动
        {
            if(count >= GET_ADXLDATA_CYCLE/ADXL_SAMPLE_CYCLE)	//1000ms/200ms(duty)次时间到
            {
               // printf("数据处理前，运动值：X=  %hd \t Y=  %hd \t Z=  %hd      \n ",temp[0].x_axis,temp[0].y_axis,temp[0].z_axis);
                //测试PB10，11引脚------------

               // printf("中断引脚状况：INT1（PB10）:    %d  , INT2(PB11):    %d   \n\n",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10),GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11));
		
								Device_ReadBytes( &intsource,ADXL345_ADDRESS,INT_SOURCE);
								//printf("INT_SOURCE=== %b  \n\n",intsource);

                *Active_data = Active_Bubble_Sort(temp);  		//得到几次3轴的平均值
                count = 0;
                //printf("运动值：X=  %hd \t Y=  %hd \t Z=  %hd      \n ",Active_data->x_axis,Active_data->y_axis,Active_data->z_axis );

            }
            Get_ADXL345_data(&temp[count]);										//临时存在temp中
            count ++;                         								//第几次计数值要保存
            Active_data->run_flag = 0;
						//printf("R");

        }
        if(Envir_data->Light_run_flag)
        {
            //L_Get_Value(AD_Value,&Environment_data.Light_data);
            Environment_data.Light_data = get_data_BH1750(); 	//此时模块应该已经关闭了 （测量时间120ms ）
            Envir_data->Light_run_flag = 0;

            //printf("光强值： %.3f     ",Environment_data.Light_data);
						//printf("L");
            BH1750_init(); 																		//重新启动 模块，模块测量完之后，会自动关闭
        }
        if(Envir_data->Tempet_run_flag)         							//温度采集
        {
            //DMA搬运adc采集的数据  到AD_Value地址所在的内存中
            T_Get_Value(AD_Value,&Environment_data.Temperature_data);
            Envir_data->Tempet_run_flag = 0;
            //printf("温度值： %d       ",Environment_data.Temperature_data);
						//printf("T");
        }
        if(Envir_data->Voice_run_flag)        							 	//声音采集
        {
            V_Get_Value(AD_Value,&Environment_data.Voice_data);
            Envir_data->Voice_run_flag = 0;
            //printf("音量值： %d  ",Environment_data.Voice_data);
						//printf("V");
        }
        if(Envir_data->Power_check_flag )
        {
            P_Get_Value(AD_Value,&Environment_data.Power_data);
            //printf("电量值： %d  \n",Environment_data.Power_data);
            Envir_data->Power_check_flag  = 0;
						//printf("P");
        }
    }
}


/******************endif***************************/
