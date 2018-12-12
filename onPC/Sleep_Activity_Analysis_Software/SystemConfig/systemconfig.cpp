#include "systemconfig.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileInfoList>
#include <QDir>
#include <QFile>
#include <minwindef.h>
#include <fileapi.h>
#include <winbase.h>
#include <QProcess>
#include "DataContainer/datacontainer.h"
#include <QDebug>
#include <QMessageBox>
#include "opration/restoredevicedefaultdata.h"
#include "opration/qwaitdialog.h"
#include <QThread>
#include "managetipsinfo.h"
#include "usartcommunication.h"
SystemConfig::SystemConfig(QWidget *parent, int WWW, int HHH) : QWidget(parent)
{
    parentthis=parent;
    if(WWW&&HHH)
    {
        //创建时指定大小
        thisWidth=WWW;
        thisHeight=HHH;
    }
    else
    {
        //默认子页面大小
        thisWidth=1920;
        thisHeight=1080;
        this->setStyleSheet("background-color:yellow;");
    }
    init_widgetDevice();
    init_widgetSoftWare();
    init_widgetPersonalInfo();
    init_PageTopInfo();
    init_PageSecInfo();

}

void SystemConfig::init_PageTopInfo()
{
    pageName =new  QLabel(this);
    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor(92,123,137));
    pageName->setPalette(pa);
    pageName->setFont(QFont("楷体",22,75));
    pageName->setText("系统设置");


}

void SystemConfig::init_PageSecInfo()
{
    tabwidget = new QTabWidget(this);
    tabwidget->setFixedSize(thisWidth,thisHeight-40);
    QIcon icon1(":/picture/huangzhijie/硬件管理.png");
    QIcon icon2(":/picture/huangzhijie/软件管理.png");
    QIcon icon3(":/picture/huangzhijie/信息管理.png");
    tabwidget->addTab(widgetDevice,icon1,tr("硬件管理"));
    tabwidget->addTab(widgetSoftware,icon2,tr("软件管理"));
    tabwidget->addTab(widgetPersonalInfo,icon3,tr("信息管理"));
    QHBoxLayout* ly_H_Top = new QHBoxLayout();
    ly_H_Top->addSpacing(10);
    ly_H_Top->addWidget(pageName);
    ly_H_Top->addStretch();
    ly_H_Top->addSpacing(10);

    QVBoxLayout* ly_V_all = new QVBoxLayout;
    ly_V_all->addSpacing(10);
    ly_V_all->addLayout(ly_H_Top);
    QLabel *m_label = new QLabel(this);
    m_label->setFrameShape(QFrame::HLine);
    m_label->setFrameShadow(QFrame::Raised);
    m_label->setMaximumSize(QSize(6000, 1));
    ly_V_all->addWidget(m_label);

    ly_V_all->addWidget(tabwidget);
    ly_V_all->addStretch();

    //this->setLayout(ly_V_all);
    ly_V_all->setGeometry(QRect(0,0,thisWidth,thisHeight));

}

void SystemConfig::init_widgetDevice()
{
    widgetDevice = new QWidget();
    btn_CheckDeviceStatus           = new QPushButton(widgetDevice);
    btn_ReadLogFile                 = new QPushButton(widgetDevice);
    btn_RestoreDeviceDefaultData    = new QPushButton(widgetDevice);
    btn_FormatDevice                = new QPushButton(widgetDevice);
    btn_DeviceParameters            = new QPushButton(widgetDevice);
    textedit                        = new QTextEdit(widgetDevice);

    btn_CheckDeviceStatus->setText("检查设备连接状态");btn_CheckDeviceStatus->setFixedSize(200,40);
    btn_ReadLogFile->setText("读取设备Log文件");btn_ReadLogFile->setFixedSize(200,40);
    btn_RestoreDeviceDefaultData->setText("还原设备数据文件");btn_RestoreDeviceDefaultData->setFixedSize(200,40);
    btn_FormatDevice->setText("格式化设备");btn_FormatDevice->setFixedSize(200,40);
    btn_DeviceParameters->setText("设备参数");btn_DeviceParameters->setFixedSize(200,40);
    textedit->setFixedSize(thisWidth-220,thisHeight-80);

    connect(btn_CheckDeviceStatus,SIGNAL(clicked(bool)),this,SLOT(slot_btn_checkdevicestatus()));
    connect(btn_FormatDevice,SIGNAL(clicked(bool)),this,SLOT(slot_btn_fomatDevice()));
    connect(btn_RestoreDeviceDefaultData,SIGNAL(clicked(bool)),this,SLOT(slot_btn_restoreDeviceData()));
    connect(btn_ReadLogFile,SIGNAL(clicked(bool)),this,SLOT(slot_btn_readlogfile()));
    connect(btn_DeviceParameters,SIGNAL(clicked(bool)),this,SLOT(slot_btn_usart()));

    QVBoxLayout* ly_V_btn = new QVBoxLayout;
    ly_V_btn->addSpacing(10);
    ly_V_btn->addWidget(btn_CheckDeviceStatus);
    ly_V_btn->addWidget(btn_ReadLogFile);
    ly_V_btn->addWidget(btn_RestoreDeviceDefaultData);
    ly_V_btn->addWidget(btn_FormatDevice);
    ly_V_btn->addWidget(btn_DeviceParameters);
    ly_V_btn->addStretch();
    ly_V_btn->setGeometry(QRect(0,0,200,thisHeight-40));
    textedit->setGeometry(205,10,thisWidth-210,thisHeight-60);
    //widgetDevice->setLayout(ly_V_btn);
}

