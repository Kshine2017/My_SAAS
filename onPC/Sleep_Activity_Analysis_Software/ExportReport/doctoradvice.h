#ifndef DOCTORADVICE_H
#define DOCTORADVICE_H

#include <QDialog>
#include <QTextEdit>
#include <QTableView>
#include <QLabel>
#include <QPushButton>
/*
类描述：医师建议输入界面类，可以对某一天的数据进行评价和给出建议
关键接口：无
*/
class DoctorAdvice : public QDialog
{
    Q_OBJECT
public:
    explicit DoctorAdvice(QWidget *parent = 0, int flag_Day =0);
    int Flag_Day;       //天标志，1~6
    QLabel* lb_tips;    //建议标签
    QLabel* lb_moTips;  //模板建议标签
    QTextEdit* textedit;//意见输入
    QTableView* tableview;  //模板意见展示
    QPushButton* btn_save;  //保存
    QPushButton* btn_cancel;//取消


signals:
    void signal_tips_ok(bool);//信号：通知父类是否有评价意见
public slots:
    void slot_btn_save();     //槽：保存评价意见
    void slot_btn_cancel();   //槽：取消
    void slot_doubleclick(QModelIndex index);//槽：双击事件
};

#endif // DOCTORADVICE_H
