#ifndef STATISTICS_H
#define STATISTICS_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

#include <QVBoxLayout>
#include <QHBoxLayout>



/*
类描述：自定义控件，一天的数据展示
关键接口：
        无
*/
class Statistics : public QWidget
{
    Q_OBJECT
public:
    explicit Statistics(QWidget *parent = 0, int thisWWW=600, int thisHHH=600, int flag_Day=0);
    int Flag_Day;                   //天标记
    QWidget *parentthis;            //父类指针
    QLabel* lb_DateName;            //日期
    QLabel* lb_DateValue;

    QLabel* lb_TotalSleepTimesName; //总睡觉次数
    QLabel* lb_TotalSleepTimesValue;

    QLabel* lb_TotalSleepTimeName;  //总睡觉时长
    QLabel* lb_TotalSleepTimeValue;

    QLabel* lb_DeepPercentName;     //深睡百分比
    QLabel* lb_DeepPercentValue;

    QHBoxLayout* ly_H_Title;        //布局
    QVBoxLayout* ly_V_all;

    QPushButton* btn_Tips;          //建议，评价按钮
    int TimesOfSleep;               //入睡次数

signals:

public slots:
    void slot_btn_advice();         //按钮功能 槽
    void slot_btn_TEXT(bool ok);    //按钮的文字修改 槽
};

#endif // STATISTICS_H
