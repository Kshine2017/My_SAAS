#include "managetipsinfo.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "DataBase/infodaoimp.h"
#include <QDateTime>
ManageTipsInfo::ManageTipsInfo(QWidget *parent) : QDialog(parent)
{
    setFixedSize(600,400);
    setWindowTitle("管理建议文字");
    this->setWindowModality(Qt::WindowModal);
    this->setAttribute(Qt::WA_DeleteOnClose);
    init_all();
    show();
}

void ManageTipsInfo::init_all()
{
    lb_advice = new QLabel(this);lb_advice->setText("建言：");
    textedit = new QTextEdit(this);
    lb_level = new QLabel(this);lb_level->setText("等级：");
    cmbox   = new QComboBox(this);
    cmbox->insertItem(0,"高");
    cmbox->insertItem(1,"中");
    cmbox->insertItem(2,"低");
    cmbox->setCurrentIndex(1);

    btn_save = new QPushButton(this);btn_save->setText("保存");
    connect(btn_save,SIGNAL(clicked(bool)),this,SLOT(slot_btn_save()));

    btn_cancel = new QPushButton(this);btn_cancel->setText("取消");
    connect(btn_cancel,SIGNAL(clicked(bool)),this,SLOT(slot_btn_cancel()));
    QHBoxLayout* ly_H_btn = new QHBoxLayout();
    ly_H_btn->addStretch();
    ly_H_btn->addWidget(btn_save);
    ly_H_btn->addSpacing(10);
    ly_H_btn->addWidget(btn_cancel);
    ly_H_btn->addSpacing(10);

    QVBoxLayout* ly_V_all= new QVBoxLayout();
    ly_V_all->addWidget(lb_advice);
    ly_V_all->addWidget(textedit);
    ly_V_all->addWidget(lb_level);
    ly_V_all->addWidget(cmbox);
    ly_V_all->addLayout(ly_H_btn);
    this->setLayout(ly_V_all);

}

void ManageTipsInfo::slot_btn_cancel()
{
    close();
}

void ManageTipsInfo::slot_btn_save()
{
    if(textedit->toPlainText().isEmpty())
    {
       QMessageBox::warning(this,"警告：","建议内容不可以为空！",QMessageBox::Ok);
        return;
    }
    QString text=textedit->toPlainText();
    QString level = cmbox->currentText();
    QString Cr="admin";
    QString Ct=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    InfoDaoImp idi(this);
    idi.insertData_TipsInfo(text,level,Cr,Ct,"无","无");
    close();
}
