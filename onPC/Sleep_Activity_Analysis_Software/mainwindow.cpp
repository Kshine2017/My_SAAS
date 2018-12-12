#include "mainwindow.h"
//#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>
#include <QLine>
#include <QTime>
#include "DataContainer/datacontainer.h"
#include <dbt.h>
#include <QDesktopServices>
#include "patient/switchpatient.h"
#include "patient/patientinfo.h"

#define UPTOOLHEIGHT 40
#define WINDOWHEIGHTLESS    0
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    //主窗口设置
    QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();也可以
    thisWIDTH =desktop->width();
    thisHEIGHT=desktop->height();
    if(thisWIDTH ==1920||thisWIDTH ==1924)//测试
        thisHEIGHT=1080;//测试
//    thisWIDTH =1366;
//    thisHEIGHT =768;
    qDebug()<<"屏幕大小："<<thisWIDTH<<thisHEIGHT;
    setFixedSize(thisWIDTH,thisHEIGHT-WINDOWHEIGHTLESS);
    Page_flag =0;//初始页面

    //-----------------菜单条-------------------
    UpDock = new QDockWidget(this); //工具条窗口
    UpDock->setAutoFillBackground(true);//自动填满背景
    UpDock->setWindowFlags(Qt::FramelessWindowHint);//不可调节大小
    UpDock->setFeatures(QDockWidget::NoDockWidgetFeatures);//不可移动

    upwidget = new QWidget(this); //不继承
    upwidget->setAutoFillBackground(true);
    upwidget->setWindowFlags(Qt::FramelessWindowHint);

    UpDock->setFixedSize(thisWIDTH,UPTOOLHEIGHT);
    UpDock->setWidget(upwidget);
    addDockWidget(Qt::TopDockWidgetArea,UpDock);
    //----------------------------------------
    lb_UP_background =new QLabel(UpDock);
    lb_UP_background->setFixedSize(thisWIDTH,UPTOOLHEIGHT);
    QPixmap pix_tooll;
    pix_tooll.load(":/picture/updockbackground.png");
    pix_tooll.scaled(lb_UP_background->size(), Qt::IgnoreAspectRatio);
    lb_UP_background->setScaledContents(true);
    lb_UP_background->setPixmap(pix_tooll);
    lb_UP_background->setGeometry(0,0,thisWIDTH,UPTOOLHEIGHT);

    init_UP_Label(); //初始化软件标题栏窗口
    //-----------------工具条-------------------
    DockWindow = new QDockWidget(this); //工具条窗口
    DockWindow->setAutoFillBackground(true);//自动填满背景
    DockWindow->setWindowFlags(Qt::FramelessWindowHint);//不可调节大小
    DockWindow->setFeatures(QDockWidget::NoDockWidgetFeatures);//不可移动

    UserWindow = new QWidget(this); //不继承
    UserWindow->setAutoFillBackground(true);
    UserWindow->setWindowFlags(Qt::FramelessWindowHint);

    //主界面的工具栏
    DockWindow->setFixedSize(thisWIDTH/10,thisHEIGHT-WINDOWHEIGHTLESS);
    //UserWindow->setFixedSize(thisWIDTH/10,thisHEIGHT-WINDOWHEIGHTLESS);

    DockWindow->setWidget(UserWindow);
    addDockWidget(Qt::LeftDockWidgetArea,DockWindow);
    //主界面的工具栏背景--------------------------
    lb_Left_Background =new QLabel(DockWindow);
    lb_Left_Background->setFixedSize(thisWIDTH/10,thisHEIGHT-UPTOOLHEIGHT-WINDOWHEIGHTLESS-5);
    QPixmap pix_tool;
    pix_tool.load(":/picture/leftdockbackground.png");
    pix_tool.scaled(lb_Left_Background->size(), Qt::IgnoreAspectRatio);
    lb_Left_Background->setScaledContents(true);
    lb_Left_Background->setPixmap(pix_tool);
    lb_Left_Background->setGeometry(0,0,thisWIDTH/10,thisHEIGHT-WINDOWHEIGHTLESS);
    /**
     * @brief 工具栏按钮
     */
    init_Left_Btn();
    //----------------------------------------------------------------------

    //qDebug()<<"第一次连接到数据容器："<<DataContainer::getInstance();
    /**
     * @brief 第一次连接到界面出现的三个窗口选择按钮
     */
    create_CCNdialog();
}

MainWindow::~MainWindow()
{
//    delete ui;
    DataContainer::getInstance()->releaseContainers();

}
void Delay_MSec(unsigned int msec)
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < _Timer )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


