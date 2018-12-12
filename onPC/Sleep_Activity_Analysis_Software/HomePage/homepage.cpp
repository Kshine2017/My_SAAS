#include "homepage.h"
#include <QDebug>
#include <QHBoxLayout>
#define Kshine 1.5 //大小(1/kshine* 屏幕)
#define KshineSpeed 3 //移动速度
Homepage::Homepage(QWidget *parent, int WWW, int HHH):QWidget(parent)
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
    qDebug()<<"页面：概览  尺寸："<<width()<<height();

}







