#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include <QObject>
#include <QString>
#include <QVector>

#include "graphplot/markframe.h"
#define DUAN_BORDER 18

/*
类描述：易失内存数据，主要用于存储数据，方便全局调用
关键接口：
    略
*/
class DataContainer: public QObject
{
    Q_OBJECT

public:
    static DataContainer* getInstance();//得到对象
    void releaseContainers();           //主动删除内存空间
    ~DataContainer();                   //析构
    //数据容器
    //-----------
    int ChooseNUM;                      //勾选的文件夹个数
    int MarkBarWidth;                   //子类使用的界面宽度信息
    int KFlag_PDF;
    QString DevicePath;                 //设备地址
    QString ID;                         //儿童或观测对象的ID号
    QString PatientName;                //儿童或观测对象的姓名
    QString Gander;                     //儿童或观测对象的性别
    QString BrithDay;                   //儿童或观测对象的生日
    QString Age;                        //儿童或观测对象的年龄
    QString WatcherName;                //儿童或观测对象的监护人
    QString WatcherPhone;               //儿童或观测对象的监护人联系方式
    QString WatcherAdress;              //儿童或观测对象的监护人地址
    //-----------
    QString DoctorTips1;            //医师建议：第一天建议
    QString DoctorTips2;            //医师建议：第二天建议
    QString DoctorTips3;            //医师建议：第三天建议
    QString DoctorTips4;            //医师建议：第四天建议
    QString DoctorTips5;            //医师建议：第五天建议
    QString DoctorTips6;            //医师建议：第六天建议

    //六天的日期
    QString day1;                   //文件夹名称1
    QString day2;
    QString day3;
    QString day4;
    QString day5;
    QString day6;

    //报告数据
    QStringList strlist;

    //设备原始数据
    //六天所有的活动数据86400
    QVector<double> activeList86400_Day1;
    QVector<double> activeList86400_Day2;
    QVector<double> activeList86400_Day3;
    QVector<double> activeList86400_Day4;
    QVector<double> activeList86400_Day5;
    QVector<double> activeList86400_Day6;
    //六天所有的光线数据
    QVector<double> lightList86400_Day1;
    QVector<double> lightList86400_Day2;
    QVector<double> lightList86400_Day3;
    QVector<double> lightList86400_Day4;
    QVector<double> lightList86400_Day5;
    QVector<double> lightList86400_Day6;
    //六天所有的声音数据
    QVector<double> voiceList86400_Day1;
    QVector<double> voiceList86400_Day2;
    QVector<double> voiceList86400_Day3;
    QVector<double> voiceList86400_Day4;
    QVector<double> voiceList86400_Day5;
    QVector<double> voiceList86400_Day6;

    //绘图数据30s 2880
    //画图所需的活动数组
    QVector<double> activeList2880_Day1;
    QVector<double> activeList2880_Day2;
    QVector<double> activeList2880_Day3;
    QVector<double> activeList2880_Day4;
    QVector<double> activeList2880_Day5;
    QVector<double> activeList2880_Day6;

    //画图所需的光强数组
    QVector<double> lightList2880_Day1;
    QVector<double> lightList2880_Day2;
    QVector<double> lightList2880_Day3;
    QVector<double> lightList2880_Day4;
    QVector<double> lightList2880_Day5;
    QVector<double> lightList2880_Day6;

    //画图所需的声音数组
    QVector<double> voiceList2880_Day1;
    QVector<double> voiceList2880_Day2;
    QVector<double> voiceList2880_Day3;
    QVector<double> voiceList2880_Day4;
    QVector<double> voiceList2880_Day5;
    QVector<double> voiceList2880_Day6;

    //分析用数据10min 144
    //分析所需的活动数组
    QVector<double> activeList144_Day1;
    QVector<double> activeList144_Day2;
    QVector<double> activeList144_Day3;
    QVector<double> activeList144_Day4;
    QVector<double> activeList144_Day5;
    QVector<double> activeList144_Day6;

    //分析所需的光强数组
    QVector<double> lightList144_Day1;
    QVector<double> lightList144_Day2;
    QVector<double> lightList144_Day3;
    QVector<double> lightList144_Day4;
    QVector<double> lightList144_Day5;
    QVector<double> lightList144_Day6;

    //分析所需的声音数组
    QVector<double> voiceList144_Day1;
    QVector<double> voiceList144_Day2;
    QVector<double> voiceList144_Day3;
    QVector<double> voiceList144_Day4;
    QVector<double> voiceList144_Day5;
    QVector<double> voiceList144_Day6;

    //绘图分析所需深睡144-----------------
    QVector<double> deepSleep144_Day1;
    QVector<double> deepSleep144_Day2;
    QVector<double> deepSleep144_Day3;
    QVector<double> deepSleep144_Day4;
    QVector<double> deepSleep144_Day5;
    QVector<double> deepSleep144_Day6;
    //绘图分析所需浅睡144-----------------
    QVector<double> shallowSleep144_Day1;
    QVector<double> shallowSleep144_Day2;
    QVector<double> shallowSleep144_Day3;
    QVector<double> shallowSleep144_Day4;
    QVector<double> shallowSleep144_Day5;
    QVector<double> shallowSleep144_Day6;

    //处理之后的144数组等间距梯度图
    QVector<double> AnalysisResult144_Day1;
    QVector<double> AnalysisResult144_Day2;
    QVector<double> AnalysisResult144_Day3;
    QVector<double> AnalysisResult144_Day4;
    QVector<double> AnalysisResult144_Day5;
    QVector<double> AnalysisResult144_Day6;


    //辅助标记数据
    //活动
    QVector<struct_Duan> DUAN1 ;
    QVector<struct_Duan> DUAN2 ;
    QVector<struct_Duan> DUAN3 ;
    QVector<struct_Duan> DUAN4 ;
    QVector<struct_Duan> DUAN5 ;
    QVector<struct_Duan> DUAN6 ;
    //睡眠（由活动得到）
    QVector<struct_Duan> SUAN1 ;
    QVector<struct_Duan> SUAN2 ;
    QVector<struct_Duan> SUAN3 ;
    QVector<struct_Duan> SUAN4 ;
    QVector<struct_Duan> SUAN5 ;
    QVector<struct_Duan> SUAN6 ;
public slots:

private:
    static DataContainer* instance;
    explicit DataContainer(QObject *parent = 0);
};

#endif // DATACONTAINER_H
