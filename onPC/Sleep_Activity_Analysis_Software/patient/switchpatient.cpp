#include "switchpatient.h"
#include <QDebug>
#include <QMessageBox>
#include "DataContainer/datacontainer.h"
#include <QHeaderView>
#include "DataBase/infodaoimp.h"

#define  window_width 600
#define window_height 500
SwitchPatient::SwitchPatient(QWidget *parent) : QDialog(parent)
{
    setFixedSize(window_width,window_height);
    setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_DeleteOnClose);

    this->setWindowFlags(Qt::FramelessWindowHint |windowFlags()); //| Qt::Window );//
    //setWindowFlags(Qt::WindowStaysOnTopHint);
    TitleBar* pTitleBar = new TitleBar(this);
    pTitleBar->setFont(QFont("楷体",12,75));

    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);
    pTitleBar->setPalette(pa);

    installEventFilter(pTitleBar);
    setWindowTitle("切换对象");
    //setWindowIcon(QIcon(":/Images/logo"));
    setAutoFillBackground(true);

    //界面
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(117, 138, 151));
    setAutoFillBackground(true);
    setPalette(pal);

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addStretch();
       pLayout->setSpacing(0);
       pLayout->setContentsMargins(0, 0, 0, 0);
       setLayout(pLayout);
    lb_backcolor = new QLabel(this);
    lb_backcolor->setGeometry(0,pTitleBar->height(),10000,10000);
    lb_backcolor->setStyleSheet("background-color:white;");



    init_all();
    get_tabledata();
    rowFlag=-1;
    this->show();
}

void SwitchPatient::init_all()
{
    tableview = new QTableView(this);
    tableview->setFixedSize(window_width-20,window_height*4/5);

    btn_save = new QPushButton(this);btn_save->setText("确定");
    connect(btn_save,SIGNAL(clicked(bool)),this,SLOT(slot_btn_save()));
    btn_cancel = new QPushButton(this);btn_cancel->setText("取消");
    connect(btn_cancel,SIGNAL(clicked(bool)),this,SLOT(slot_btn_cancel()));

    btn_save->setStyleSheet("background-color:rgb(117, 138, 151);color: white;border-radius: 4px; border: 1px groove rgb(200,200,200); ");
    btn_cancel->setStyleSheet("background-color:rgb(255,255,255);color: black;border-radius: 4px; border: 1px groove rgb(200,200,200); ");
    btn_save    ->setFixedSize(100,30);
    btn_cancel->setFixedSize(100,30);


    ly_H_table = new QHBoxLayout();
    ly_H_table->addStretch();
    ly_H_table->addWidget(tableview);
    ly_H_table->addStretch();


    ly_H_Button = new QHBoxLayout();
    ly_H_Button->addStretch();
    ly_H_Button->addWidget(btn_save);
    ly_H_Button->addSpacing(10);
    ly_H_Button->addWidget(btn_cancel);
    ly_H_Button->addSpacing(65);

    ly_V_ALL = new QVBoxLayout();
    ly_V_ALL->addStretch();
    ly_V_ALL->addSpacing(30);
    ly_V_ALL->addLayout(ly_H_table);
    ly_V_ALL->addSpacing(10);
    ly_V_ALL->addLayout(ly_H_Button);
    ly_V_ALL->addStretch();
    //setLayout(ly_V_ALL);
    ly_V_ALL->setGeometry(QRect(0,0,window_width,window_height));

}

void SwitchPatient::get_tabledata()
{
    InfoDaoImp idi(this);
    model = new QMySQlQueryModel() ;
    idi.selectData_PatientInfo(ALL_Patient,"",model);
    tableview->setModel(model);

    tableview->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableview->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tableview->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tableview->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    tableview->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    tableview->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);

    tableview->setEditTriggers(QAbstractItemView::NoEditTriggers); //使其不可编辑
    tableview->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableview->setSelectionMode ( QAbstractItemView::SingleSelection);

            connect(tableview,SIGNAL(clicked(QModelIndex)),this,SLOT(slot_click(QModelIndex)));
            connect(tableview,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slot_doubleclick(QModelIndex)));

}

void SwitchPatient::slot_btn_save()
{
    if(rowFlag==-1)
    {
        this->close();
        return;
    }
    //---------------------------
    DataContainer* dc =DataContainer::getInstance();


    qDebug()<<"选中第"<<rowFlag+1<<"行";
    QString ID = tableview->model()->index(rowFlag,0).data().toString();
    //------------------------------------------------------------------
    QString Name;
    QString Gender;
    QString BrithDay;
    QString Age;

    QString WatcherName;
    QString WatcherPhone;
    QString WatcherAdress;
    InfoDaoImp idi(this);
    idi.selectData_PatientInfo_byID(ID,Name,Gender,BrithDay,Age,WatcherName,WatcherPhone,WatcherAdress);
    dc->ID=ID;
    dc->PatientName=Name;
    dc->Gander=Gender;
    dc->BrithDay=BrithDay;
    dc->Age=Age;
    dc->WatcherName=WatcherName;
    dc->WatcherPhone=WatcherPhone;
    dc->WatcherAdress=WatcherAdress;
    emit signal_Sure();
    //---------------------------
    this->close();
}

void SwitchPatient::slot_btn_cancel()
{
    this->close();
}

void SwitchPatient::slot_doubleclick(QModelIndex index)
{
    rowFlag=index.row();
    qDebug()<<"双击选中第"<<rowFlag+1<<"行";
    //---------------------------
    DataContainer* dc =DataContainer::getInstance();

    QString ID = tableview->model()->index(rowFlag,0).data().toString();
    //------------------------------------------------------------------
    QString Name;
    QString Gender;
    QString BrithDay;
    QString Age;

    QString WatcherName;
    QString WatcherPhone;
    QString WatcherAdress;
    InfoDaoImp idi(this);
    idi.selectData_PatientInfo_byID(ID,Name,Gender,BrithDay,Age,WatcherName,WatcherPhone,WatcherAdress);
    dc->ID=ID;
    dc->PatientName=Name;
    dc->Gander=Gender;
    dc->BrithDay=BrithDay;
    dc->Age=Age;
    dc->WatcherName=WatcherName;
    dc->WatcherPhone=WatcherPhone;
    dc->WatcherAdress=WatcherAdress;
    emit signal_Sure();
    this->close();
}

void SwitchPatient::slot_click(QModelIndex index)
{
    rowFlag=index.row();
}
