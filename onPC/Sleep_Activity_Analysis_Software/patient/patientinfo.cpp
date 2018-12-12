#include "patientinfo.h"
#include <QDebug>
#include <QMessageBox>

#include <QDateTime>
#include "DataBase/infodaoimp.h"
#define  window_width 600
#define window_height 500
#define dateedit_format "yyyy-MM-dd"
PatientInfo::PatientInfo(QWidget *parent):QDialog(parent)
{


    setFixedSize(window_width,window_height);
    setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_DeleteOnClose);

    this->setWindowFlags(Qt::FramelessWindowHint |windowFlags()); //| Qt::Window );//
    //setWindowFlags(Qt::WindowStaysOnTopHint);
    pTitleBar = new TitleBar(this);
    pTitleBar->setFont(QFont("楷体",12,75));

    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);
    pTitleBar->setPalette(pa);

    installEventFilter(pTitleBar);
    setWindowTitle("新增观察对象");
    //setWindowIcon(QIcon(":/Images/logo"));
    setAutoFillBackground(true);

    //界面
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(117, 138, 151));
    setAutoFillBackground(true);
    setPalette(pal);

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addStretch();
       pLayout->setSpacing(0);
       pLayout->setContentsMargins(0, 0, 0, 0);
       setLayout(pLayout);
    lb_backcolor = new QLabel(this);
    lb_backcolor->setGeometry(0,pTitleBar->height(),10000,10000);
    lb_backcolor->setStyleSheet("background-color:white;");


    init_Label_LineEdit();

    init_Layout();

    dc =DataContainer::getInstance();


    this->show();

}

PatientInfo::~PatientInfo()
{
    qDebug()<<"析构--新增patient界面";
}

void PatientInfo::init_Label_LineEdit()
{
    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor(100,100,100));

//观测对象信息
    lb_PatientInfo =new QLabel(this);lb_PatientInfo->setFont(QFont("楷体",16,75));lb_PatientInfo->setText("对象信息");
    lb_NUMBER= new QLabel(this); lb_NUMBER      ->setText("<font color = '#ff0000'>* </font>ID：");le_NUMBER =new QLineEdit(this);//自动生成
    lb_NAME= new QLabel(this);   lb_NAME        ->setText("<font color = '#ff0000'>* </font>姓名：");le_NAME =new QLineEdit(this);
    lb_GANDER= new QLabel(this);  lb_GANDER     ->setText("<font color = '#ff0000'>* </font>性别：");comB_GANDER =new QComboBox(this);comB_GANDER->addItem(tr("男"));comB_GANDER->addItem(tr("女"));
    lb_BRITHDAY= new QLabel(this);lb_BRITHDAY   ->setText("<font color = '#ff0000'>* </font>生日：");dateEdit = new QDateEdit(this);dateEdit->setMaximumDateTime(QDateTime::currentDateTime());dateEdit->setCalendarPopup(true);//dateEdit->setDate(QDate(2018,01,01));//选择
    lb_AGE= new QLabel(this);    lb_AGE         ->setText("<font color = '#ff0000'>* </font>月龄：");le_AGE =new QLineEdit(this);le_AGE->setEnabled(0);//自动生成

    /*
     * 日期
     * */
    dateEdit->setDate(QDate::currentDate());
    le_AGE->setText("0个月");

    le_NUMBER->setPlaceholderText("请输入观察对象ID号");
    le_NAME->setPlaceholderText("请输入观察对象姓名");

    //观测对象正则
    //[\u4e00-\u9fa5]{0,5}      只输入中文5个字
    QRegExp regExp("^[\u4e00-\u9fa5]{0,5}[a-zA-z]{0,30}$"); //正则表达式
    le_NAME->setValidator(new QRegExpValidator(regExp,this));
    regExp.setPattern("^[0-9]{0,25}$");
    le_NUMBER->setValidator(new QRegExpValidator(regExp,this));


    dateEdit->setDisplayFormat(dateedit_format);
    lb_NUMBER->setPalette(pa);
    lb_NAME->setPalette(pa);
    lb_GANDER->setPalette(pa);
    lb_BRITHDAY->setPalette(pa);
    lb_AGE->setPalette(pa);
    le_NUMBER->setStyleSheet("background-color:rgb(255,255,255);color: black;border-radius: 4px; border: 1px groove rgb(200,200,200); ");
    le_NAME->setStyleSheet("background-color:rgb(255,255,255);color: black;border-radius: 4px; border: 1px groove rgb(200,200,200); ");
    comB_GANDER->setStyleSheet("background-color:rgb(255,255,255);color: black;border-radius: 4px; border: 1px groove rgb(200,200,200); ");
    dateEdit->setStyleSheet("background-color:rgb(255,255,255);color: black;border-radius: 4px; border: 1px groove rgb(200,200,200); ");
    le_AGE->setStyleSheet("background-color:rgb(255,255,255);color: black;border-radius: 4px; border: 1px groove rgb(200,200,200); ");