void SystemConfig::init_widgetSoftWare()
{
    widgetSoftware = new QWidget();
    btn_SetPageStytle           = new QPushButton(widgetSoftware);


    btn_SetPageStytle->setText("设置页面风格");btn_SetPageStytle->setFixedSize(200,40);


    btn_SetTipsInfo          = new QPushButton(widgetSoftware);
    btn_SetTipsInfo->setText("管理建议文字");btn_SetTipsInfo->setFixedSize(200,40);
    connect(btn_SetTipsInfo,SIGNAL(clicked(bool)),this,SLOT(slot_btn_manageTipsInfo()));


    QVBoxLayout* ly_V_btn = new QVBoxLayout;
    ly_V_btn->addSpacing(10);
    ly_V_btn->addWidget(btn_SetPageStytle);
    ly_V_btn->addWidget(btn_SetTipsInfo);
    ly_V_btn->addStretch();
    widgetSoftware->setLayout(ly_V_btn);

}

void SystemConfig::init_widgetPersonalInfo()
{
    widgetPersonalInfo = new QWidget();
    btn_SetLoginInfo           = new QPushButton(widgetPersonalInfo);


    btn_SetLoginInfo->setText("设置登录信息");btn_SetLoginInfo->setFixedSize(200,40);


    QVBoxLayout* ly_V_btn = new QVBoxLayout;
    ly_V_btn->addSpacing(10);
    ly_V_btn->addWidget(btn_SetLoginInfo);

    ly_V_btn->addStretch();

    widgetPersonalInfo->setLayout(ly_V_btn);
}

void SystemConfig::slot_ShowMesageFromThread(QString text)
{
    //QMessageBox::information(this,"来自线程的提示",text,QMessageBox::Ok);
    textedit->append(text);
}

void SystemConfig::slot_SowWorkProgress(qint64 progress)
{
    Q_UNUSED(progress)
}

