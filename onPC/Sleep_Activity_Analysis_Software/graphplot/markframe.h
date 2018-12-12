#ifndef MARKFRAME_H
#define MARKFRAME_H


#include <QFrame>
#include <QVector>  //动态数组
#include <QPoint>


struct struct_Duan
{
  int startX;
  int endX;
};

/*
类描述：自定义控件，标记条
关键接口：
    void clearData();
    void autoMaticMark(const QVector<struct_Duan>& autoDUAN);
*/

class MarkFrame : public QFrame
{
    Q_OBJECT
private:

    struct_Duan temdata;                    //全局临时数据
    int Press_Flag;                         //鼠标按下标志
    int Move_Flag;                          //鼠标移动标志
    int counter;                            //计数
    int X_isInOneDuan(int the_X);           //判断x是否在一个段里面
    int DeleteMidDuan_ByLast();             //删除中间临时段
public:
    //int counter_DUAN;                     //段的个数
    QVector<struct_Duan> DUAN ;             //动态数组，存储  标记段,(起始X和结束X)
    explicit MarkFrame(QWidget *parent = 0);
    ~MarkFrame();
    void clearData();                       //清空数据
    void autoMaticMark(const QVector<struct_Duan>& autoDUAN);   //自动绘制标记
protected:
    virtual void mousePressEvent(QMouseEvent* ev);              //鼠标按下
    virtual void mouseMoveEvent(QMouseEvent* ev);               //鼠标移动
    virtual void mouseReleaseEvent(QMouseEvent* ev);            //鼠标松开
    virtual void paintEvent(QPaintEvent *ev);                   //控件重绘
signals:

public slots:
};

#endif // MARKFRAME_H
