#include "qwaitdialog.h"

#include <QDebug>
#define WWW 200
#define HHH 150
QWaitDialog::QWaitDialog(QWidget *parent,const QString &title) :
    QDialog(parent)
{
    this->setFixedSize(WWW, HHH);
    //设置透明度
    //this->move(0,768-HHH-40);
    this->setWindowOpacity(0.8);
    setAttribute(Qt::WA_DeleteOnClose);
    //取消对话框标题
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    this->setStyleSheet("background-color:transparent;");
    tem=0;
    tem_int=0;
    TotalSize=0;
    TotalNumber=0;
    //设置背景色为淡蓝色,顶层窗口做不出圆角来
    background = new QFrame(this);
    background->setStyleSheet("background-color: rgb(55, 135,215);border-radius:10px;");
    background->setGeometry(0, 0, WWW, HHH);

    label = new QLabel(background);

    label->setStyleSheet("background-color:transparent;");

    label->setGeometry(0, 0, WWW, HHH);
    movie = new QMovie(":/picture/gif/timg.gif");
    label->setScaledContents(true);
    label->setMovie(movie);

    tip_label = new QLabel(background);
    tip_label->setFont(QFont("宋体",16,75));
    tip_label->setText(title);
    tip_label->setStyleSheet("color: black;background-color: transparent;");
    tip_label->setGeometry(20, HHH-30, 300, 30);

    movie->start();
    show();
}

QWaitDialog::~QWaitDialog(){

    qDebug()<<"析构-qWaitDialog";
    delete label;
    delete movie;
    delete tip_label;
    delete background;
}

void QWaitDialog::slot_showProgress(qint64 progress)
{

    QString msg="进度：";
    if(progress==100)
    {
        tip_label->setText("已完成！");
        tem=0;
        //close();
    }
    else
    {
        msg+=QString::number(tem*100.0/144.0,'f',2)+"%";
        tip_label->setText(msg);
        tem++;
    }
}

void QWaitDialog::slot_setTotalSize(qint64 totalsize)
{
    TotalSize=totalsize;
}

void QWaitDialog::slot_setToTalNumber(int totalnum)
{
    TotalNumber=totalnum;
}

void QWaitDialog::slot_showNumProgress(int num)
{
    QString msg="进度：";
    tem_int+=num;
    if(TotalNumber)
        msg+=QString::number(tem_int*100.0/TotalNumber,'f',2)+"%";
    tip_label->setText(msg);
}

void QWaitDialog::slot_showSizeProgress(qint64 size)
{
    QString msg="进度：";
    tem+=size;
    if(TotalSize)
    msg+=QString::number(tem*100.0/TotalSize,'f',2)+"%";
    tip_label->setText(msg);
}
