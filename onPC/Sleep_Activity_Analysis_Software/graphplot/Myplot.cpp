#include "Myplot.h"

#include <QGridLayout>
#include <QDebug>

#define HOUR_ST 12
#define HOUR_ED 36
#define TEMP_H 50
#define TEMP_L 0

#define WEIDGET_W  1600
#define WEIDGET_H  150
MyPlot::MyPlot(QWidget *parent) : QWidget(parent)
{

    //Widget_main =new QWidget(this);

    Storge =new double[LENGTH_NUM];
    memset(Storge,0,sizeof(double)*LENGTH_NUM);//清空

    temPIC = new QCustomPlot(this);
    //位置和尺寸
    //temPIC->setGeometry(15,40,WEIDGET_W,WEIDGET_H); //

    QGridLayout* gridlayout=new QGridLayout();
    gridlayout->addWidget(temPIC);
    this->setLayout(gridlayout);
    //setStyleSheet("background-color:white;");

    //X和Y轴
    //  temPIC->xAxis->setLabel("时间/min");
    //  temPIC->xAxis->setLabelColor(Qt::white);
    temPIC->xAxis->setVisible(0);
    temPIC->xAxis->setRange(HOUR_ST,HOUR_ED);
    temPIC->xAxis->grid()->setSubGridVisible(false);
    temPIC->xAxis->setBasePen(QPen(QColor(255,255,255)));
    temPIC->xAxis->setTickLabelColor(QColor(255,255,255));
    temPIC->xAxis->setTickPen(QPen(QColor(255,255,255)));
    temPIC->xAxis->setSubTickPen(QPen(QColor(255,255,255)));

    //    temPIC->yAxis->setLabel("温度/℃");
    //    temPIC->yAxis->setLabelColor(Qt::white);
    //temPIC->yAxis->setRange(TEMP_L,TEMP_H);
    temPIC->yAxis->setVisible(0);
    temPIC->yAxis->grid()->setSubGridVisible(false);
    temPIC->yAxis->setBasePen(QPen(QColor(255,255,255)));
    temPIC->yAxis->setTickLabelColor(QColor(255,255,255));
    temPIC->yAxis->setTickPen(QPen(QColor(255,255,255)));
    temPIC->yAxis->setSubTickPen(QPen(QColor(255,255,255)));

    //设置可缩放，拖拽
    // temPIC->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |QCP::iSelectLegend | QCP::iSelectPlottables);
}

MyPlot::~MyPlot()
{
    //析构
}

void MyPlot::insertData(double tem)
{
    QVector<double> Xstorge(LENGTH_NUM);
    QVector<double> Ystorge(LENGTH_NUM);
    //更新数据（前插后弃）
    for(int i=0;i < LENGTH_NUM - 1 ;i++)
    {
        Storge[LENGTH_NUM-1-i]=Storge[LENGTH_NUM-2-i];

    }
    Storge[0]=tem;//放入最新的数据

    //装载数据
    for(int i=0;i<30;i++)
    {
        Xstorge[i]=i+1;
        Ystorge[i]=Storge[i];
    }

    //添加曲线
    for(int ii=0;ii<0+1-temPIC->graphCount();ii++)temPIC->addGraph();//第0层
    QPen pen;
    pen.setColor(QColor(255, 200, 20, 200));
    pen.setStyle(Qt::DashLine);//虚线
    pen.setWidthF(2.0);//2.5

    temPIC->graph(0)->setPen(pen);
    //temPIC->graph(0)->setLineStyle(QCPGraph::lsLine);

    // 设置渐变色
    QLinearGradient linear(QPointF(0, 234), QPointF(0, 0));
    linear.setColorAt(0, Qt::blue);
    linear.setColorAt(0.5,Qt::yellow);
    linear.setColorAt(1, Qt::red);
    temPIC->graph(0)->setBrush(QBrush(linear));
    //temPIC->graph(0)->setBrush(QBrush(QColor(255,200,20,160)));//填充 rgb+不透明度
    //temPIC->graph(0)->setScatterStyle(QCPScatterStyle(QPixmap(":/C:/Users/Administrator/Desktop/3.png")));
    temPIC->graph(0)->setData(Xstorge,Ystorge);
    temPIC->replot();
}

void MyPlot::display_Act(const QVector<double>& Ystorge,int graph)
{
    qDebug()<<"绘制活动图"<<Ystorge.length();
    QVector<double> Xstorge(LENGTH_NUM);
    //装载X数据
    for(double i=0;i<LENGTH_NUM;i++)
    {
        Xstorge[i]=i*(HOUR_ED-HOUR_ST)/LENGTH_NUM+HOUR_ST;
    }
    //添加曲线
    for(int ii=0;ii<graph+1-temPIC->graphCount();ii++)temPIC->addGraph();

    //temPIC->graphCount();
    QPen pen;
    pen.setColor(QColor(0, 0, 255, 255));
    pen.setWidthF(2.0);//2.5

    temPIC->graph(graph)->setPen(pen);
    temPIC->graph(graph)->setData(Xstorge,Ystorge);
    temPIC->replot();
}

void MyPlot::display_Light(const QVector<double>& Ystorge, int graph)
{
    qDebug()<<"绘制光强图"<<Ystorge.length();
    QVector<double> Xstorge(LENGTH_NUM);
    //装载X数据
    for(double i=0;i<LENGTH_NUM;i++)
    {
        Xstorge[i]=i*(HOUR_ED-HOUR_ST)/LENGTH_NUM+HOUR_ST;
    }
    //添加曲线
    for(int ii=0;ii<graph+1-temPIC->graphCount();ii++)temPIC->addGraph();
    QPen pen;
    pen.setColor(QColor(255,0,0,255));
    pen.setWidthF(2.0);//2.5

    temPIC->graph(graph)->setPen(pen);
    temPIC->graph(graph)->setData(Xstorge,Ystorge);
    temPIC->replot();
}

