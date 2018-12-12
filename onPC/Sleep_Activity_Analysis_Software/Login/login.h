#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

/*
类描述：窗口类，登录界面
关键接口：
        无
*/
class Login : public QDialog
{
    Q_OBJECT
public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    QWidget* widget;
    QPushButton* btn_login;//登录
    QPushButton* btn_regist;//注册
    QPushButton* btn_quit;//退出

    QLineEdit* le_account;//账号
    QLineEdit* le_pwd;    //密码

    QLabel* lb_account;
    QLabel* lb_pwd;

    QLabel* lb_pic;
    QLabel* lb_title;

    void init_all();

    int thisWIDTH;
    int thisHEIGHT;

signals:

public slots:

    void slot_login();

    void slot_regist();

    void slot_quit();

};

#endif // LOGIN_H
