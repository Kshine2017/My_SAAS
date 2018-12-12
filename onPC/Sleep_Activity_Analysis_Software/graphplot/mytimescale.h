#ifndef MYTIMESCALE_H
#define MYTIMESCALE_H
#include <QFrame>


/*
类描述：自定义控件，时间刻度
关键接口：
    无
*/
class MyTimeScale:public QFrame
{
    Q_OBJECT
public:
    MyTimeScale(QWidget *parent = 0);
    int border;
    int TimeStartPoint;         //时间起始点 12点
    int TimeDarkStartPoint;     //黑夜开始点
    int TimeDawnStartPoint;     //早上开始点
    int TimeOverPoint;          //时间结束点12（36%24）

protected:
    virtual void paintEvent(QPaintEvent *ev);  //控件重绘

};

#endif // MYTIMESCALE_H
