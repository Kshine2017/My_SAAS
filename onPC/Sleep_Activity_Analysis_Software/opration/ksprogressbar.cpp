#include "ksprogressbar.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>
//界面大小
#define WWW 300
#define HHH 150


//和图片有关的参数，不能修改
#define PStartStart 25
#define PStartOver 75

#define PFristStart 350
#define PFristOver 395

#define PSecondStart 560
#define PSecondOver 605

#define PThirdStart 775
#define PThirdOver 820

#define PEndStart 935
#define PEndOver 980

KSProgressBar::KSProgressBar(QWidget *parent) : QDialog(parent)
{

    this->setWindowFlags(Qt::Dialog| Qt::FramelessWindowHint);
    this->setFixedSize(WWW,HHH);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setStyleSheet("background:transparent;");
    QDesktopWidget* desktop = QApplication::desktop();
    int Window_width=desktop->width();
    int Window_height=desktop->height();
    if(Window_width==1920)Window_height=1080;//简单处理分屏问题
    this->move(Window_width-WWW,Window_height-HHH-50);

    lb_background=new QLabel(this);lb_background->setFixedSize(WWW,HHH);
    lb_back=new QLabel(this);lb_back->setFixedSize(WWW,60*WWW/1000);
    lb_mid=new QLabel(this);lb_mid->setFixedSize(WWW,60*WWW/1000);
    lb_front=new QLabel(this);lb_front->setFixedSize(WWW,60*WWW/1000);
    tip_label = new QLabel(this);tip_label->setFixedSize(WWW,60);tip_label->setStyleSheet("background:transparent");

    YYY=HHH/4;
    lb_back->move(0,YYY);
    lb_mid->move(0,YYY);
    lb_front->move(0,YYY);

    QPixmap *pixmap_Back = new QPixmap(":/picture/operation/progressbar_Back.png");//:/picture/operation/progressbar_Back.png
    QPixmap *pixmap_Mid = new QPixmap(":/picture/operation/progressbar_Mid.png");
    QPixmap *pixmap_Front = new QPixmap(":/picture/operation/progressbar_Front.png");
    QPixmap *pixmap_backgound = new QPixmap(":/picture/operation/progressbar_Background.png");
    pixmap_backgound->scaled(lb_background->size(), Qt::KeepAspectRatio);
    pixmap_Back->scaled(lb_back->size(), Qt::KeepAspectRatio);
    pixmap_Mid->scaled(lb_mid->size(), Qt::KeepAspectRatio);
    pixmap_Front->scaled(lb_front->size(), Qt::KeepAspectRatio);

    lb_background->setScaledContents(true);
    lb_back->setScaledContents(true);
    lb_mid->setScaledContents(true);
    lb_front->setScaledContents(true);



    lb_background->setPixmap(QPixmap(*pixmap_backgound));
    lb_back->setPixmap(QPixmap(*pixmap_Back));
    lb_mid->setPixmap(QPixmap(*pixmap_Mid));
    lb_front->setPixmap(QPixmap(*pixmap_Front));

    tem=0;
    tem_int=0;
    TotalSize=0;
    TotalNumber=0;
    workflag=1;
    this->show();
}

void KSProgressBar::SetPercentage(int flag, double PPP)
{
    int beforeOver=0;
    int nextStart=0;
    int length=0;
    if(flag==1)//第一阶段工作
    {
        beforeOver =PStartOver*WWW/1000;
        nextStart = PFristOver*WWW/1000;
    }

    else if(flag==2)//第2阶段工作
    {
        beforeOver =PFristOver*WWW/1000;
        nextStart = PSecondOver*WWW/1000;
    }
    else if(flag==3)//第3阶段工作
    {
        beforeOver =PSecondOver*WWW/1000;
        nextStart = PThirdOver*WWW/1000;
    }
    else if(flag==4)//第4阶段工作
    {
        beforeOver =PThirdOver*WWW/1000;
        nextStart = PEndOver*WWW/1000;
    }

    length= nextStart -beforeOver;//该段 的总长度
    lb_mid->move(PPP*length/100+beforeOver,YYY);

}

KSProgressBar::~KSProgressBar()
{
    qDebug()<<"析构---KSProgressBar";

}

void KSProgressBar::slot_setTotalSize(qint64 totalsize)
{
    TotalSize=totalsize;
}

void KSProgressBar::slot_showSizeProgress(qint64 size)
{

    QString msg="进度"+QString::number(workflag)+"：";
    tem+=size;
    double kpkpkp=0;
    if(TotalSize)
    {
        kpkpkp = tem*100.0/TotalSize;
        msg+=QString::number(kpkpkp,'f',2)+"%";
    }
    tip_label->setText(msg);
    //-------------------------------
    SetPercentage(workflag, kpkpkp);
}

void KSProgressBar::slot_setToTalNumber(int totalnum)
{
    TotalNumber=totalnum;
}

void KSProgressBar::slot_showNumProgress(int num)
{
    QString msg="进度"+QString::number(workflag)+"：";
    tem_int+=num;
    double kpkpkp=0;

    if(TotalNumber)
    {
        kpkpkp = tem_int*100.0/TotalNumber;
        if(kpkpkp>100)kpkpkp=100;
        msg+=QString::number(kpkpkp,'f',2)+"%";
    }
    tip_label->setText(msg);

    //-------------------------------

    SetPercentage(workflag, kpkpkp);
}

void KSProgressBar::slot_clearTemData()
{
    tem=0;
    tem_int=0;
    TotalSize=0;
    TotalNumber=0;
}

//每次切换工作，都自加
void KSProgressBar::slot_addWorkFlag()
{
//    tem=0;
//    tem_int=0;
//    TotalSize=0;
//    TotalNumber=0;
    workflag++;
    qDebug()<<"============================>"<<workflag;
    if(workflag>4)
       close();//最后一个工作结束时，关闭此类
}