//监护人信息
    lb_WatcherInfo  =new QLabel(this);lb_WatcherInfo    ->setFont(QFont("楷体",16,75));lb_WatcherInfo->setText("监护人信息");
    lb_watchername  =new QLabel(this);lb_watchername    ->setText("<font color = '#ff0000'>* </font>姓名：");le_watchername =new QLineEdit(this);//姓名
    lb_watcherphone =new QLabel(this);lb_watcherphone   ->setText("<font color = '#ff0000'>* </font>联系电话：");le_watcherphone =new QLineEdit(this);//联系电话
    lb_watcheradress=new QLabel(this);lb_watcheradress  ->setText("  联系地址：");le_watcheradress =new QLineEdit(this);//联系地址

    lb_watchername->setPalette(pa);
    lb_watcherphone->setPalette(pa);
    lb_watcheradress->setPalette(pa);
    le_watchername->setStyleSheet("background-color:rgb(255,255,255);color: black;border-radius: 4px; border: 1px groove rgb(200,200,200); ");
    le_watcherphone->setStyleSheet("background-color:rgb(255,255,255);color: black;border-radius: 4px; border: 1px groove rgb(200,200,200); ");
    le_watcheradress->setStyleSheet("background-color:rgb(255,255,255);color: black;border-radius: 4px; border: 1px groove rgb(200,200,200); ");

    connect(dateEdit,SIGNAL(dateTimeChanged(QDateTime)),this,SLOT(slot_calculateAge()));

    btn_save = new QPushButton(this);btn_save->setText("确定");
    connect(btn_save,SIGNAL(clicked(bool)),this,SLOT(slot_btn_save()));
    btn_cancel = new QPushButton(this);btn_cancel->setText("取消");
    connect(btn_cancel,SIGNAL(clicked(bool)),this,SLOT(slot_btn_cancel()));

    btn_save->setStyleSheet("background-color:rgb(117, 138, 151);color: white;border-radius: 4px; border: 1px groove rgb(200,200,200); ");
    btn_cancel->setStyleSheet("background-color:rgb(255,255,255);color: black;border-radius: 4px; border: 1px groove rgb(200,200,200); ");


    lb_NUMBER   ->setFixedSize(100,25);
    lb_AGE      ->setFixedSize(100,25);
    lb_BRITHDAY ->setFixedSize(100,25);
    lb_GANDER   ->setFixedSize(100,25);
    lb_NAME     ->setFixedSize(100,25);
    le_NUMBER   ->setFixedSize(window_width*3/5,25);
    le_AGE      ->setFixedSize(window_width*3/5,25);
    dateEdit    ->setFixedSize(window_width*3/5,25);
    comB_GANDER ->setFixedSize(window_width*3/5,25);
    le_NAME     ->setFixedSize(window_width*3/5,25);
    btn_save    ->setFixedSize(100,30);
    btn_cancel->setFixedSize(100,30);
    lb_PatientInfo->setFixedSize(window_width*3/5,30);
    lb_WatcherInfo->setFixedSize(window_width*3/5,30);
    lb_watchername->setFixedSize(100,25);
    lb_watcherphone ->setFixedSize(100,25);
    lb_watcheradress->setFixedSize(100,25);
    le_watchername  ->setFixedSize(window_width*3/5,25);
    le_watcherphone ->setFixedSize(window_width*3/5,25);
    le_watcheradress->setFixedSize(window_width*3/5,25);

    //监护人信息输入提示
    le_watchername->setPlaceholderText("请输入监护人姓名");
    le_watcherphone->setPlaceholderText("请输入监护人电话");
    le_watcheradress->setPlaceholderText("请输入监护人住址");

    //监护人信息正则

    regExp.setPattern("^[\u4e00-\u9fa5]{0,5}[a-zA-z]{0,30}$");
    le_watchername->setValidator(new QRegExpValidator(regExp,this));
    regExp.setPattern("^((13[0-9])|(14[5|7])|(15([0-3]|[5-9]))|(18[0,5-9]))\\d{8}$");
    le_watcherphone->setValidator(new QRegExpValidator(regExp,this));


}

