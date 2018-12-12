#include "exportreport.h"
#include <QDebug>
#include "DataContainer/datacontainer.h"
#include "graphplot/statistics.h"
#include "graphplot/onetable.h"
#include <QDateTime>
#include "print/myprintpreviewdialog.h"
#include "QMessageBox"
#define dateedit_format "yyyy-MM-dd"
#define dateedit_format_all "yyyy-MM-dd hh:mm:ss"
ExportReport::ExportReport(QWidget *parent, int WWW, int HHH) : QWidget(parent)
{

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
    this->setFixedSize(thisWidth,thisHeight);
    this->setAutoFillBackground(true);

    qDebug()<<"页面：数据管理  尺寸："<<width()<<height();
    init_PageTopInfo();
    init_PageSecInfo();
    init_Statistics();
    init_PageThirdInfo();
    //init_PageReportInfo();
    init_PageAllLayout();
    qDebug()<<"数据管理页面：测试连接到数据容器"<<DataContainer::getInstance();
    qDebug()<<DataContainer::getInstance()->DevicePath;

    testTableData();

}

void ExportReport::slot_print()
{
    DataContainer* dc = DataContainer::getInstance();
    MyPrintPreViewDialog MPPVD(this);
    MPPVD.choosenum=dc->ChooseNUM;
    MPPVD.ID=dc->ID;
    MPPVD.Name=dc->PatientName;
    MPPVD.Gander=dc->Gander;
    MPPVD.Brithday=dc->BrithDay;
    MPPVD.Age=dc->Age;
    MPPVD.Date=QDateTime::currentDateTime().toString(dateedit_format);
    //数据--------------------------------------
    for(int i=0;i<dc->strlist.length();i++)
    {
        MPPVD.strlist.append(dc->strlist[i]);
    }
    //建议---------------------------------
    for(int i=0;i<dc->ChooseNUM;i++)
    {
        QString tips;
        QString day="";
        if(i==0){ tips=dc->DoctorTips1;day=dc->day1;}
        if(i==1){ tips=dc->DoctorTips2;day=dc->day2;}
        if(i==2){ tips=dc->DoctorTips3;day=dc->day3;}
        if(i==3){ tips=dc->DoctorTips4;day=dc->day4;}
        if(i==4){ tips=dc->DoctorTips5;day=dc->day5;}
        if(i==5){ tips=dc->DoctorTips6;day=dc->day6;}
        if(!tips.isEmpty())
        MPPVD.strlist_advice.append(day+": "+tips);
    }
    MPPVD.print();
}

void ExportReport::slot_printpreview()
{
    DataContainer* dc = DataContainer::getInstance();
    MyPrintPreViewDialog MPPVD(this);
    MPPVD.choosenum=dc->ChooseNUM;
    MPPVD.ID=dc->ID;
    MPPVD.Name=dc->PatientName;
    MPPVD.Gander=dc->Gander;
    MPPVD.Brithday=dc->BrithDay;
    MPPVD.Age=dc->Age;
    MPPVD.Date=QDateTime::currentDateTime().toString(dateedit_format);
    //数据--------------------------------------
    for(int i=0;i<dc->strlist.length();i++)
    {
        MPPVD.strlist.append(dc->strlist[i]);
    }
    //建议---------------------------------
    for(int i=0;i<dc->ChooseNUM;i++)
    {
        QString tips;
        QString day="";
        if(i==0){ tips=dc->DoctorTips1;day=dc->day1;}
        if(i==1){ tips=dc->DoctorTips2;day=dc->day2;}
        if(i==2){ tips=dc->DoctorTips3;day=dc->day3;}
        if(i==3){ tips=dc->DoctorTips4;day=dc->day4;}
        if(i==4){ tips=dc->DoctorTips5;day=dc->day5;}
        if(i==5){ tips=dc->DoctorTips6;day=dc->day6;}
        if(!tips.isEmpty())
        MPPVD.strlist_advice.append(day+": "+tips);
    }
    MPPVD.printpreview();
}

