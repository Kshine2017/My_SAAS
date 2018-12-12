#ifndef ENLARGEPLOT_H
#define ENLARGEPLOT_H

#include <QWidget>
#include <QDialog>
#include "qcustomplot.h"
#define LENGTH_NUM 2880

/*
类描述：功能类，单独展示活动数据
关键接口：
    void disPlay_Active_data(const QVector<double> &Ystorge);
*/
class EnlargePlot : public QDialog
{
    Q_OBJECT
public:
    explicit EnlargePlot(QWidget *parent = 0);

    QCustomPlot *customPlot;                                 //控件

public:
    void disPlay_data(const QVector<double> &Ystorge, int graph, bool visiable);//展示活动数据
private:

    void init();                                            //初始化
signals:

public slots:
};

#endif // ENLARGEPLOT_H
