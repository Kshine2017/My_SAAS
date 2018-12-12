#ifndef DATAANALYZE_H
#define DATAANALYZE_H

#include <QObject>
#include <QVector>
#include "DataContainer/datacontainer.h"


/*
类描述：功能类，从6个活动容器（144），分析出 深度睡眠和浅度睡眠等 关键性 睡眠指标数值
关键接口：
        无
*/
class DataAnalyze : public QObject
{
    Q_OBJECT
public:
    explicit DataAnalyze(QObject *parent = 0);
    DataContainer* dc;
    int chknum;
    int Markbarwidth;
signals:
    void work_over();                       //工作结束，通知主线程关闭次线程和自己
    void work_message(QString text);        //发送消息给主线程
    void work_Progress(qint64 progress);    //发送进度数值
    void work_Progress_int(int num);        //发送文件单次个数
    void work_TotalSize(qint64);            //发送数据总大小
    void work_TotalNumber(int);             //发送文件总个数
private:
    void getData_FuzzificationTable();      //得到模糊数据
    void getData_Remove_Noise_AfterFT();    //20分钟去杂，标准化
    void getData_Divide_Status_AfterRN();   //三值化
    void getData_Remove_Noise_AfterDS();    //30分钟去杂，标准化
    void getData_Remove_Noise_AfterDSRN();  //40分钟去杂
    void getData_Divide_SleepActive_AfterDS();//区别睡觉和活动的时间域
    void getData_DUANs();                   //自动标记的数据
    void getMarkData_FromDeepAndShalow();   //得到标记数据
public slots:
    void startAnalyze();                    //开始分析

};

#endif // DATAANALYZE_H