void ExportReport::slot_saveResult()
{
    DataContainer* dc = DataContainer::getInstance();
    if(dc->ChooseNUM==0)
    {
        QMessageBox::information(this,"提示","尚未选择文件进行分析，请至数据分析页面！",QMessageBox::Ok);
        emit signal_getinDataAnalysisWindow();
        return;
    }
    //---------------------
    calculate();
    //---------------------

    MyPrintPreViewDialog MPPVD(this);
    MPPVD.choosenum=dc->ChooseNUM;
    MPPVD.ID=dc->ID;
    MPPVD.Name=dc->PatientName;
    MPPVD.Gander=dc->Gander;
    MPPVD.Brithday=dc->BrithDay;
    MPPVD.Age=dc->Age;
    MPPVD.Date=QDateTime::currentDateTime().toString(dateedit_format);
    //数据--------------------------------------
    for(int i=0;i<dc->strlist.length();i++)
    {
        MPPVD.strlist.append(dc->strlist[i]);
    }
    //建议---------------------------------
    for(int i=0;i<dc->ChooseNUM;i++)
    {
        QString tips;
        QString day="";
        if(i==0){ tips=dc->DoctorTips1;day=dc->day1;}
        if(i==1){ tips=dc->DoctorTips2;day=dc->day2;}
        if(i==2){ tips=dc->DoctorTips3;day=dc->day3;}
        if(i==3){ tips=dc->DoctorTips4;day=dc->day4;}
        if(i==4){ tips=dc->DoctorTips5;day=dc->day5;}
        if(i==5){ tips=dc->DoctorTips6;day=dc->day6;}
        if(!tips.isEmpty())
        MPPVD.strlist_advice.append(day+": "+tips);
    }
    //---------------------------------------
    MPPVD.exportPDF();
    dc->KFlag_PDF=1;
    QMessageBox::information(this,"成功","已保存数据结果到PDF文件！",QMessageBox::Ok);
}

void ExportReport::init_PageTopInfo()
{
    pageName =new  QLabel(this);
    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor(92,123,137));
    pageName->setPalette(pa);
    pageName->setFont(QFont("楷体",22,75));
    pageName->setText("睡眠统计");

    ly_H_Top = new QHBoxLayout();
    ly_H_Top->addSpacing(10);
    ly_H_Top->addWidget(pageName);
    ly_H_Top->addStretch();
    ly_H_Top->addSpacing(10);
}

void ExportReport::init_PageSecInfo()
{
    ly_H_Sec = new QHBoxLayout();

    /*
    btn_print = new QPushButton(this);
    btn_print->setFixedSize(130,30);
    btn_print->setIcon(QIcon(":/picture/testIcon.png"));
    btn_print->setText(" 打印");
    btn_print->setStyleSheet(""
                                  "QPushButton{font:18px;font-weight:bold;color:rgb(96 , 125 , 139);background-color:transparent;}"
                                  "QPushButton:hover {font:20px;color:rgb(136 , 136 , 136);}"
                                  "QPushButton:pressed { color:rgb(0 , 0 , 0);}"
                                  "");
    connect(btn_print,SIGNAL(clicked(bool)),this,SLOT(slot_print()));
    ly_H_Sec->addSpacing(10);
    ly_H_Sec->addWidget(btn_print);



    btn_printpreview = new QPushButton(this);
    btn_printpreview->setFixedSize(130,30);
    btn_printpreview->setIcon(QIcon(":/picture/testIcon.png"));
    btn_printpreview->setText(" 打印预览");
    btn_printpreview->setStyleSheet(""
                                  "QPushButton{font:18px;font-weight:bold;color:rgb(96 , 125 , 139);background-color:transparent;}"
                                  "QPushButton:hover {font:20px;color:rgb(136 , 136 , 136);}"
                                  "QPushButton:pressed { color:rgb(0 , 0 , 0);}"
                                  "");
    connect(btn_printpreview,SIGNAL(clicked(bool)),this,SLOT(slot_printpreview()));

    ly_H_Sec->addSpacing(40);
    ly_H_Sec->addWidget(btn_printpreview);

*/
    btn_saveResult = new QPushButton(this);
    btn_saveResult->setFixedSize(130,30);
    btn_saveResult->setIcon(QIcon(":/picture/huangzhijie/保存.png"));
    btn_saveResult->setText(" 确认保存");
    btn_saveResult->setStyleSheet(""
                                  "QPushButton{font:18px;font-weight:bold;color:rgb(96 , 125 , 139);background-color:transparent;}"
                                  "QPushButton:hover {font:20px;color:rgb(136 , 136 , 136);}"
                                  "QPushButton:pressed { color:rgb(0 , 0 , 0);}"
                                  "");
    connect(btn_saveResult,SIGNAL(clicked(bool)),this,SLOT(slot_saveResult()));

    ly_H_Sec->addSpacing(40);
    ly_H_Sec->addWidget(btn_saveResult);

    ly_H_Sec->addStretch();
    ly_H_Sec->addSpacing(10);

}