void MainWindow::create_AnalzyPageWindow()
{
    if(Page_flag!=1)//如果在别的页面，则创建页面1
    {
        qDebug()<<"创建数据分析页面";
        Page_flag=1; //避免重复创建
        analysisWindow =new DataAnalysis(this,thisWIDTH/10*9,thisHEIGHT-UPTOOLHEIGHT-WINDOWHEIGHTLESS-5);
        analysisWindow->setWindowFlags(Qt::FramelessWindowHint);
        this->setCentralWidget(analysisWindow);
    }
}
void MainWindow::create_ManagementPageWindow()
{
    if(Page_flag!=2)//如果在别的页面，则创建页面1
    {
        qDebug()<<"创建数据管理页面";
        Page_flag=2; //避免重复创建

        manageWindow =new DataManagement(this,thisWIDTH/10*9,thisHEIGHT-UPTOOLHEIGHT-WINDOWHEIGHTLESS-5);
        manageWindow->setWindowFlags(Qt::FramelessWindowHint);
        this->setCentralWidget(manageWindow);
    }

}
void MainWindow::create_ExportReportWindow()
{
    if(Page_flag!=3)//如果在别的页面，则创建页面1
    {
        qDebug()<<"创建生成报告页面";
        Page_flag=3; //避免重复创建

        reportWindow =new ExportReport(this,thisWIDTH/10*9,thisHEIGHT-UPTOOLHEIGHT-WINDOWHEIGHTLESS-5);
        connect(reportWindow,SIGNAL(signal_getinDataAnalysisWindow()),this,SLOT(create_AnalzyPageWindow()));
        reportWindow->setWindowFlags(Qt::FramelessWindowHint);
        this->setCentralWidget(reportWindow);
    }
}


void MainWindow::create_SystemConfig()
{
    if(Page_flag!=4)//如果在别的页面，则创建页面1
    {
        qDebug()<<"创建系统设置页面";
        Page_flag=4; //避免重复创建

        systemWindow =new SystemConfig(this,thisWIDTH/10*9,thisHEIGHT-UPTOOLHEIGHT-WINDOWHEIGHTLESS-5);
        systemWindow->setWindowFlags(Qt::FramelessWindowHint);
        this->setCentralWidget(systemWindow);
    }
}

void MainWindow::create_pdfprint()
{
    if(Page_flag!=5)//如果在别的页面，则创建页面1
    {
        qDebug()<<"创建pdf页面";
        Page_flag=5; //避免重复创建

        pdfprintWindow =new PDFandPrint(this,thisWIDTH/10*9,thisHEIGHT-UPTOOLHEIGHT-WINDOWHEIGHTLESS-5);
        connect(pdfprintWindow,SIGNAL(signal_getinDataAnalysisWindow()),this,SLOT(create_AnalzyPageWindow()));
        connect(pdfprintWindow,SIGNAL(signal_getinExportReportWindow()),this,SLOT(create_ExportReportWindow()));
        pdfprintWindow->setWindowFlags(Qt::FramelessWindowHint);
        this->setCentralWidget(pdfprintWindow);
    }
}

void MainWindow::slot_LOGO()
{
    qDebug()<<"点击LOGO";
    QDesktopServices::openUrl(QUrl(QLatin1String("http://www.bb-care.com.cn/")));


}

void MainWindow::slot_createPatient()
{
    qDebug()<<"创建“新增对象”界面";
     new PatientInfo(this);
}

void MainWindow::slot_choosePatient()
{
    qDebug()<<"创建“选择对象”界面";
    new SwitchPatient(this);
}
void MainWindow::slot_quitButton()
{
    if(QMessageBox::question(this,"退出","确定要关闭软件吗？",QMessageBox::Yes|QMessageBox::No)==QMessageBox::No)
        return;
    Page_flag=100;//可以不写
    this->setCentralWidget(NULL);

    Delay_MSec(1000);
    this->close();
}

void MainWindow::backto_HomePage()
{
    if(Page_flag!=0)//如果在别的页面，则创建页面1
    {
        qDebug()<<"回到概览页面";
        Page_flag=0; //避免重复创建
        homewindow = new Homepage(this,thisWIDTH/10*9,thisHEIGHT-UPTOOLHEIGHT-WINDOWHEIGHTLESS-5);
        this->setCentralWidget(homewindow);
    }
}

void MainWindow::create_UserPageWindow()
{
    //预留

}


