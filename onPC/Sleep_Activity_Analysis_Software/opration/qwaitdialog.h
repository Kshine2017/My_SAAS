#ifndef QWAITDIALOG_H
#define QWAITDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QMovie>
#include <QFrame>
/*
类描述：功能类，等待窗口
关键接口：
        无
*/
class QWaitDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QWaitDialog(QWidget *parent,const QString &title);
    ~QWaitDialog();
private:
    QMovie *movie;              //播放类
    QLabel *label;              //图片，gif格式
    QLabel * tip_label;         //文字
    QFrame * background;        //背景
    qint64 tem;                 //大小数值
    int tem_int;                //个数数值
    qint64 TotalSize;           //总大小
    int TotalNumber;            //总个数
signals:

public slots:
    void slot_showProgress(qint64 progress);    //显示进度
    void slot_setTotalSize(qint64 totalsize);   //设置总大小
    void slot_showSizeProgress(qint64 size);    //显示大小进度
    void slot_setToTalNumber(int totalnum);     //设置总个数
    void slot_showNumProgress(int num);         //显示个数进度
};

#endif // QWAITDIALOG_H
