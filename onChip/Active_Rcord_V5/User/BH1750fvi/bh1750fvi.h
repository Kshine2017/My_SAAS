#ifndef __BH1750_H
#define __BH1750_H


#include "machine_control.h"
#include "file.h"
#include "Environment.h"
#define BH1750_ADDRESS 0x46     //������ַ

/*
������ַ˵����
����BH1750FVI-�����ֲᣨ��10ҳ��������
������ַ��2����ʽ����ADDR�˿ھ�����
ADDR = "H" (ADDR �� 0.7VCC) -> "1011 100" ->  0b10111000 -> 0xB8
ADDR = "L"��ADDR �� 0.3VCC��-> "0100 011" ->  0b01000110  -> 0x46
*/

//typedef struct BH_data  											//BH1750��������
//{   
//    short int data;
//    volatile unsigned char run_flag;          //BH1750���ݲɼ���־
//} BH1750_Data_t;


////��� ȡ��  ʱ����
//#define   BH_SAMPLE_CYCLE  120   							//ÿ120msȡһ������
//#define   GET_BHDATA_CYCLE 1000  							//ʵ����Ҫ�õ���������1000ms



/*
BH1750FIV������ģʽ��Ϊ�����ͺ�һ���ͣ�

�������ַ�Ϊ�߾���1��2�͵;���ģʽ��

������ͬ����Ϊ�߾���1��2�͵;���ģʽ��

�������������͵��������ڵ����Ͷ���һ�����ݾͻ��Զ�תΪPowerDownģʽ��
*/
#define BHAddWrite     0x46      //�ӻ���ַ+���д����λ    ���ø߾���������ģʽ�ȷ���IIC��ʼ�ź��ٷ���7λ������ַ+1λд��־λҲ����0X46
#define BHAddRead      0x47      //�ӻ���ַ+��������λ  	 ���ø߾���������ģʽ�ȷ���IIC��ʼ�ź��ٷ���7λ������ַ+1λ����־λҲ����0X47
#define BHPowDown      0x00      //�ر�ģ��  
#define BHPowOn        0x01      //��ģ��ȴ�����ָ��  
#define BHReset        0x07      //�������ݼĴ���ֵ��PowerOnģʽ����Ч  
#define BHModeH1       0x10      //�߷ֱ��� ��λ1lx ����ʱ��120ms  
#define BHModeH2       0x11      //�߷ֱ���ģʽ2 ��λ0.5lx ����ʱ��120ms  
#define BHModeL        0x13      //�ͷֱ��� ��λ4lx ����ʱ��16ms  
#define BHSigModeH     0x20      //һ�θ߷ֱ��� ���� ������ģ��ת�� PowerDownģʽ  
#define BHSigModeH2    0x21      //ͬ������  
#define BHSigModeL     0x23      // ������


//extern BH1750_Data_t BH1750_struct_Data;
void   		 BH1750_init();
float   get_data_BH1750();
uint8_t Check_BH1750();






#endif