void MainWindow::init_UP_Label()
{
    ly_lb_H         = new QHBoxLayout();         //标签的水平布局
    ly_lb_H2         = new QHBoxLayout();         //标签的水平布局

    btn_LOGO         = new QPushButton(UpDock);         //logo图片-->按钮
    btn_LOGO->setFixedSize(48,30);
    btn_LOGO->setStyleSheet("QPushButton{border-image: url(:/picture/LOGO.png);}"
                            //"QPushButton:hover{border-image: url(:/picture/uploaddata-up.png);}"
                            //"QPushButton:pressed{border-image: url(:/picture/uploaddata-down.png);}"
                            );
    btn_LOGO->setFlat(true);
    connect(btn_LOGO,SIGNAL(clicked()),this,SLOT(slot_LOGO()));

    lb_CompanyName  = new QLabel(UpDock);         //公司名称
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);
    lb_CompanyName->setPalette(pa);
    lb_CompanyName->setFont(QFont("楷体",12,75));
    lb_CompanyName->setText("上海贝生医疗科技有限公司");

    lb_SoftWareName = new QLabel(UpDock);         //软件名称
    QPalette pa1;
    pa1.setColor(QPalette::WindowText,Qt::white);
    lb_SoftWareName->setPalette(pa1);
    lb_SoftWareName->setFont(QFont("楷体",12,75));
    lb_SoftWareName->setText("睡眠分析软件");

    lb_Version      = new QLabel(UpDock);         //版本
    QPalette pa2;
    pa2.setColor(QPalette::WindowText,Qt::white);
    lb_Version->setPalette(pa2);
    lb_Version->setFont(QFont("楷体",8,75));
    lb_Version->setText("版本号：BV1.0.0.1");

    ly_lb_H->addSpacing(30);
    ly_lb_H->addWidget(btn_LOGO);
    ly_lb_H->addSpacing(30);
    ly_lb_H->addWidget(lb_CompanyName);

    ly_lb_H2->addWidget(lb_SoftWareName);
    ly_lb_H2->addWidget(lb_Version);

    ly_lb_H->setGeometry(QRect(0,0,thisWIDTH/2,UPTOOLHEIGHT));
    ly_lb_H2->setGeometry(QRect(thisWIDTH-300,0,300,UPTOOLHEIGHT));
}

