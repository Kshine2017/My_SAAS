#ifndef LOGINSECONDWIDGET_H
#define LOGINSECONDWIDGET_H

#include <QDialog>
#include <QPushButton>

/*
类描述：窗口类，首次登录，跳出的“新建，选择，随便”窗口。
关键接口：
        无
*/
class LoginSecondWidget : public QDialog
{
    Q_OBJECT
public:
    explicit LoginSecondWidget(QWidget *parent = 0);
    ~LoginSecondWidget();
    QPushButton* btn_CreatePatient; //新建观测对象
    QPushButton* btn_ChoosePatient; //选择对象
    QPushButton* btn_NO_OPERATION;  //随便逛逛
signals:
    void signal_CreatePatient();
    void signal_ChoosePatient();
    void signal_getinwindow();
public slots:
    void slot_CreatePatient();
    void slot_ChoosePatient();
    void slot_No_Operation();
};

#endif // LOGINSECONDWIDGET_H
