#include "loginsecondwidget.h"
#include <QHBoxLayout>
#include <QDebug>
LoginSecondWidget::LoginSecondWidget(QWidget *parent) : QDialog(parent)
{
    setFixedSize(600,250);
    setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("选择监测对象");
    btn_CreatePatient = new QPushButton(this);
    btn_CreatePatient->setFixedSize(150,150);
    btn_ChoosePatient = new QPushButton(this);
    btn_ChoosePatient->setFixedSize(150,150);
    btn_NO_OPERATION  = new QPushButton(this);
    btn_NO_OPERATION ->setFixedSize(150,150);
    QFont font;
    font.setFamily("楷体");font.setBold(true);font.setPointSize(22);
    btn_CreatePatient->setFont(font);
    btn_ChoosePatient->setFont(font);
    btn_NO_OPERATION->setFont(font);

    btn_CreatePatient->setText("新建对象");
    btn_ChoosePatient->setText("选择对象");
    btn_NO_OPERATION->setText("随便逛逛");
    btn_CreatePatient->setStyleSheet("QPushButton{background-color:rgb(0,162,232);}");
    btn_ChoosePatient->setStyleSheet("QPushButton{background-color:rgb(255,242,0);}");
    btn_NO_OPERATION->setStyleSheet("QPushButton{background-color:rgb(255,127,39);}");

    QHBoxLayout* ly_H_btn = new QHBoxLayout;
    ly_H_btn->addWidget(btn_CreatePatient);
    ly_H_btn->addWidget(btn_ChoosePatient);
    ly_H_btn->addWidget(btn_NO_OPERATION );

    connect(btn_CreatePatient,SIGNAL(clicked()),this,SLOT(slot_CreatePatient()));
    connect(btn_ChoosePatient,SIGNAL(clicked()),this,SLOT(slot_ChoosePatient()));
    connect(btn_NO_OPERATION ,SIGNAL(clicked()),this,SLOT(slot_No_Operation ()));

    this->setLayout(ly_H_btn);
    show();
}

LoginSecondWidget::~LoginSecondWidget()
{
    qDebug()<<"析构---CCN界面";
}

void LoginSecondWidget::slot_CreatePatient()
{
    emit signal_CreatePatient();//启动外部操作
    emit signal_getinwindow();
    close();
}

void LoginSecondWidget::slot_ChoosePatient()
{
    emit signal_ChoosePatient();//启动外部操作
    emit signal_getinwindow();
    close();
}

void LoginSecondWidget::slot_No_Operation()
{
    emit signal_getinwindow();
    close();
}
