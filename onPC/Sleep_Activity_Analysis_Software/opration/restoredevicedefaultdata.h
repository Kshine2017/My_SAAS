#ifndef RESTOREDEVICEDEFAULTDATA_H
#define RESTOREDEVICEDEFAULTDATA_H
#include <QString>
#include <QObject>
#include <QWidget>
/*
类描述：功能类，将设备恢复到其默认数据
关键接口：
        无
*/

class RestoreDeviceDefaultData : public QObject
{
    Q_OBJECT
public:
    RestoreDeviceDefaultData(QWidget* parent=0);
    QString SourceDir;//源文件夹（安装地址下）
    QString TODir;//目标地址（设备地址）
    void start();
public slots:
    void startCOPYFileinSecondDir(); //在二级子文件夹下直接覆盖空文件(通过外部信号调用)
signals:
    void work_over();//工作结束，通知主线程关闭次线程和自己
    void work_message(QString text);//发送消息给主线程
    void work_Progress(qint64 progress);//发送进度数值
private://---------------------------------------------------------------------
    QWidget *thisparent;
    void startDeleteDeviceData();//开始删除设备数据
    void startWriteInEmptyFiles();//开始写入空文件

    //--------------------------------------------------------------------------
    bool deleteDir(const QString &delPath);//删除包括自己在内的所有文件夹，子文件夹和文件
    bool deleteDirExceptDelPath(const QString &delPath, int Zero);//删除子文件夹和文件
    bool copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist);//复制文件
    bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist);//复制文件夹
};

#endif // RESTOREDEVICEDEFAULTDATA_H