void PatientInfo::init_Layout()
{

    ly_H_NUMBER = new QHBoxLayout();
    ly_H_NUMBER->addStretch();
    ly_H_NUMBER->addWidget(lb_NUMBER);
    ly_H_NUMBER->addWidget(le_NUMBER);
    ly_H_NUMBER->addStretch();

    ly_H_NAME = new QHBoxLayout();
    ly_H_NAME->addStretch();
    ly_H_NAME->addWidget(lb_NAME);
    ly_H_NAME->addWidget(le_NAME);
    ly_H_NAME->addStretch();

    ly_H_GANDER = new QHBoxLayout();
    ly_H_GANDER->addStretch();
    ly_H_GANDER->addWidget(lb_GANDER);
    ly_H_GANDER->addWidget(comB_GANDER);
    ly_H_GANDER->addStretch();

    ly_H_BRITHDAY = new QHBoxLayout();
    ly_H_BRITHDAY->addStretch();
    ly_H_BRITHDAY->addWidget(lb_BRITHDAY);
    ly_H_BRITHDAY->addWidget(dateEdit);
    ly_H_BRITHDAY->addStretch();

    ly_H_AGE = new QHBoxLayout();
    ly_H_AGE->addStretch();
    ly_H_AGE->addWidget(lb_AGE);
    ly_H_AGE->addWidget(le_AGE);
    ly_H_AGE->addStretch();

    ly_H_Button = new QHBoxLayout();
    ly_H_Button->addStretch();
    ly_H_Button->addWidget(btn_save);
    ly_H_Button->addSpacing(10);
    ly_H_Button->addWidget(btn_cancel);
    ly_H_Button->addSpacing(65);


    ly_H_patientinfo = new QHBoxLayout();
    ly_H_patientinfo->addSpacing(30);
    ly_H_patientinfo->addWidget(lb_PatientInfo);
    ly_H_patientinfo->addStretch();

    ly_H_watcherinfo  = new QHBoxLayout();
    ly_H_watcherinfo->addSpacing(30);
    ly_H_watcherinfo->addWidget(lb_WatcherInfo);
    ly_H_watcherinfo->addStretch();

    ly_H_watchername   = new QHBoxLayout();
    ly_H_watchername->addStretch();
    ly_H_watchername->addWidget(lb_watchername);
    ly_H_watchername->addWidget(le_watchername);
    ly_H_watchername->addStretch();

    ly_H_watcherphone  = new QHBoxLayout();
    ly_H_watcherphone->addStretch();
    ly_H_watcherphone->addWidget(lb_watcherphone);
    ly_H_watcherphone->addWidget(le_watcherphone);
    ly_H_watcherphone->addStretch();

    ly_H_watcheradress  = new QHBoxLayout();
    ly_H_watcheradress->addStretch();
    ly_H_watcheradress->addWidget(lb_watcheradress);
    ly_H_watcheradress->addWidget(le_watcheradress);
    ly_H_watcheradress->addStretch();

    ly_V_ALL = new QVBoxLayout();
    ly_V_ALL->addStretch();
    ly_V_ALL->addSpacing(20);
    ly_V_ALL->addLayout(ly_H_patientinfo);
    ly_V_ALL->addLayout(ly_H_NUMBER);   ly_V_ALL->addSpacing(10);
    ly_V_ALL->addLayout(ly_H_NAME);     ly_V_ALL->addSpacing(10);
    ly_V_ALL->addLayout(ly_H_GANDER);   ly_V_ALL->addSpacing(10);
    ly_V_ALL->addLayout(ly_H_BRITHDAY); ly_V_ALL->addSpacing(10);
    ly_V_ALL->addLayout(ly_H_AGE);      ly_V_ALL->addSpacing(20);

    ly_V_ALL->addLayout(ly_H_watcherinfo);ly_V_ALL->addSpacing(10);
    ly_V_ALL->addLayout(ly_H_watchername);ly_V_ALL->addSpacing(10);
    ly_V_ALL->addLayout(ly_H_watcherphone);ly_V_ALL->addSpacing(10);
    ly_V_ALL->addLayout(ly_H_watcheradress);ly_V_ALL->addSpacing(10);

    ly_V_ALL->addLayout(ly_H_Button);ly_V_ALL->addSpacing(10);
    ly_V_ALL->addStretch();
    //setLayout(ly_V_ALL);
    ly_V_ALL->setGeometry(QRect(0,0,window_width,window_height));

}