void ExportReport::init_Statistics()
{
    DataContainer* dc =DataContainer::getInstance();
    int SDAY_num;//单日 段数
    int Hnum = thisWidth/TABLEWWW;//行表个数
    int HX=0;   //标志
    int endHHH=0; //结果

    for(int pi=0;pi<dc->ChooseNUM;pi++)
    {
        //        if(pi==0){SDAY_num=dc->DUAN1.length()-1;}
        //        if(pi==1){SDAY_num=dc->DUAN2.length()-1;}
        //        if(pi==2){SDAY_num=dc->DUAN3.length()-1;}
        //        if(pi==3){SDAY_num=dc->DUAN4.length()-1;}
        //        if(pi==4){SDAY_num=dc->DUAN5.length()-1;}
        //        if(pi==5){SDAY_num=dc->DUAN6.length()-1;}

        if(pi==0){SDAY_num=dc->SUAN1.length();}
        if(pi==1){SDAY_num=dc->SUAN2.length();}
        if(pi==2){SDAY_num=dc->SUAN3.length();}
        if(pi==3){SDAY_num=dc->SUAN4.length();}
        if(pi==4){SDAY_num=dc->SUAN5.length();}
        if(pi==5){SDAY_num=dc->SUAN6.length();}

        HX=0;//补行标志
        if(SDAY_num%Hnum)HX=1;
        endHHH += (SDAY_num/Hnum+HX)*TABLEHHH+40;
        qDebug()<<"-=-=-="<<pi<<SDAY_num;
    }
    qDebug()<<endHHH;

    //if(endHHH<thisHeight)endHHH=thisHeight;
    //创建widget
    SWidget = new QWidget(this);
    if(endHHH==0)endHHH=thisHeight;
    SWidget->setFixedSize(thisWidth,endHHH);
    //创建控件
    QVBoxLayout* ly_V_SADY = new QVBoxLayout();

    if(dc->ChooseNUM>=1){SDAY1= new Statistics(SWidget,thisWidth,thisHeight,1);ly_V_SADY->addWidget(SDAY1);}
    if(dc->ChooseNUM>=2){SDAY2= new Statistics(SWidget,thisWidth,thisHeight,2);ly_V_SADY->addWidget(SDAY2);}
    if(dc->ChooseNUM>=3){SDAY3= new Statistics(SWidget,thisWidth,thisHeight,3);ly_V_SADY->addWidget(SDAY3);}
    if(dc->ChooseNUM>=4){SDAY4= new Statistics(SWidget,thisWidth,thisHeight,4);ly_V_SADY->addWidget(SDAY4);}
    if(dc->ChooseNUM>=5){SDAY5= new Statistics(SWidget,thisWidth,thisHeight,5);ly_V_SADY->addWidget(SDAY5);}
    if(dc->ChooseNUM>=6){SDAY6= new Statistics(SWidget,thisWidth,thisHeight,6);ly_V_SADY->addWidget(SDAY6);}

    ly_V_SADY->addStretch();
    ly_V_SADY->setGeometry(QRect(0,0,thisWidth,endHHH));
    //创建滚动区域
    scrollArea = new QScrollArea(this);
    scrollArea->setFixedSize(thisWidth,thisHeight-(40+30+80+4));
    scrollArea->setBackgroundRole(QPalette::Light);  // 背景色
    scrollArea->setWidget(SWidget);
    scrollArea->setAlignment(Qt::AlignTop);  // 居中对齐






}

