#ifndef OPENFILEGETDATA_H
#define OPENFILEGETDATA_H

#include <QObject>
#include <QDialog>
#include "openfilegetdata_proxy.h"
#include <QPushButton>
#include <QFileInfo>
/*
类描述：功能类，打开文件，存放文件所有数据到内存
关键接口：
        无
*/
class OpenFileGetData : public QDialog
{
    Q_OBJECT
public:
    explicit OpenFileGetData(const QString& UserName,QWidget *parent = 0);
    ~OpenFileGetData();
    OpenFileGetData_Proxy* tableProxy;  //表代理

    QString NNNName;                    //对象名字
    QPushButton* btn_Sure;              //确认按钮
    int checked_num;                    //勾选的文件夹个数
    QStringList strlist_CheckedFileName;//存放 被勾选的文件夹名称
signals:
    void signal_strlistCheckFiles(const QFileInfoList& strlist);
    void signal_strlistCheckFiles(const QStringList& strlist);
public slots:
    void startOpenFile();               //打开文件（得到文件列表）
    void slot_tableclick(QModelIndex index);    //表格点击
    void slot_btn_sure();               //确认选择
};

#endif // OPENFILEGETDATA_H
