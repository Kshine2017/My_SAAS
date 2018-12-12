#ifndef OPENFILEGETDATA_PROXY_H
#define OPENFILEGETDATA_PROXY_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include "opration/openfilegetdata_delegate.h"
/*
类描述：功能类，使用Checkbox代理的tableview
关键接口：
        无
*/

class OpenFileGetData_Proxy: public QWidget
{
    Q_OBJECT
public:
    OpenFileGetData_Proxy(QWidget* parent=0);
    QTableView* tableview;                          //表格
    QStandardItemModel *model;                      //表格数据
    OpenFileGetData_Delegate* checkboxDelegate;     //checkbox代理

    void insertData(const QString& filename,double sleeptime,double efficiency,const QString& remarks);//插入数据
    void init_tableview();                          //初始化表格

};

#endif // OPENFILEGETDATA_PROXY_H