void ExportReport::init_PageThirdInfo()
{
    DataContainer* dc   =DataContainer::getInstance();
    QString patientname =dc->PatientName;
    QString Age         = dc->Age;
    QString Gander      = dc->Gander;
    QString ID          = dc->ID;
    QString datetime    = QDateTime::currentDateTime().toString(dateedit_format_all);

    WidgetLB = new QWidget(this);
    WidgetLB->setFixedSize(thisWidth,80);
    WidgetLB->setStyleSheet("background-color:rgb(160,177,185);");

    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor(255,255,255));
    QFont font;
    font.setFamily("楷体");font.setPointSize(12);font.setBold(true);

    lb_numberID = new QLabel(WidgetLB);
    lb_numberID->setFixedHeight(40);
    lb_numberID->setFont(font);lb_numberID->setPalette(pa);
    lb_numberID->setText("编号："+ID);

    lb_Date = new QLabel(WidgetLB);
    lb_Date->setFixedHeight(40);
    lb_Date->setFont(font);lb_Date->setPalette(pa);
    lb_Date->setText("日期："+datetime);
    //
    lb_name = new QLabel(WidgetLB);
    lb_name->setFont(font);
    lb_name->setPalette(pa);
    lb_name->setText("姓名："+patientname);
    lb_name->setFixedHeight(40);

    lb_age = new QLabel(WidgetLB);
    lb_age->setFont(font);
    lb_age->setPalette(pa);
    lb_age->setText("年龄："+Age);
    lb_age->setFixedHeight(40);

    lb_gander = new QLabel(WidgetLB);
    lb_gander->setFont(font);
    lb_gander->setPalette(pa);
    lb_gander->setText("性别："+Gander);
    lb_gander->setFixedHeight(40);

    ly_H_Thrid_up =new QHBoxLayout();

    ly_H_Thrid_down=new QHBoxLayout();

    ly_H_Thrid_up->addSpacing(60);
    ly_H_Thrid_up->addWidget(lb_numberID);
    ly_H_Thrid_up->addSpacing(60);
    ly_H_Thrid_up->addWidget(lb_Date);
    ly_H_Thrid_up->addStretch();
    ly_H_Thrid_down->addSpacing(60);
    ly_H_Thrid_down->addWidget(lb_name);
    ly_H_Thrid_down->addSpacing(60);
    ly_H_Thrid_down->addWidget(lb_age);
    ly_H_Thrid_down->addSpacing(60);
    ly_H_Thrid_down->addWidget(lb_gander);
    ly_H_Thrid_down->addStretch();
    ly_V_Thrid=new QVBoxLayout();

    ly_V_Thrid->addLayout(ly_H_Thrid_up);
    QLabel *m_label3 = new QLabel(WidgetLB);
    m_label3->setFrameShape(QFrame::HLine);
    m_label3->setFrameShadow(QFrame::Raised);
    m_label3->setMaximumSize(QSize(6000, 1));
    ly_V_Thrid->addWidget(m_label3);
    ly_V_Thrid->addLayout(ly_H_Thrid_down);
    //    WidgetLB->setLayout(ly_V_Thrid);
    ly_V_Thrid->setGeometry(QRect(0,0,thisWidth,80));

}

void ExportReport::init_PageReportInfo()
{
    lb_report1 = new QLabel(this);
    // lb_report1->setFixedWidth(thisWidth);
    QPixmap pix_tool1;
    pix_tool1.load(":/picture/test/reportLB1.png");
    pix_tool1.scaled(lb_report1->size(), Qt::IgnoreAspectRatio);
    lb_report1->setScaledContents(true);
    lb_report1->setPixmap(pix_tool1);

    lb_report2 = new QLabel(this);
    // lb_report2->setFixedWidth(thisWidth);
    QPixmap pix_tool2;
    pix_tool2.load(":/picture/test/reportLB2.png");
    pix_tool2.scaled(lb_report2->size(), Qt::IgnoreAspectRatio);
    lb_report2->setScaledContents(true);
    lb_report2->setPixmap(pix_tool2);

    lb_report3 = new QLabel(this);
    // lb_report3->setFixedWidth(thisWidth);
    QPixmap pix_tool3;
    pix_tool3.load(":/picture/test/reportLB3.png");
    pix_tool3.scaled(lb_report3->size(), Qt::IgnoreAspectRatio);
    lb_report3->setScaledContents(true);
    lb_report3->setPixmap(pix_tool3);
}


void ExportReport::init_PageAllLayout()
{
    ly_V_all = new QVBoxLayout();
    ly_V_all->addSpacing(10);
    ly_V_all->addLayout(ly_H_Top);

    QLabel *m_label = new QLabel(this);
    m_label->setFrameShape(QFrame::HLine);
    m_label->setFrameShadow(QFrame::Raised);
    m_label->setMaximumSize(QSize(6000, 1));
    ly_V_all->addWidget(m_label);

    ly_V_all->addLayout(ly_H_Sec);

    QLabel *m_label2 = new QLabel(this);
    m_label2->setFrameShape(QFrame::HLine);
    m_label2->setFrameShadow(QFrame::Raised);
    m_label2->setMaximumSize(QSize(6000, 1));
    ly_V_all->addWidget(m_label2);

    ly_V_all->addWidget(WidgetLB);


    QLabel *m_label4 = new QLabel(this);
    m_label4->setFrameShape(QFrame::HLine);
    m_label4->setFrameShadow(QFrame::Raised);
    m_label4->setMaximumSize(QSize(6000, 1));
    ly_V_all->addWidget(m_label4);

    //    ly_V_all->addWidget(lb_report1);
    //    ly_V_all->addWidget(lb_report2);
    //    ly_V_all->addWidget(lb_report3);

    //    ly_V_all->addWidget(SDAY1);
    //    ly_V_all->addWidget(SDAY2);
    //    ly_V_all->addWidget(SDAY3);
    //    ly_V_all->addWidget(SDAY4);
    //    ly_V_all->addWidget(SDAY5);
    //    ly_V_all->addWidget(SDAY6);

    ly_V_all->addWidget(scrollArea);
    ly_V_all->addStretch();

    ly_V_all->setGeometry(QRect(0,0,thisWidth,thisHeight));
}

