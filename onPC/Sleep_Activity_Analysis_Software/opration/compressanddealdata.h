#ifndef COMPRESSANDDEALDATA_H
#define COMPRESSANDDEALDATA_H

#include <QObject>
#include <QVector>
#include "DataContainer/datacontainer.h"
#include "DataContainer/flagcontainer.h"

/*
类描述：功能类，压缩和处理（对原始数据进行处理，得到简化数据）
关键接口：
        无
*/

class CompressAndDealData : public QObject
{
    Q_OBJECT
public:
    explicit CompressAndDealData(QObject *parent = 0);
    DataContainer* dc;                  //数据源
    FlagContainer* fc;                  //数据源
    int CheckNum;
signals:
    void work_over();                   //工作结束，通知主线程关闭次线程和自己
    void work_message(QString text);    //发送消息给主线程
    void work_Progress(qint64 progress);//发送进度数值
    void work_Progress_int(int num);    //
    void work_TotalSize(qint64);        //总数据大小
    void work_TotalNumber(int);         //总文件个数
public slots:
    void start();                       //开始工作
private:
    void start_dealAllFlag(int DAYNUM);
    void start_getDrawData();           //得到画图用的2880压缩数组（30s一个点）
    void start_getAnalyzeData();        //得到分析用的144压缩数组(10min一个点)
    int tem[86400];
};

#endif // COMPRESSANDDEALDATA_H
