#ifndef OBTAINDEVICEMEASUREDATA_H
#define OBTAINDEVICEMEASUREDATA_H
#include <QString>
#include <QObject>
#include <QWidget>
/*
类描述：功能类，从设备复制文件到本地PC
关键接口：
        无
*/
class ObtainDeviceMeasureData: public QObject
{
    Q_OBJECT
public:
    QString SourceDir;                      //源文件夹（设备地址G:/ACtive_Data/）
    QString TODir;                          //目标地址（D:/Active_Data/XXX/）
    QWidget *thisparent;                    //父类指针
    ObtainDeviceMeasureData(const QString& UserName,QWidget* parent=0);
    ~ObtainDeviceMeasureData();
    void start();                           //开始复制
public slots:
    void startCOPYFileFromDevice();         //把设备中的数据复制到 指定的用户目录下
signals:
    void work_over();                       //工作结束，通知主线程关闭次线程和自己
    void work_message(QString text);        //发送消息给主线程
    void work_Progress(qint64 progress);    //发送进度数值
    void work_TotalSize(qint64);
private://---------------------------------------------------------------------
    qint64 getDirSize(const QString &targetDir);
    bool deleteDir(const QString &delPath);                                                         //删除包括自己在内的所有文件夹，子文件夹和文件
    bool deleteDirExceptDelPath(const QString &delPath, int Zero);                                  //删除子文件夹和文件
    bool copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist);                   //复制文件
    bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist);   //复制文件夹
};

#endif // OBTAINDEVICEMEASUREDATA_H