void PatientInfo::slot_btn_save()
{
    qDebug()<<"保存，发送数据信号";
    QString ID                  =       le_NUMBER->text();
    QString Name                =       le_NAME->text();
    QString Gender              =       comB_GANDER->currentText();
    QString BrithDay            =       dateEdit->text();
    QString Age                 =       le_AGE->text();

    QString WatcherName         =       le_watchername->text();
    QString WatcherPhone        =       le_watcherphone->text();
    QString WatcherAdress       =       le_watcheradress->text();

    QString CreationDate        =       QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString Creator             =       "无";
    QString ModificationDate    =       "无";
    QString Modifier            =       "无";

    if(Name == "" || ID == "" || WatcherName == "" || WatcherPhone == "")
    {
        QMessageBox::information(this,"提示","当前信息不完整！",QMessageBox::Ok);
        return;
    }
    //------------------------------------------------------------
    InfoDaoImp idi(this);
    idi.insertData_PatientInfo(ID,Name,Gender,BrithDay,Age,WatcherName,WatcherPhone,WatcherAdress,CreationDate,Creator,ModificationDate,Modifier);
    //-------------------------------------------------------------
    dc->ID=ID;
    dc->PatientName=Name;
    dc->Gander=Gender;
    dc->BrithDay=BrithDay;
    dc->Age=Age;
    dc->WatcherName=WatcherName;
    dc->WatcherPhone=WatcherPhone;
    dc->WatcherAdress=WatcherAdress;
    emit signal_Sure();
    this->close();
}

void PatientInfo::slot_btn_cancel()
{
    this->close();
}

void PatientInfo::slot_calculateAge()
{
    qDebug()<<"出生月计算";
    QString QSyear=QDateTime::currentDateTime().toString("yyyy");
    QString QMonth=QDateTime::currentDateTime().toString("MM");

    QString BSyear =dateEdit->dateTime().toString("yyyy");
    QString BSmonth =dateEdit->dateTime().toString("MM");

    //已出生月数
    int sum= (QSyear.toInt() - BSyear.toInt())*12 + QMonth.toInt()-BSmonth.toInt();
    QString sss;
    if(sum<12)sss=QString::number(sum)+"个月";
    else
    {
        int year = sum/12;//岁
        int month =sum%12;//月
        if(month!=0)sss = QString::number(year)+"岁"+QString::number(month)+"个月";
        else
            sss = QString::number(year)+"岁";
    }
    le_AGE->setText(sss);
}
