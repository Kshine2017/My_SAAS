#include "enlargeplot.h"

EnlargePlot::EnlargePlot(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("活动曲线");
    setFixedSize(1200,400);
    setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_DeleteOnClose);
    init();
}

void EnlargePlot::init()
{
    customPlot = new QCustomPlot(this);
    customPlot->setFixedSize(1200,400);
    //设置可缩放，拖拽
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |
                                QCP::iSelectAxes |QCP::iSelectLegend |
                                QCP::iSelectPlottables);

    //设置完数据后可以对x，y轴进行设置
    customPlot->xAxis->setLabel("时间");
    customPlot->yAxis->setLabel("活动值");
    //设置y轴的范围
    customPlot->yAxis->setRange(0,500);

    //设置X时间坐标轴
    //配置底部轴以显示日期而不是编号：
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("hh::mm");
    customPlot->xAxis->setTicker(dateTicker);
    customPlot->xAxis->setRange(4 * 3600 , 28 * 3600);
}

void EnlargePlot::disPlay_data(const QVector<double>& Ystorge,int graph,bool visiable)
{
    QVector<double> Xstorge(LENGTH_NUM);
    //装载X/Y数据
    //时间坐标轴起始点为8：00（秒为单位），
    for(int i=0;i<LENGTH_NUM;i++)
    {
        //每30秒一个点
        Xstorge[i] = i * 30 + 4 *3600;
    }
    //添加曲线
    for(int ii=0;ii<graph+1-customPlot->graphCount();ii++)customPlot->addGraph();


    QPen pen;
    if(graph==0)pen.setColor(QColor(0, 0, 255, 255));
    if(graph==1)pen.setColor(QColor(255, 0, 0, 255));
    if(graph==2)pen.setColor(QColor(255, 255,0, 255));
    pen.setWidthF(2.0);//2.5

    customPlot->graph(graph)->setPen(pen);
    customPlot->graph(graph)->setData(Xstorge,Ystorge);
    customPlot->graph(graph)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond, 1));
    customPlot->graph(graph)->setVisible(visiable);
    customPlot->replot();
}


