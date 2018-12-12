#include "openfilegetdata_proxy.h"

OpenFileGetData_Proxy::OpenFileGetData_Proxy(QWidget *parent):QWidget(parent)
{
    init_tableview();
}

void OpenFileGetData_Proxy::insertData(const QString &filename, double sleeptime, double efficiency, const QString &remarks)
{
    int countrow = model->rowCount();
    QStandardItem* item1=new QStandardItem;
    item1->setText(filename);
    model->setItem(countrow,1,item1);

    QStandardItem* item2=new QStandardItem;
    item2->setText(QString::number(sleeptime,'f',1)+"小时");
    model->setItem(countrow,2,item2);

    QStandardItem* item3=new QStandardItem;
    item3->setText(QString::number(efficiency,'f',1)+"%");
    model->setItem(countrow,3,item3);

    QStandardItem* item4=new QStandardItem;
    item4->setText(remarks);
    model->setItem(countrow,4,item4);
}

void OpenFileGetData_Proxy::init_tableview()
{
    tableview = new QTableView(this);
    tableview->setFixedSize(600,460);

    model = new QStandardItemModel;
    model->setColumnCount(5);
    //model->setRowCount(1024);
    tableview->setModel(model);
    tableview->setColumnWidth(0,40);   //勾选
    tableview->setColumnWidth(1,120);   //文件夹名称 2018-06-26
    tableview->setColumnWidth(2,100);   //睡眠时长
    tableview->setColumnWidth(3,100);   //睡眠效率
    tableview->setColumnWidth(4,195);   //备注信息
    model->setHeaderData(0,Qt::Horizontal,"勾选");
    model->setHeaderData(1,Qt::Horizontal,"日期");
    model->setHeaderData(2,Qt::Horizontal,"睡眠时长");
    model->setHeaderData(3,Qt::Horizontal,"睡眠效率");
    model->setHeaderData(4,Qt::Horizontal,"备注信息");
    //---使用代理---------------
    checkboxDelegate = new OpenFileGetData_Delegate(this);
    tableview->setItemDelegateForColumn(0,checkboxDelegate);

}
