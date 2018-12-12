#ifndef CURRENTPATIENT_H
#define CURRENTPATIENT_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDateEdit>
#include <QString>
#include <QComboBox>
#include "graphplot/titlebar.h"
#include "DataContainer/datacontainer.h"

/*
类描述：界面类，当前用户
可以修改用户信息
*/
class CurrentPatient : public QDialog
{
    Q_OBJECT
public:
    explicit CurrentPatient(QWidget *parent = 0);
    ~CurrentPatient();
    TitleBar *pTitleBar;                            //标题栏
    QLabel* lb_backcolor;                           //背景色
    DataContainer* dc;                              //数据容器
    //-------------------------------------------------------------------
    QLabel* lb_PatientInfo;                         //对象信息
    QLabel* lb_NUMBER;    QLabel* lb_NUMBER_value;  //ID
    QLabel* lb_NAME;        QLineEdit* le_NAME;     //姓名
    QLabel* lb_GANDER;      QComboBox * comB_GANDER;//QLineEdit* le_GANDER;//性别
    QLabel* lb_BRITHDAY;    QDateEdit* dateEdit;    QLineEdit* le_BRITHDAY;//生日
    QLabel* lb_AGE;         QLineEdit* le_AGE;      //年龄
    //-------------------------------------------------------------------
    QLabel* lb_WatcherInfo;                         //监护人信息
    QLabel* lb_watchername;     QLineEdit* le_watchername;  //监护人姓名
    QLabel* lb_watcherphone;    QLineEdit* le_watcherphone; //监护人电话
    QLabel* lb_watcheradress;   QLineEdit* le_watcheradress;//监护人地址
    QPushButton* btn_save;      //保存
    QPushButton* btn_cancel;    //取消

    //布局
    QHBoxLayout* ly_H_patientinfo;
    QHBoxLayout* ly_H_NUMBER;
    QHBoxLayout* ly_H_NAME;
    QHBoxLayout* ly_H_GANDER;
    QHBoxLayout* ly_H_BRITHDAY;
    QHBoxLayout* ly_H_AGE;
    //
    QHBoxLayout* ly_H_watcherinfo;
    QHBoxLayout* ly_H_watchername;
    QHBoxLayout* ly_H_watcherphone;
    QHBoxLayout* ly_H_watcheradress;
    //
    QHBoxLayout* ly_H_Button;

    QVBoxLayout* ly_V_ALL;
    //初始化------------------------------------------------------------------
    void init_Label_LineEdit();
    void init_Layout();
    void init_data();
private slots:
    void slot_btn_save();
    void slot_btn_cancel();
    void slot_calculateAge();
signals:
    void signal_sendQString(QString,QString,QString,QString,QString);
    void signal_Sure();

};

#endif // CURRENTPATIENT_H
