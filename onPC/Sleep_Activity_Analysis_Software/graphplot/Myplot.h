#ifndef TEMPERATUREPLOT_H
#define TEMPERATUREPLOT_H

#include <QWidget>
#include "qcustomplot.h"
#define LENGTH_NUM 2880


/*
类描述：自定义控件，数据展示
关键接口：
    void display_Act(const QVector<double> &Ystorge, int graph);       //行动
    void display_Light(const QVector<double>& Ystorge, int graph);     //光强
    void display_voice(const QVector<double> &Ystorge, int graph);     //声音
*/
class MyPlot : public QWidget
{
    Q_OBJECT
    void insertData(double tem);//此函数私有化，不给外界使用
public:
    explicit MyPlot(QWidget *parent = nullptr);
    ~MyPlot();

    //公开接口：单个数据源Ystorge，指定图层graph
    void display_Act(const QVector<double> &Ystorge, int graph);       //行动
    void display_Light(const QVector<double>& Ystorge, int graph);     //光强
    void display_voice(const QVector<double> &Ystorge, int graph);     //声音
    void display_DeepSleep(QVector<double> &Ystorge, int graph);       //背景
    void display_ShallowSleep(QVector<double> &Ystorge, int graph);    //背景
    void display_Max144(const QVector<double> &Ystorge, int graph);    //上边界
    void display_analyzeBar();                                         //分析条
    void clearDataKshine();                                            //清空
    //QWidget* Widget_main;                 //这个类的主界面
    QCustomPlot* temPIC;
    double* Storge;                         //存储之前的温度值
    /**
     * @brief MyPlot::setGraphVisible   设置customPlot图层曲线是否可见
     * @param b 是否可见
     * @param graph 曲线的图层
     */
    void setGraphVisible(bool b,int graph);                             //设置图层是否可见
    //int Length;
protected:
    //声明
    void mouseDoubleClickEvent(QMouseEvent*);

signals:

public slots:
};

#endif
