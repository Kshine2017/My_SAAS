#include "login.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>
Login::Login(QWidget *parent) : QDialog(parent)
{
    QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();也可以
    thisWIDTH =desktop->width();
    thisHEIGHT=desktop->height();
    if(thisWIDTH ==1920||thisWIDTH ==1924)//测试
        thisHEIGHT=1080;//测试
//    thisWIDTH =1366;
//    thisHEIGHT =768;
    qDebug()<<"屏幕大小："<<thisWIDTH<<thisHEIGHT;
    setFixedSize(thisWIDTH,thisHEIGHT);
    this->setWindowFlags(Qt::FramelessWindowHint);
   // setAttribute(Qt::WA_DeleteOnClose);


    init_all();



    le_account->setText("BB-Care");
    le_pwd->setText("123456");
}

Login::~Login()
{
    qDebug()<<"析构--登录界面";
}

void Login::init_all()
{
    lb_pic      =   new QLabel(this);
    QPixmap pixmap;
    lb_pic->setFixedSize(thisWIDTH,thisHEIGHT);
    pixmap.load(":/picture/login/loginbackground.jpg");
    pixmap.scaled(lb_pic->size(), Qt::IgnoreAspectRatio);
    lb_pic->setScaledContents(true);
    lb_pic->setPixmap(pixmap);

//-------------------------------------------------
    widget = new QWidget(this);
    widget->setFixedSize(400,400);
    widget->setStyleSheet("background-color:rgba(200,200,200,100);");
   // widget->setWindowOpacity(0.1);

    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);
    QFont font1;
    font1.setFamily("楷体");
    font1.setBold(true);
    font1.setPointSize(20);
    QFont font2;
    font2.setPointSize(20);

    lb_title      =   new QLabel(widget);lb_title->setFont(QFont("楷体",32,75));lb_title->setText("SAAS2018");lb_title->setFixedHeight(60);
    lb_title->setPalette(pa);

    lb_account  =   new QLabel(widget);
    lb_account ->setText("账号：");
    lb_account->setPalette(pa);
    lb_account->setFont(font1);


    lb_pwd      =   new QLabel(widget);
    lb_pwd     ->setText("密码：");
    lb_pwd->setPalette(pa);
    lb_pwd->setFont(font1);

    le_account  =   new QLineEdit(widget);le_account->setFixedSize(250,30);le_account->setFont(font2);//le_account->setEchoMode(QLineEdit::Password);
    le_pwd      =   new QLineEdit(widget);le_pwd->setFixedSize(250,30);le_pwd->setFont(font2);le_pwd->setEchoMode(QLineEdit::Password);

    btn_login   =   new QPushButton(widget);btn_login->setFixedSize(100,40);btn_login->setText("登录");
    connect(btn_login,SIGNAL(clicked()),this,SLOT(slot_login()));
    btn_quit    =   new QPushButton(widget);btn_quit->setFixedSize(100,40);btn_quit->setText("退出");
    connect(btn_quit,SIGNAL(clicked()),this,SLOT(slot_quit()));

    btn_regist  =   new QPushButton(this);btn_regist->setFixedSize(100,40);btn_regist->setText("注册用户");


    //-----------------
    lb_title->setStyleSheet("background-color:rgba(200,200,200,0);");
    lb_account->setStyleSheet("background-color:rgba(200,200,200,0);");
    lb_pwd->setStyleSheet("background-color:rgba(200,200,200,0);");
    le_account->setStyleSheet("background-color:rgba(200,200,200,0);");
    le_pwd->setStyleSheet("background-color:rgba(200,200,200,0);");

    //------------------
    btn_regist->setStyleSheet("background-color:rgba(200,200,200,100);");


    QHBoxLayout* ly_H_account = new QHBoxLayout;
    QHBoxLayout* ly_H_pwd = new QHBoxLayout;
    QHBoxLayout* ly_H_btn = new QHBoxLayout;

    QVBoxLayout* ly_V_all   = new QVBoxLayout;


    ly_H_account->addStretch();ly_H_account->addWidget(lb_account);ly_H_account->addWidget(le_account);ly_H_account->addSpacing(60);ly_H_account->addStretch();
    ly_H_pwd->addStretch();ly_H_pwd->addWidget(lb_pwd);ly_H_pwd->addWidget(le_pwd);ly_H_pwd->addSpacing(60);ly_H_pwd->addStretch();
    ly_H_btn->addStretch();ly_H_btn->addWidget(btn_login);ly_H_btn->addSpacing(60); ly_H_btn->addWidget(btn_quit);ly_H_btn->addStretch();

    ly_V_all->addStretch();
    ly_V_all->addWidget(lb_title,0,Qt::AlignHCenter);
    ly_V_all->addSpacing(30);
    ly_V_all->addLayout(ly_H_account);
    ly_V_all->addSpacing(30);
    ly_V_all->addLayout(ly_H_pwd);
    ly_V_all->addSpacing(30);
    ly_V_all->addLayout(ly_H_btn);
    ly_V_all->addStretch();

    //ly_V_all->setGeometry(QRect(0,0,200,200));
    widget->setLayout(ly_V_all);

    QHBoxLayout* ly_H_allllll   = new QHBoxLayout;
     ly_H_allllll->addStretch();ly_H_allllll->addWidget(widget);ly_H_allllll->addSpacing(60);
     this->setLayout(ly_H_allllll);
}

void Login::slot_login()
{
    qDebug()<<"点击登录";

    if(le_account->text()==QString("BB-Care")&&le_pwd->text()==QString("123456"))
    {
        QDialog::accept();
        this->close();
    }
    else
    {
        QMessageBox::information(this,"提示","账号或者密码错误",QMessageBox::Yes);
    }
}

void Login::slot_regist()
{

}

void Login::slot_quit()
{
    QDialog::reject();
}