void MyPlot::display_voice(const QVector<double>& Ystorge, int graph)
{
    qDebug()<<"绘制声音图"<<Ystorge.length();
    QVector<double> Xstorge(LENGTH_NUM);
    //装载X数据
    for(double i=0;i<LENGTH_NUM;i++)
    {
        Xstorge[i]=i*(HOUR_ED-HOUR_ST)/LENGTH_NUM+HOUR_ST;
    }
    //添加曲线
    for(int ii=0;ii<graph+1-temPIC->graphCount();ii++)temPIC->addGraph();
    QPen pen;
    pen.setColor(QColor(255,255,0,255));
    pen.setWidthF(2.0);//2.5

    temPIC->graph(graph)->setPen(pen);
    temPIC->graph(graph)->setData(Xstorge,Ystorge);
    temPIC->replot();
}




void MyPlot::display_DeepSleep(QVector<double> &Ystorge, int graph)
{

    qDebug()<<"绘制深度睡眠图"<<Ystorge.length();
    QVector<double> Xstorge(LENGTH_NUM);
    QVector<double> YYYstorge(LENGTH_NUM);
    //装载X数据
    for(double i=0;i<LENGTH_NUM;i++)
    {
        Xstorge[i]=i*(HOUR_ED-HOUR_ST)/LENGTH_NUM+HOUR_ST;
        YYYstorge[i]=Ystorge[i/20];
    }

    //添加曲线
    for(int ii=0;ii<graph+1-temPIC->graphCount();ii++)temPIC->addGraph();
    QPen pen;
    pen.setColor(QColor(0,0,0,255));
    pen.setWidthF(0.1);//2.5
    temPIC->graph(graph)->setPen(pen);
    // 设置渐变色
    QLinearGradient linear(QPointF(0, 234), QPointF(0, 0));
    linear.setColorAt(0, Qt::gray);
    //        linear.setColorAt(0.5,Qt::yellow);
    linear.setColorAt(1, Qt::gray);
    temPIC->graph(graph)->setBrush(QBrush(linear));
    temPIC->graph(graph)->setData(Xstorge,YYYstorge);
    temPIC->replot();
}

void MyPlot::display_ShallowSleep(QVector<double> &Ystorge, int graph)
{
    qDebug()<<"绘制浅度睡眠图"<<Ystorge.length();
    QVector<double> Xstorge(LENGTH_NUM);
    QVector<double> YYYstorge(LENGTH_NUM);
    //装载X数据
    for(double i=0;i<LENGTH_NUM;i++)
    {
        Xstorge[i]=i*(HOUR_ED-HOUR_ST)/LENGTH_NUM+HOUR_ST;
        YYYstorge[i]=Ystorge[i/20];
    }

    //添加曲线
    for(int ii=0;ii<graph+1-temPIC->graphCount();ii++)temPIC->addGraph();
    QPen pen;
    pen.setColor(QColor(0,0,0,255));
    pen.setWidthF(0.1);//2.5
    temPIC->graph(graph)->setPen(pen);
    // 设置渐变色
    QLinearGradient linear(QPointF(0, 234), QPointF(0, 0));
    linear.setColorAt(0, Qt::lightGray);
    //        linear.setColorAt(0.5,Qt::yellow);
    linear.setColorAt(1, Qt::lightGray);
    temPIC->graph(graph)->setBrush(QBrush(linear));
    temPIC->graph(graph)->setData(Xstorge,YYYstorge);
    temPIC->replot();
}

void MyPlot::display_Max144(const QVector<double> &Ystorge, int graph)
{
    qDebug()<<"绘制144压缩数据图"<<Ystorge.length();
    QVector<double> Xstorge(LENGTH_NUM);
    QVector<double> YYYstorge(LENGTH_NUM);
    //装载X数据
    for(double i=0;i<LENGTH_NUM;i++)
    {
        Xstorge[i]=i*(HOUR_ED-HOUR_ST)/LENGTH_NUM+HOUR_ST;
        YYYstorge[i]=Ystorge[i/20];
    }

    //添加曲线
    for(int ii=0;ii<graph+1-temPIC->graphCount();ii++)temPIC->addGraph();
    QPen pen;
    pen.setColor(QColor(0,0,0,255));
    pen.setWidthF(3.0);//2.5

    temPIC->graph(graph)->setPen(pen);
    temPIC->graph(graph)->setData(Xstorge,YYYstorge);
    temPIC->replot();

}



void MyPlot::display_analyzeBar()
{

}

void MyPlot::clearDataKshine()
{
    for(int i=0;i<temPIC->graphCount();i++)
    {
        temPIC->graph(i)->data().data()->clear();
    }
    temPIC->replot();
}

/**
 * @brief MyPlot::setGraphVisible   设置customPlot图层曲线是否可见
 * @param b 是否可见
 * @param graph 曲线的图层
 */
void MyPlot::setGraphVisible(bool b, int graph)
{
    temPIC->graph(graph)->setVisible(b);
    temPIC->replot();
}


void MyPlot::mouseDoubleClickEvent(QMouseEvent *)
{
    qDebug()<<"点击事件";
}
