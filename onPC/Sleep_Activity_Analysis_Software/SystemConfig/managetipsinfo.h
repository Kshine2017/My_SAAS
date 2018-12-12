#ifndef MANAGETIPSINFO_H
#define MANAGETIPSINFO_H

#include <QDialog>
#include <QTextEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
/*
类描述：管理建议信息，包括新增，删除和修改。
关键接口：无
*/
class ManageTipsInfo : public QDialog
{
    Q_OBJECT
public:
    explicit ManageTipsInfo(QWidget *parent = 0);
    //
    QTextEdit* textedit;
    QComboBox* cmbox;
    QLabel* lb_level;
    QLabel* lb_advice;

    QPushButton* btn_save;
    QPushButton* btn_cancel;

    //初始化
    void init_all();

signals:

public slots:
    void slot_btn_cancel();
    void slot_btn_save();
};

#endif // MANAGETIPSINFO_H
