#include "statistics.h"
#include "onetable.h"
#include "DataContainer/datacontainer.h"
#include <QDebug>
#include "ExportReport/doctoradvice.h"
Statistics::Statistics(QWidget *parent, int thisWWW, int thisHHH, int flag_Day) : QWidget(parent)
{



    DataContainer* dc =DataContainer::getInstance();
    int TimesOfSleep=0;
    parentthis=parent;
    Flag_Day = flag_Day;
    QString Day;
    QString Tips;
         if(flag_Day==1){TimesOfSleep=dc->SUAN1.length();Day = dc->day1;Tips = dc->DoctorTips1;}
    else if(flag_Day==2){TimesOfSleep=dc->SUAN2.length();Day = dc->day2;Tips = dc->DoctorTips2;}
    else if(flag_Day==3){TimesOfSleep=dc->SUAN3.length();Day = dc->day3;Tips = dc->DoctorTips3;}
    else if(flag_Day==4){TimesOfSleep=dc->SUAN4.length();Day = dc->day4;Tips = dc->DoctorTips4;}
    else if(flag_Day==5){TimesOfSleep=dc->SUAN5.length();Day = dc->day5;Tips = dc->DoctorTips5;}
    else if(flag_Day==6){TimesOfSleep=dc->SUAN6.length();Day = dc->day6;Tips = dc->DoctorTips6;}

    int Hnum = thisWWW/TABLEWWW;//行表个数
    int HX=0;//补行标志
    if(TimesOfSleep%Hnum)HX=1;
    int endHHH = (TimesOfSleep/Hnum+HX)*TABLEHHH+40;
    setFixedSize(thisWWW,endHHH);

    lb_DateName = new QLabel(this);lb_DateName->setText("日期：");
    lb_DateValue = new QLabel(this);lb_DateValue->setText(Day);

    lb_TotalSleepTimesName = new QLabel(this);lb_TotalSleepTimesName->setText("入睡次数：");
    lb_TotalSleepTimesValue = new QLabel(this);lb_TotalSleepTimesValue->setText(QString::number(TimesOfSleep));

    lb_TotalSleepTimeName = new QLabel(this);lb_TotalSleepTimeName->setText("入睡时长：");
    lb_TotalSleepTimeValue = new QLabel(this);

    lb_DeepPercentName = new QLabel(this);lb_DeepPercentName->setText("深睡占比：");
    lb_DeepPercentValue = new QLabel(this);

    btn_Tips = new QPushButton(this);connect(btn_Tips,SIGNAL(clicked()),this,SLOT(slot_btn_advice()));
    if(Tips == "")
        {btn_Tips->setText("未给出建议");btn_Tips->setStyleSheet("background-color:rgb(255,255,255);");
    }else {btn_Tips->setText("已给出建议");btn_Tips->setStyleSheet("background-color:rgb(0,255,0);");}

    ly_H_Title = new QHBoxLayout();
    ly_H_Title->addWidget(lb_DateName);ly_H_Title->addWidget(lb_DateValue);
    ly_H_Title->addSpacing(30);
    ly_H_Title->addWidget(lb_TotalSleepTimesName);ly_H_Title->addWidget(lb_TotalSleepTimesValue);
    ly_H_Title->addSpacing(30);
    ly_H_Title->addWidget(lb_TotalSleepTimeName);ly_H_Title->addWidget(lb_TotalSleepTimeValue);
    ly_H_Title->addSpacing(30);
    ly_H_Title->addWidget(lb_DeepPercentName);ly_H_Title->addWidget(lb_DeepPercentValue);
    ly_H_Title->addSpacing(30);
    ly_H_Title->addWidget(btn_Tips);

    ly_H_Title->addStretch();
    //--------------------------------------------------------------------------------

    ly_V_all = new QVBoxLayout();
    ly_V_all->addLayout(ly_H_Title);
    int Hcount=0;
    QHBoxLayout *ly_H_tem;
    for(int i=0;i<TimesOfSleep;i++)  // 0  1  2       3  4  5       6  7  8
    {
        if(i%Hnum==0)
        {
            if(Hcount){ly_H_tem->addStretch();ly_V_all->addLayout(ly_H_tem);}
            Hcount++;//行数
            ly_H_tem = new QHBoxLayout();//换行
        }
        OneTable* tem = new OneTable(thisWWW,thisHHH,this,flag_Day,i);
        ly_H_tem->addWidget(tem);
    }
    if(Hcount){ly_H_tem->addStretch();ly_V_all->addLayout(ly_H_tem);}
    ly_V_all->addStretch();
    this->setLayout(ly_V_all);
    //测试用
    // this->setStyleSheet("background-color:red;");
    QVector<double>* tem144Shalow;
    QVector<double>* tem144Deep;
    QVector<struct_Duan> temDUAN;
    struct_Duan          temD;
         if(flag_Day==1){temDUAN=dc->SUAN1;tem144Shalow=&dc->shallowSleep144_Day1;tem144Deep=&dc->deepSleep144_Day1;}
    else if(flag_Day==2){temDUAN=dc->SUAN2;tem144Shalow=&dc->shallowSleep144_Day2;tem144Deep=&dc->deepSleep144_Day2;}
    else if(flag_Day==3){temDUAN=dc->SUAN3;tem144Shalow=&dc->shallowSleep144_Day3;tem144Deep=&dc->deepSleep144_Day3;}
    else if(flag_Day==4){temDUAN=dc->SUAN4;tem144Shalow=&dc->shallowSleep144_Day4;tem144Deep=&dc->deepSleep144_Day4;}
    else if(flag_Day==5){temDUAN=dc->SUAN5;tem144Shalow=&dc->shallowSleep144_Day5;tem144Deep=&dc->deepSleep144_Day5;}
    else if(flag_Day==6){temDUAN=dc->SUAN6;tem144Shalow=&dc->shallowSleep144_Day6;tem144Deep=&dc->deepSleep144_Day6;}

    int LenthOfShalow=0;
    int LenthOfDeep=0;
    int len =144;//10分钟

    double  rate = 1.0*(dc->MarkBarWidth-DUAN_BORDER*2)/len;
    for(int Flag_DUAN=0;Flag_DUAN<temDUAN.length();Flag_DUAN++ )
    {
        temD = temDUAN[Flag_DUAN];//得到两个坐标点 temD.startX 和 temD.endX

        int     KshineStart=(temD.startX-DUAN_BORDER)/rate;
        int     KshineEnd=(temD.endX-DUAN_BORDER)/rate;
        for(int i=KshineStart;i<=KshineEnd;i++)
        {
            if(i >= len)break;
            if((*tem144Shalow)[i])LenthOfShalow++;
            if((*tem144Deep)[i])LenthOfDeep++;
        }
    }

    //--------------------------------------------
    int Lsleep=LenthOfShalow+LenthOfDeep;
    double  percent =1.0*LenthOfDeep/Lsleep;
    lb_TotalSleepTimeValue->setText(QString::number(Lsleep/6)+"."+QString::number((Lsleep%6)*100/6)+"小时");
    lb_DeepPercentValue->setText(QString::number(percent*100,'f',1)+"%");
    //
}

void Statistics::slot_btn_advice()
{
    connect( new DoctorAdvice(parentthis,Flag_Day),SIGNAL(signal_tips_ok(bool)),this,SLOT(slot_btn_TEXT(bool)));
}

void Statistics::slot_btn_TEXT(bool ok)
{
    if(ok) {btn_Tips->setText("已给出建议");btn_Tips->setStyleSheet("background-color:rgb(0,255,0);");}
    else  {btn_Tips->setText("未给出建议");btn_Tips->setStyleSheet("background-color:rgb(255,255,255);");}
}