void MainWindow::init_Left_Btn()
{
    ly_btn_V = new QVBoxLayout;          //按钮的垂直布局

    btn_Home            = new QPushButton(DockWindow);   //回到主界面
    btn_Home->setIcon(QIcon(":/picture/huangzhijie/概览.png"));  btn_Home->setIconSize(QSize(30,30));              btn_Home->setText(" 概览      ");
    btn_Home->setStyleSheet(""
                            "QPushButton{font:18px;font-weight:bold;color:rgb(96 , 125 , 139);background-color:transparent;}"
                            "QPushButton:hover {font:20px;color:rgb(136 , 136 , 136);}"
                            "QPushButton:pressed { color:rgb(0 , 0 , 0);}"
                            "");
    connect(btn_Home,SIGNAL(clicked(bool)),this,SLOT(backto_HomePage()));

    btn_DataAnalyze     = new QPushButton(DockWindow);   //界面1
    connect(btn_DataAnalyze,SIGNAL(clicked()),this,SLOT(create_AnalzyPageWindow()));
    btn_DataAnalyze->setStyleSheet(""
                                   "QPushButton{font:18px;font-weight:bold;color:rgb(96 , 125 , 139);background-color:transparent;}"
                                   "QPushButton:hover {font:20px;color:rgb(136 , 136 , 136);}"
                                   "QPushButton:pressed { color:rgb(0 , 0 , 0);}"
                                   "");
    btn_DataAnalyze->setIcon(QIcon(":/picture/huangzhijie/数据分析.png")); btn_DataAnalyze->setIconSize(QSize(30,30));    btn_DataAnalyze->setText(" 数据分析  ");



    btn_DataManage      = new QPushButton(DockWindow);   //界面2
    connect(btn_DataManage,SIGNAL(clicked()),this,SLOT(create_ManagementPageWindow()));
    btn_DataManage->setStyleSheet(""
                                  "QPushButton{font:18px;font-weight:bold;color:rgb(96 , 125 , 139);background-color:transparent;}"
                                  "QPushButton:hover {font:20px;color:rgb(136 , 136 , 136);}"
                                  "QPushButton:pressed { color:rgb(0 , 0 , 0);}"
                                  "");
    btn_DataManage->setIcon(QIcon(":/picture/huangzhijie/数据管理.png"));btn_DataManage->setIconSize(QSize(30,30));    btn_DataManage->setText(" 数据管理  ");

    btn_ExportReport    = new QPushButton(DockWindow);   //界面3
    connect(btn_ExportReport,SIGNAL(clicked()),this,SLOT(create_ExportReportWindow()));
    btn_ExportReport->setStyleSheet(""
                                      "QPushButton{font:18px;font-weight:bold;color:rgb(96 , 125 , 139);background-color:transparent;}"
                                      "QPushButton:hover {font:20px;color:rgb(136 , 136 , 136);}"
                                      "QPushButton:pressed { color:rgb(0 , 0 , 0);}"
                                      "");
     btn_ExportReport->setIcon(QIcon(":/picture/huangzhijie/睡眠统计.png"));btn_ExportReport->setIconSize(QSize(30,30)); btn_ExportReport->setText(" 睡眠统计  ");


    btn_SystemConfig     = new QPushButton(DockWindow);   //界面4
    connect(btn_SystemConfig,SIGNAL(clicked()),this,SLOT(create_SystemConfig()));
    btn_SystemConfig->setStyleSheet(""
                                   "QPushButton{font:18px;font-weight:bold;color:rgb(96 , 125 , 139);background-color:transparent;}"
                                   "QPushButton:hover {font:20px;color:rgb(136 , 136 , 136);}"
                                   "QPushButton:pressed { color:rgb(0 , 0 , 0);}"
                                   "");
    btn_SystemConfig->setIcon(QIcon(":/picture/huangzhijie/系统设置.png"));btn_SystemConfig->setIconSize(QSize(30,30));  btn_SystemConfig->setText(" 系统设置  ");




    btn_Quit            = new QPushButton(DockWindow);   //退出
    connect(btn_Quit,SIGNAL(clicked()),this,SLOT(slot_quitButton()));
    btn_Quit->setStyleSheet(""
                            "QPushButton{font:18px;font-weight:bold;color:rgb(96 , 125 , 139);background-color:transparent;}"
                            "QPushButton:hover {font:20px;color:rgb(136 , 136 , 136);}"
                            "QPushButton:pressed { color:rgb(0 , 0 , 0);}"
                            "");
    btn_Quit->setIcon(QIcon(":/picture/huangzhijie/退出.png"));btn_Quit->setIconSize(QSize(30,30));                btn_Quit->setText(" 退出程序  ");
  //ly_btn_V->addSpacing(30);
    ly_btn_V->addWidget(btn_Home);


    btn_pdfprint            = new QPushButton(DockWindow);   //退出
    connect(btn_pdfprint,SIGNAL(clicked()),this,SLOT(create_pdfprint()));
    btn_pdfprint->setStyleSheet(""
                            "QPushButton{font:18px;font-weight:bold;color:rgb(96 , 125 , 139);background-color:transparent;}"
                            "QPushButton:hover {font:20px;color:rgb(136 , 136 , 136);}"
                            "QPushButton:pressed { color:rgb(0 , 0 , 0);}"
                            "");
    btn_pdfprint->setIcon(QIcon(":/picture/huangzhijie/生成报告.png"));btn_pdfprint->setIconSize(QSize(30,30));                btn_pdfprint->setText(" 生成报告  ");
  //ly_btn_V->addSpacing(30);



    QLabel *m_label = new QLabel(DockWindow);
    m_label->setFrameShape(QFrame::HLine);
    m_label->setFrameShadow(QFrame::Raised);
    m_label->setMaximumSize(QSize(1000, 1));
    ly_btn_V->addWidget(m_label);
    ly_btn_V->addWidget(btn_DataAnalyze);
    ly_btn_V->addWidget(btn_ExportReport);
    ly_btn_V->addWidget(btn_pdfprint);
    ly_btn_V->addWidget(btn_DataManage);
    ly_btn_V->addWidget(btn_SystemConfig);
    ly_btn_V->addWidget(btn_Quit);
    ly_btn_V->setGeometry(QRect(0,0,thisWIDTH/10,300));
}

