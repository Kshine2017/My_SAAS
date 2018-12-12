#ifndef SWITCHPATIENT_H
#define SWITCHPATIENT_H

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include "graphplot/titlebar.h"
#include <QLabel>
#include "DataBase/qmysqlquerymodel.h"

/*
类描述：界面类，切换对象
*/
class SwitchPatient : public QDialog
{
    Q_OBJECT
public:
    explicit SwitchPatient(QWidget *parent = 0);
    int rowFlag;            //表格行号
    QTableView* tableview;  //表格
    QLabel* lb_backcolor;   //背景颜色
    QPushButton* btn_save;  //保存
    QPushButton* btn_cancel;//取消
    //布局
    QHBoxLayout* ly_H_table;
    QHBoxLayout* ly_H_Button;
    QVBoxLayout* ly_V_ALL;
    QMySQlQueryModel* model;
    //初始化---------------------------------
    void init_all();
    void get_tabledata();
signals:
    void signal_Sure();
private slots:
    void slot_btn_save();
    void slot_btn_cancel();
    void slot_doubleclick(QModelIndex index);
    void slot_click(QModelIndex index);
};

#endif // SWITCHPATIENT_H
