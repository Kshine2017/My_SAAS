#ifndef EXPORTLIST_H
#define EXPORTLIST_H

#include <QDialog>
#include <QLabel>
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
class ExportList : public QDialog
{
    Q_OBJECT
public:
    explicit ExportList(QString ID, QString Name, QWidget *parent = 0);
    QLabel* lb_test;
    QString IIIID;
private:
    //table
    QTableView* tableView;      //表格
    QVBoxLayout* mainLayout;    //主布局
    QHBoxLayout* labelLayout;   //观察对象信息布局
    QHBoxLayout* btnLayout;     //按钮布局

    QLabel* labelID;            //观察对象ID
    QLabel* labelName;          //观察对象姓名

    QPushButton* btnSure;       //确认打开按钮
    QPushButton* btnCancel;     //取消按钮

    /**
     * @brief init  初始化布局
     */
    void init();
    /**
     * @brief initPaintInfo     初始化观察对象信息
     * @param ID                ID号
     * @param Name              姓名
     */
    void initPaintInfo(QString ID,QString Name);
signals:

public slots:
    void slot_openPDF(QModelIndex index);

};

#endif // EXPORTLIST_H
