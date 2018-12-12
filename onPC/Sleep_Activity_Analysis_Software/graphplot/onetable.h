#ifndef ONETABLE_H
#define ONETABLE_H

#include <QWidget>
#include <QTableWidget>
#define TABLEWWW 300
#define TABLEHHH 60


/*
类描述：自定义控件，单段睡眠数据
关键接口：
        无
*/
class OneTable : public QWidget
{
    Q_OBJECT
public:
    explicit OneTable(int outWWW, int outHHH, QWidget *parent = 0, int Flag_day=0, int Flag_DUAN=0);
    QTableWidget* tablewidget;
    QString str0;                           //睡眠开始
    QString str1;                           //睡眠结束
    QString str2;                           //浅睡时长
    QString str3;                           //深睡时长
    void getDataFromDC(int Flag_Day, int Flag_DUAN);//从数据容器中得到数据，并存到str0~str3中。
    int KshineStart;
    int KshineEnd;
    int LenthOfShalow;
    int LenthOfDeep;
signals:

public slots:
};

#endif // ONETABLE_H
