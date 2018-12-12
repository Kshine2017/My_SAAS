#ifndef READFILEDATA_H
#define READFILEDATA_H

#include <QObject>
#include <QFileInfo>
#include "DataContainer/datacontainer.h"
#include "DataContainer/flagcontainer.h"
/*
类描述：功能类，读取文件中的数据
关键接口：
        无
*/
class ReadFileData : public QObject
{
    Q_OBJECT
public:

    explicit ReadFileData(QObject *parent = 0);
    DataContainer* dc;
    FlagContainer* fc;
    int getTotalFileNum(const QFileInfoList &fileinfolist); //获取总文件个数
signals:
    void work_over();                   //工作结束，通知主线程关闭次线程和自己
    void work_message(QString text);    //发送消息给主线程
    void work_Progress(qint64 progress);//发送进度数值
    void work_Progress_int(int num);    //文件个数
    void work_TotalSize(qint64);        //总文件大小
    void work_TotalNumber(int);         //总文件个数
public slots:
    void startReadFileData(const QStringList &strlist);//开始读取数据

};

#endif // READFILEDATA_H