void ExportReport::testTableData()
{

}

void ExportReport::calculate()
{
    DataContainer* dc   =DataContainer::getInstance();
    QVector<double>* tem144Shalow;
    QVector<double>* tem144Deep;
    QVector<struct_Duan> temDUAN;
    struct_Duan          temD;
    QString date;
    for(int flag_Day=1;flag_Day<=dc->ChooseNUM; flag_Day++)
    {
        if(flag_Day==1){date=dc->day1;temDUAN=dc->SUAN1;tem144Shalow=&dc->shallowSleep144_Day1;tem144Deep=&dc->deepSleep144_Day1;}
        else if(flag_Day==2){date=dc->day2;temDUAN=dc->SUAN2;tem144Shalow=&dc->shallowSleep144_Day2;tem144Deep=&dc->deepSleep144_Day2;}
        else if(flag_Day==3){date=dc->day3;temDUAN=dc->SUAN3;tem144Shalow=&dc->shallowSleep144_Day3;tem144Deep=&dc->deepSleep144_Day3;}
        else if(flag_Day==4){date=dc->day4;temDUAN=dc->SUAN4;tem144Shalow=&dc->shallowSleep144_Day4;tem144Deep=&dc->deepSleep144_Day4;}
        else if(flag_Day==5){date=dc->day5;temDUAN=dc->SUAN5;tem144Shalow=&dc->shallowSleep144_Day5;tem144Deep=&dc->deepSleep144_Day5;}
        else if(flag_Day==6){date=dc->day6;temDUAN=dc->SUAN6;tem144Shalow=&dc->shallowSleep144_Day6;tem144Deep=&dc->deepSleep144_Day6;}

        //    int LenthOfShalow=0;
        //    int LenthOfDeep=0;
        int TimesOfDay=0;
        int TimesOfNight=0;
        int LengthOfDayDeep=0;
        int LengthOfNightDeep=0;
        int LengthOfDayShalow=0;
        int LengthOfNightShalow=0;
        int len =144;//10分钟

        double  rate = 1.0*(dc->MarkBarWidth-DUAN_BORDER*2)/len;
        for(int Flag_DUAN=0;Flag_DUAN<temDUAN.length();Flag_DUAN++ )
        {
            temD = temDUAN[Flag_DUAN];//得到两个坐标点 temD.startX 和 temD.endX
            int fdn=0;
            int     KshineStart=(temD.startX-DUAN_BORDER)/rate;
            int     KshineEnd=(temD.endX-DUAN_BORDER)/rate;
            int KshineMid = (KshineStart+KshineEnd)/2;
            if(KshineMid<48||KshineMid>=108)
            {
                fdn=1;//day
                TimesOfDay++;
            }
            else
            {
                fdn=0;//night
                TimesOfNight++;
            }
            for(int i=KshineStart;i<=KshineEnd;i++)
            {
                if(i >= len)break;
                //if((*tem144Shalow)[i])LenthOfShalow++;
                if((*tem144Shalow)[i])
                {
                    if(fdn)LengthOfDayShalow++;
                    else LengthOfNightShalow++;
                }
                //if((*tem144Deep)[i])LenthOfDeep++;
                if((*tem144Deep)[i])
                {
                    if(fdn)LengthOfDayDeep++;
                    else LengthOfNightDeep++;
                }
            }
        }
        //save data
        dc->strlist.append(date);
        dc->strlist.append(QString::number(TimesOfDay)+"次");
        dc->strlist.append(QString::number(LengthOfDayDeep/6)+"."+QString::number((LengthOfDayDeep%6)*100/6)+"小时");
        dc->strlist.append(QString::number(LengthOfDayShalow/6)+"."+QString::number((LengthOfDayShalow%6)*100/6)+"小时");
        dc->strlist.append(QString::number(TimesOfNight)+"次");
        dc->strlist.append(QString::number(LengthOfNightDeep/6)+"."+QString::number((LengthOfNightDeep%6)*100/6)+"小时");
        dc->strlist.append(QString::number(LengthOfNightShalow/6)+"."+QString::number((LengthOfNightShalow%6)*100/6)+"小时");
    }

}


