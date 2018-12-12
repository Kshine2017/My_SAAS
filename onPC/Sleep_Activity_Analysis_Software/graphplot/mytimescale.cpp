#include "mytimescale.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>     //画笔


MyTimeScale::MyTimeScale(QWidget *parent):QFrame(parent)
{
    this->setStyleSheet("background-color:white");
    this->setFixedHeight(40);
    border=27;
    TimeStartPoint=12;//时间起始点 12点
    TimeDarkStartPoint=20;//黑夜开始点
    TimeDawnStartPoint=30;//早上开始点
    TimeOverPoint=36;//时间结束点12（36%24）
    //update();
}
void MyTimeScale::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev);
    QPainter painter(this);
    //painter.begin(this);
    //painter.drawText(0,0,100,30,Qt::AlignLeft,"时间条");
    //刻度
    //qDebug()<<this->width();
    //int www = double(1690-border*2)/24;
    int www = double(width()-border*2)/24;
    int borderUP=5;
    int hhh=20;
    for(int i=0;i<=24;i++)
    {
       int xxx=i*www+border;
       painter.drawLine(xxx,borderUP,xxx,borderUP+hhh);
       painter.drawText(xxx-14,borderUP+hhh,50,30,Qt::AlignLeft,QString::number((TimeStartPoint+i)%24)+":00");
       if(i<24)
       {
           //painter.drawRect(xxx,borderUP,www,hhh);
           if(i+TimeStartPoint <TimeDarkStartPoint ||i+TimeStartPoint>=TimeDawnStartPoint)
           painter.fillRect(QRect(QPoint(xxx+1,borderUP),QPoint(xxx+www,borderUP+hhh/2)), QBrush(Qt::yellow));
           else
           painter.fillRect(QRect(QPoint(xxx+1,borderUP),QPoint(xxx+www,borderUP+hhh/2)), QBrush(Qt::gray));
       }
    }

    //painter.end();
}
