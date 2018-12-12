#include "openfilegetdata.h"
#include <QDebug>
#include <QDir>
#include <QMessageBox>
OpenFileGetData::OpenFileGetData(const QString &UserName, QWidget *parent) : QDialog(parent)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowModality(Qt::ApplicationModal);
    this->setFixedSize(600,500);
    this->setWindowTitle(UserName);
    NNNName = UserName;
    checked_num=0;
    strlist_CheckedFileName.clear();
    btn_Sure = new QPushButton(this);
    btn_Sure->setGeometry(250,465,100,30);
    btn_Sure->setFont(QFont("楷体",18,75));
    btn_Sure->setText("确认打开");
    connect(btn_Sure,SIGNAL(clicked()),this,SLOT(slot_btn_sure()));

    tableProxy = new OpenFileGetData_Proxy(this);
    connect(tableProxy->tableview,SIGNAL(clicked(QModelIndex)),
            this,SLOT(slot_tableclick(QModelIndex)));
    //tableProxy->insertData("2017-01-02",8.7,52.23,"睡觉睡得很香");
    startOpenFile();
    show();
}

OpenFileGetData::~OpenFileGetData()
{
    qDebug()<<"析构---打开文件";
}

void OpenFileGetData::startOpenFile()
{
    //QString pathStr = "d:\\Active_Data\\"+NNNName;
    QString pathStr = "./Active_Data/"+NNNName;
    QDir *dir = new QDir(pathStr);
    if(!dir->exists())
    {
        //QMessageBox::information(this,"提示","请先上传设备文件",QMessageBox::Ok);
        return;
    }
    //dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
    dir->sorting();
    QFileInfoList allFiles = dir->entryInfoList();
    QStringList filesnamelist;
    foreach (QFileInfo file, allFiles)
    {

            if(file.fileName()!="." && file.fileName()!="..")
            {
                if(file.isDir())//是文件夹
                {
                    filesnamelist.append(file.fileName());
                }
            }
    }
    //排序
    for(int i=0;i<filesnamelist.count()-1;i++)
    {
        for(int j=0;j<filesnamelist.count()-i-1;j++)
        {
            if(filesnamelist[j]<filesnamelist[j+1])
            {
                QString tem=filesnamelist[j];
                filesnamelist[j]=filesnamelist[j+1];
                filesnamelist[j+1]=tem;
            }
        }
    }
    for(int i=0;i<filesnamelist.count();i++)
        tableProxy->insertData(filesnamelist[i],0.0,0.0,"未分析");

}

void OpenFileGetData::slot_tableclick(QModelIndex index)
{
    if(index.column()==0)//checkbox
    {
        if(index.data().toBool()==true)
        {
            checked_num++;
            if(checked_num>6)
            {
                qDebug()<<"最多可以勾选6天的文件！";
                QMessageBox::information(this,"提示","最多可以勾选6天的文件！",QMessageBox::Ok);
                checked_num--;
                //将该checkbox变回unchecked
                tableProxy->tableview->model()->setData(index,0);
//                strlist_CheckedFileName.pop_back();

            }
            else //未超过6天文件
            {
                QString tem =tableProxy->tableview->model()->index(index.row(),1).data().toString();
                strlist_CheckedFileName.append(tem);
            }
        }
        else
        {
            checked_num--;
            QString tem =tableProxy->tableview->model()->index(index.row(),1).data().toString();
            for(int i=0;i<strlist_CheckedFileName.length();i++)
            {
                if(strlist_CheckedFileName[i] == tem)
                {
                    strlist_CheckedFileName.removeAt(i);
                }
            }
        }
    }
    if(checked_num<0)checked_num=0;

    qDebug()<<"已勾选："<<checked_num<<"个";
    qDebug()<<strlist_CheckedFileName;

}

void OpenFileGetData::slot_btn_sure()
{
    if(strlist_CheckedFileName.length()==0)
    {
        close();
        return;
    }

#if 0
    //将stringlist 装换成fileinfolist
    QFileInfoList  fileinfolist;
    QString pathStr = "d:\\Active_Data\\"+NNNName+"\\";
    for(int i=0;i<strlist_CheckedFileName.length();i++)
    {
        QFileInfo tem(pathStr+strlist_CheckedFileName[i]);
        fileinfolist.append(tem);
    }
        qDebug()<<"发送信息："<<fileinfolist[0].fileName();
        emit signal_strlistCheckFiles(fileinfolist);
#else
    qDebug()<<"发送信息："<<strlist_CheckedFileName;
    emit signal_strlistCheckFiles(strlist_CheckedFileName);
#endif
    close();
}
/*
//整数或浮点数皆可使用
void bubble_sort(T arr[], int len)
{
    int i, j;  T temp;
    for (i = 0; i < len - 1; i++)
        for (j = 0; j < len - 1 - i; j++)
        if (arr[j] > arr[j + 1])
        {
            temp = arr[j];
            arr[j] = arr[j + 1];
            arr[j + 1] = temp;
        }
}
*/