void MainWindow::create_CCNdialog()
{
       LSdialog = new LoginSecondWidget(this);
       connect(LSdialog,SIGNAL(signal_CreatePatient()),this,SLOT(slot_createPatient()));
       connect(LSdialog,SIGNAL(signal_ChoosePatient()),this,SLOT(slot_choosePatient()));
       connect(LSdialog,SIGNAL(signal_getinwindow()),this,SLOT(create_AnalzyPageWindow()));
}
void MainWindow::initUSB()
{
    static const GUID GUID_DEVINTERFACE_LIST[] =
    {
        // GUID_DEVINTERFACE_USB_DEVICE
        { 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } },
        // GUID_DEVINTERFACE_DISK
        { 0x53f56307, 0xb6bf, 0x11d0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } },
        // GUID_DEVINTERFACE_HID,
        { 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } },
        // GUID_NDIS_LAN_CLASS
        { 0xad498944, 0x762f, 0x11d0, { 0x8d, 0xcb, 0x00, 0xc0, 0x4f, 0xc3, 0x35, 0x8c } },
        // GUID_DEVINTERFACE_COMPORT
        { 0x86e0d1e0, 0x8089, 0x11d0, { 0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73 } },
        // GUID_DEVINTERFACE_SERENUM_BUS_ENUMERATOR
        { 0x4D36E978, 0xE325, 0x11CE, { 0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18 } },
        // GUID_DEVINTERFACE_PARALLEL
        { 0x97F76EF0, 0xF883, 0x11D0, { 0xAF, 0x1F, 0x00, 0x00, 0xF8, 0x00, 0x84, 0x5C } },
        // GUID_DEVINTERFACE_PARCLASS
        { 0x811FC6A5, 0xF728, 0x11D0, { 0xA5, 0x37, 0x00, 0x00, 0xF8, 0x75, 0x3E, 0xD1 } }
    };

    //注册插拔事件
    HDEVNOTIFY hDevNotify;
    DEV_BROADCAST_DEVICEINTERFACE NotifacationFiler;
    ZeroMemory(&NotifacationFiler,sizeof(DEV_BROADCAST_DEVICEINTERFACE));
    NotifacationFiler.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotifacationFiler.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;

    for(unsigned int i=0;i<sizeof(GUID_DEVINTERFACE_LIST)/sizeof(GUID);i++)
    {
        NotifacationFiler.dbcc_classguid = GUID_DEVINTERFACE_LIST[i];//GetCurrentUSBGUID();//m_usb->GetDriverGUID();

        hDevNotify = RegisterDeviceNotification((HANDLE)this->winId(),&NotifacationFiler,DEVICE_NOTIFY_WINDOW_HANDLE);
        if(!hDevNotify)
        {
            int Err = GetLastError();
            qDebug() << "注册失败:"<<Err <<endl;
        }
    }
}
char MainWindow::FirstDriveFromMask (ULONG unitmask)
{
    char i;
    for (i = 0; i < 26; ++i)
    {
        if (unitmask & 0x1)
            break;
        unitmask = unitmask >> 1;
    }
    return (i + 'A');
}
bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)
    Q_UNUSED(result)
    DataContainer* dc =DataContainer::getInstance();
    MSG* msg = reinterpret_cast<MSG*>(message);
    int msgType = msg->message;
    if(msgType==WM_DEVICECHANGE)
    {
        //qDebug() << "Event DEVICECHANGE Happend" << endl;
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
        switch (msg->wParam) {
        case DBT_DEVICEARRIVAL:
            if(lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if(lpdbv->dbcv_flags ==0)
                {
                    //插入u盘
                    QString USBDisk = QString(this->FirstDriveFromMask(lpdbv ->dbcv_unitmask));
                    qDebug() << "USB_Arrived and The USBDisk is: "<<USBDisk ;
                    dc->DevicePath = USBDisk + ":\\Active_Data";
                    QMessageBox::information(this,"提示","设备已插入！",QMessageBox::Ok);

                }
            }
            if(lpdb->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
            {
                PDEV_BROADCAST_DEVICEINTERFACE pDevInf  = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;
                QString strname = QString::fromWCharArray(pDevInf->dbcc_name,pDevInf->dbcc_size);
                // ui->textBrowser->append("插入设备 ：" + strname);
            }
            break;

        case DBT_DEVICEREMOVECOMPLETE:
            qDebug() << "设备移除" <<endl;
            if(lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if(lpdbv->dbcv_flags == 0)
                {
                    qDebug() << "USB_Removed";
                    dc->DevicePath="";//去除设备地址
                    QMessageBox::information(this,"提示","设备已拔出！",QMessageBox::Ok);
                }
            }
            if(lpdb->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
            {
                PDEV_BROADCAST_DEVICEINTERFACE pDevInf  = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;
                qDebug()<< "移除设备(name)：" << pDevInf->dbcc_name;
                //qDebug()<< "移除设备(guid)：" << pDevInf->dbcc_classguid;
                qDebug()<< "移除设备(size)：" << pDevInf->dbcc_size;

                QString strname = QString::fromWCharArray(pDevInf->dbcc_name,pDevInf->dbcc_size);
            }
            break;

        }
    }
    return false;
}
