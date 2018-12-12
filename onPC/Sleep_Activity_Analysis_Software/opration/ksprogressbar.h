#ifndef KSPROGRESSBAR_H
#define KSPROGRESSBAR_H

#include <QObject>
#include <QDialog>
#include <QLabel>
/*
类描述：功能类，分段进度条
关键接口：
        void SetPercentage(int flag,double PPP);
*/
class KSProgressBar : public QDialog
{
    Q_OBJECT
public:
    explicit KSProgressBar(QWidget *parent = 0);
    //进度条图片
    QLabel* lb_front;   //空框
    QLabel* lb_mid;     //档条
    QLabel* lb_back;    //色条
    //背景图片
    QLabel* lb_background;
    //文字
    QLabel * tip_label;
    void SetPercentage(int flag,double PPP);//写入数据

    qint64 tem;         //记录大小
    int tem_int;        //记录个数
    qint64 TotalSize;   //总的大小
    int TotalNumber;    //总的个数
    int workflag;       //第几阶段
    ~KSProgressBar();

private:
    int YYY;            //进度条向下位移
signals:
public slots:
    void slot_setTotalSize(qint64 totalsize);//设置总大小
    void slot_showSizeProgress(qint64 size); //显示大小进度

    void slot_setToTalNumber(int totalnum);  //设置总个数
    void slot_showNumProgress(int num);      //显示个数进度

    void slot_clearTemData();                //清除临时记录
    void slot_addWorkFlag();                 //切换到下一份工作
};

#endif // KSPROGRESSBAR_H
