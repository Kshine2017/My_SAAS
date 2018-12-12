#include "doctoradvice.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include "DataContainer/datacontainer.h"
#include "DataBase/infodaoimp.h"
#include <QHeaderView>
DoctorAdvice::DoctorAdvice(QWidget *parent, int flag_Day) : QDialog(parent)
{
    this->setWindowModality(Qt::WindowModal);
    this->setAttribute(Qt::WA_DeleteOnClose);
    Flag_Day =flag_Day;
    setFixedSize(600,400);
    lb_tips = new QLabel(this);lb_tips->setText("建议：");
    textedit = new QTextEdit(this);
    DataContainer* dc =DataContainer::getInstance();
    QString str;
    if(Flag_Day==1)str=dc->DoctorTips1;
    else if(Flag_Day==2)str=dc->DoctorTips2;
        else if(Flag_Day==3)str=dc->DoctorTips3;
            else if(Flag_Day==4)str=dc->DoctorTips4;
                else if(Flag_Day==5)str=dc->DoctorTips5;
                    else if(Flag_Day==6)str=dc->DoctorTips6;

    textedit->setPlainText(str);
    lb_moTips=new QLabel(this);lb_moTips->setText("模板建议：");
    tableview=new QTableView(this);
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
    ly_V_all->addWidget(lb_tips);
    ly_V_all->addWidget(textedit);
    ly_V_all->addWidget(lb_moTips);
    ly_V_all->addWidget(tableview);
    ly_V_all->addLayout(ly_H_btn);
    this->setLayout(ly_V_all);

    InfoDaoImp idi(this);
    QMySQlQueryModel* model = new QMySQlQueryModel() ;
    idi.selectData_AllTipsInfo(model);
    tableview->setModel(model);


    tableview->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableview->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
//  tableview->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);          //使其不可编辑
    tableview->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableview->setSelectionMode ( QAbstractItemView::SingleSelection);

    connect(tableview,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slot_doubleclick(QModelIndex)));


    this->show();



}

void DoctorAdvice::slot_btn_save()
{
    qDebug()<<"保存";
    DataContainer* dc =DataContainer::getInstance();
    QString str=textedit->toPlainText();
     if(Flag_Day==1)dc->DoctorTips1=str;
    else if(Flag_Day==2)dc->DoctorTips2=str;
        else if(Flag_Day==3)dc->DoctorTips3=str;
            else if(Flag_Day==4)dc->DoctorTips4=str;
                else if(Flag_Day==5)dc->DoctorTips5=str;
                    else if(Flag_Day==6)dc->DoctorTips6=str;
    if(str=="")emit signal_tips_ok(false);
    else emit signal_tips_ok(true);
     close();
}

void DoctorAdvice::slot_btn_cancel()
{
    qDebug()<<"取消";
    close();
}

void DoctorAdvice::slot_doubleclick(QModelIndex index)
{

    int rowFlag=index.row();
    //---------------------------
    QString str = tableview->model()->index(rowFlag,0).data().toString();
    //------------------------------------------------------------------
   textedit->append(str);
}