void SystemConfig::slot_btn_checkdevicestatus()
{
    textedit->append("[操作] - 检查设备连接状态");
    QFileInfoList list =  QDir::drives();  //获取当前系统的盘符
    bool bUsb = false;
    QStringList RemovableDeviceList;
    //QString deviceDisk;

    for(int i=0;i<list.count();i++)
    {
        UINT ret = GetDriveType((WCHAR *) list[i].filePath().utf16());
        if(ret == DRIVE_REMOVABLE)
        {
            //deviceDisk=list[i].filePath();
            RemovableDeviceList.append(list[i].filePath());
            //qDebug()<<deviceDisk<<"--->可移动盘";//"F:/"
            bUsb = true;
        }
    }
    if(!bUsb)
    {
        QMessageBox::warning(this,"重要提示：",QStringLiteral("未检测到设备！\n(1)请检查是否插入设备！\n(2)请检查是否开启设备电源!\n(3)请检查连接线是否插牢！\n(4)请在必要时联系技术人员！"),QMessageBox::Yes);
        textedit->append("[信息] - 未检测到设备！\n");
        return;
    }

    for(int j=0;j<RemovableDeviceList.length();j++)
    {
        QString lpRootPathName = RemovableDeviceList[j];//"C:\\" 此处使用"F:/" 格式也可以
        LPTSTR lpVolumeNameBuffer=new TCHAR[12];//磁盘卷标
        DWORD nVolumeNameSize=12;// 卷标的字符串长度
        DWORD VolumeSerialNumber;//硬盘序列号
        DWORD MaximumComponentLength;// 最大的文件长度
        LPTSTR lpFileSystemNameBuffer=new TCHAR[10];// 存储所在盘符的分区类型的长指针变量
        DWORD nFileSystemNameSize=10;// 分区类型的长指针变量所指向的字符串长度
        DWORD FileSystemFlags;// 文件系统的一此标志
        GetVolumeInformation(
                    (LPTSTR)lpRootPathName.utf16(),
                    lpVolumeNameBuffer,
                    nVolumeNameSize,
                    &VolumeSerialNumber,
                    &MaximumComponentLength,
                    &FileSystemFlags,
                    lpFileSystemNameBuffer,
                    nFileSystemNameSize
                    );
        QString diskname=QString::fromWCharArray(lpVolumeNameBuffer);
        textedit->append(" - 找到移动设备:"+lpRootPathName+diskname);
        if(diskname=="BB-CARE")//体动机器磁盘名称
        {
            QString tem=  lpRootPathName;
            DeviceDiskNumber=tem.left(2);
            QMessageBox::information(this,"成功","找到设备所在盘符:  "+tem,QMessageBox::Yes);
            textedit->append("[成功] - 找到设备所在盘符:"+tem);
            textedit->append(" ");
            return;
        }
    }
    textedit->append("[信息] - 未检测到设备！\n");
    QMessageBox::warning(this,"重要提示",QStringLiteral("未检测到设备！"),QMessageBox::Yes);
    return;
}
void SystemConfig::slot_btn_fomatDevice()
{



    textedit->append("[操作] - 格式化设备");

    if(DeviceDiskNumber=="")
    {
        textedit->append(" - 未找到设备，请检查设备状态！");
        return;
    }
    if(QMessageBox::question(this,"三思","确定要格式化设备:"+DeviceDiskNumber+" 吗？", QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes) == QMessageBox::No)
    {
        textedit->append("取消操作！");
        return;
    }
    textedit->append("[信息] - 如下：");
    textedit->append("--------------------------");
    QString program = "cmd.exe";
    QStringList arguments;
    arguments << "/c"<< QString("format ") + DeviceDiskNumber +QString(" /FS:FAT /Q /Y");

    QProcess p;
    p.start(program, arguments);
    if (p.waitForStarted())
    {
        p.waitForFinished();

        QByteArray all = p.readAll();
            if(all.isEmpty())
            {
                all=p.readAllStandardOutput();
                if(all.isEmpty())
                {
                    all=p.readAllStandardError();
                }
            }
        QString tt=     QString::fromLocal8Bit(all);
        textedit->append(tt);
        textedit->append("--------------------------");
        textedit->append("[成功] - 格式化成功！\n");
        //DeviceDiskNumber.clear();
        return;
    }
    else
    {
        qDebug() << "Failed to start";
        textedit->append("[失败] - 格式化失败！\n");
        return;
    }

}

void SystemConfig::slot_btn_restoreDeviceData()
{
     textedit->append("[操作] - 还原设备数据");
     if(DeviceDiskNumber=="")
     {
         textedit->append(" - 未找到设备，请检查设备状态！");
         return;
     }

     qDebug()<<"编辑---还原设备数据";

     RestoreDeviceDefaultData* RDDD=new RestoreDeviceDefaultData;
     connect(this,SIGNAL(signal_restroeDeviceData()),RDDD,SLOT(startCOPYFileinSecondDir()));//启动
     connect(RDDD,SIGNAL(work_message(QString)),this,SLOT(slot_ShowMesageFromThread(QString)));//消息框
     connect(RDDD,SIGNAL(work_Progress(qint64)),this,SLOT(slot_SowWorkProgress(qint64)));//测试


     QThread* thread = new QThread(parentthis);
     connect(RDDD,SIGNAL(work_over()),thread,SLOT(quit()));//退出线程

     QWaitDialog* wait = new QWaitDialog(parentthis,"正在还原设备");wait->move(thisWidth-wait->width()+130,thisHeight-wait->height()+40);
     connect(RDDD,SIGNAL(work_Progress(qint64)),wait,SLOT(slot_showProgress(qint64)));//显示数据
     connect(RDDD,SIGNAL(work_over()),wait,SLOT(close()));//关闭

     RDDD->TODir=DeviceDiskNumber+"\\Active_Data";    //G:+ "\\Active_Data";
     RDDD->moveToThread(thread);

     thread->start();//启动线程
     //RDDD->start();//这句话会在主线程里面执行，导致堵塞
     emit signal_restroeDeviceData();//通知线程开始工作

}

void SystemConfig::slot_btn_readlogfile()
{
    textedit->append("[操作] - 读取日志文件");
    if(DeviceDiskNumber=="")
    {
        textedit->append(" - 未找到设备，请检查设备状态！");
        return;
    }
    textedit->append("[信息] - 抱歉，该功能处于未开发状态。");

}

void SystemConfig::slot_btn_manageTipsInfo()
{
    new ManageTipsInfo(this);
}

void SystemConfig::slot_btn_usart()
{
    new UsartCommunication(this);
}



