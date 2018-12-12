#include "datamanagement.h"
#include <QDebug>
#include "DataContainer/datacontainer.h"
#include <QHeaderView>
#include "exportlist.h"
#include <QMessageBox>
#include "DataBase/infodaoimp.h"
DataManagement::DataManagement(QWidget *parent, int WWW, int HHH) : QWidget(parent)
{
    if(WWW&&HHH)
    {
        //创建时指定大小
        thisWidth=WWW;
        thisHeight=HHH;

    }
    else
    {
        //默认子页面大小
        thisWidth=1920;
        thisHeight=1080;
        this->setStyleSheet("background-color:yellow;");
    }
    this->setFixedSize(thisWidth,thisHeight);
    this->setAutoFillBackground(true);
    qDebug()<<"页面：数据管理  尺寸："<<width()<<height();
    init_PageTopInfo();
    init_PageSecInfo();
//    init_PageTableInfo();
//    testTableData();

    init_PageTableInfo_new();
    init_PageAllLayout();
    qDebug()<<"数据管理页面：测试连接到数据容器"<<DataContainer::getInstance();
    qDebug()<<DataContainer::getInstance()->DevicePath;
    init_TableViewData();
}

DataManagement::~DataManagement()
{
    delete m_model;
    delete ly_H_Top;
    delete ly_H_Sec;
    delete ly_H_Table;
    delete ly_V_all;
}

void DataManagement::slot_tableclick(QModelIndex index)
{
    if(index.column()==8)//按钮
    {
        int row=index.row();
        new ExportList(tableView->model()->index(row,0).data().toString(),tableView->model()->index(row,1).data().toString(),this);
    }

}

void DataManagement::slot_btn_Serach()
{
    if(cmbx_search_criteria->currentIndex()!=0&&le_search_insert->text().isEmpty())
    {
        QMessageBox::information(this,"信息","请输入"+cmbx_search_criteria->currentText()+"内容!",QMessageBox::Yes);
        return;
    }
    int cmbxindex=cmbx_Default_sort->currentIndex();
    InfoDaoImp idi(this);
    QMySQlQueryModel* model = new QMySQlQueryModel();
    if(cmbx_search_criteria->currentIndex()==0)
        idi.selectData_PatientInfoManagement(ALL_Patient,"全部",model,cmbxindex);
    else
        idi.selectData_PatientInfoManagement(cmbx_search_criteria->currentIndex(),le_search_insert->text(),model,cmbxindex);
    QVector<QStringList> data;
    for(int i=0;i<model->rowCount();i++)
    {
       data.append(QStringList() << model->index(i,0).data().toString()
                                 << model->index(i,1).data().toString()
                                 << model->index(i,2).data().toString()
                                 << model->index(i,3).data().toString()
                                 << model->index(i,4).data().toString()
                                 << model->index(i,5).data().toString()
                                 << model->index(i,6).data().toString()
                                 << model->index(i,7).data().toString());
    }
    delete model;
    m_model->setData(data);
    m_buttonDelegate = new ButtonDelegate(this);
    tableView->setItemDelegateForColumn(8, m_buttonDelegate);
    emit m_model->layoutChanged();


}

void DataManagement::slot_auto_sort(int cmbxindex)
{
    InfoDaoImp idi(this);
    QMySQlQueryModel* model = new QMySQlQueryModel();
    if(cmbx_search_criteria->currentIndex()==0)
        idi.selectData_PatientInfoManagement(ALL_Patient,"全部",model,cmbxindex);
    else
        idi.selectData_PatientInfoManagement(cmbx_search_criteria->currentIndex(),le_search_insert->text(),model,cmbxindex);
    QVector<QStringList> data;
    for(int i=0;i<model->rowCount();i++)
    {
       data.append(QStringList() << model->index(i,0).data().toString()
                                 << model->index(i,1).data().toString()
                                 << model->index(i,2).data().toString()
                                 << model->index(i,3).data().toString()
                                 << model->index(i,4).data().toString()
                                 << model->index(i,5).data().toString()
                                 << model->index(i,6).data().toString()
                                 << model->index(i,7).data().toString());
    }
    delete model;
    m_model->setData(data);
    m_buttonDelegate = new ButtonDelegate(this);
    tableView->setItemDelegateForColumn(8, m_buttonDelegate);
    emit m_model->layoutChanged();
}



void DataManagement::init_PageTopInfo()
{
    pageName =new  QLabel(this);
    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor(92,123,137));
    pageName->setPalette(pa);
    pageName->setFont(QFont("楷体",22,75));
    pageName->setText("数据管理");


    ly_H_Top = new QHBoxLayout();
    ly_H_Top->addSpacing(10);
    ly_H_Top->addWidget(pageName);
    ly_H_Top->addStretch();
    ly_H_Top->addSpacing(10);

}

void DataManagement::init_PageSecInfo()
{
    //cmbx_first              = new QComboBox(this);  cmbx_first->addItem("全部状态(12)");
    cmbx_search_criteria    = new QComboBox(this);
    cmbx_search_criteria->addItem("全部");
    cmbx_search_criteria->addItem("ID");
    cmbx_search_criteria->addItem("姓名");
    cmbx_search_criteria->addItem("性别");
    cmbx_search_criteria->addItem("监护人");

    le_search_insert      = new QLineEdit(this);  le_search_insert->setPlaceholderText("请输入条件内容!");
    btn_search              = new QPushButton(this);btn_search->setText("搜索");
    connect(btn_search,SIGNAL(clicked(bool)),this,SLOT(slot_btn_Serach()));

    cmbx_Default_sort       = new QComboBox(this);  cmbx_Default_sort->addItem("默认排序");
    connect(cmbx_Default_sort,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_auto_sort(int)));
    cmbx_Default_sort->addItem("年龄升序");
    cmbx_Default_sort->addItem("年龄降序");

//    cmbx_first->setFixedHeight(30);
//    cmbx_first->setFont(QFont("楷体",12,75));
//    cmbx_first->setStyleSheet(
//                "background-color:rgb(255,255,255);"
//                                       "color: black;   "
//                               "border-radius: 2px;  "
//                              "        border: 2px groove gray; "
//                              "  border-style: outset;"
//                );

    cmbx_search_criteria->setFixedHeight(30);
    cmbx_search_criteria->setFont(QFont("楷体",12,75));
    cmbx_search_criteria->setStyleSheet(
                "background-color:rgb(255,255,255);"
                                       "color: black;   "
                               "border-radius: 2px;  "
                              "        border: 2px groove gray; "
                              "  border-style: outset;"
                );

    le_search_insert->setFixedHeight(30);
    le_search_insert->setMaximumWidth(thisWidth/7);
    le_search_insert->setFont(QFont("楷体",12,75));
    le_search_insert->setStyleSheet(
                "background-color:rgb(255,255,255);"
                                       "color: black;   "
                               "border-radius: 10px;  "
                              "        border: 2px groove gray; "
                              "  border-style: outset;"
                );

    btn_search->setFont(QFont("楷体",12,75));
    btn_search->setFixedSize(100,30);
    btn_search->setStyleSheet(
                "QPushButton{background-color:rgb(255,255,255);"
                                       "color: black;   "
                               "border-radius: 10px;  "
                              "        border: 2px groove gray; "
                              "  border-style: outset;}"
                "QPushButton:hover{background-color:white; color: black;}"
                "QPushButton:pressed{background-color:rgb(85, 170, 255); "
                              "border-style: inset; }"

                );
    cmbx_Default_sort->setFixedWidth(80);
    cmbx_Default_sort->setStyleSheet(
                "background-color:rgb(255,255,255);"
                                       "color: black;   "
                               "border-radius: 2px;  "
                              "        border: 2px groove gray; "
                              "  border-style: outset;"
                );
    ly_H_Sec = new QHBoxLayout();
    ly_H_Sec->addSpacing(30);
//    ly_H_Sec->addWidget(cmbx_first);
//    ly_H_Sec->addSpacing(10);
    ly_H_Sec->addWidget(cmbx_search_criteria);
    ly_H_Sec->addSpacing(20);
    ly_H_Sec->addWidget(le_search_insert);
    ly_H_Sec->addSpacing(10);
    ly_H_Sec->addWidget(btn_search);

    ly_H_Sec->addStretch();

    ly_H_Sec->addWidget(cmbx_Default_sort);
    ly_H_Sec->addSpacing(30);

}

void DataManagement::init_PageTableInfo()
{
    tablewidget = new QTableWidget(this);
    tablewidget->setColumnCount(7);
    tablewidget->setFixedWidth(thisWidth);
    tablewidget->setMaximumHeight(6000);
    tablewidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tablewidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tablewidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tablewidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    tablewidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    tablewidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    //tablewidget->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);

    QStringList header;
    header<<"编号"<<"姓名"<<"年龄"<<"联系方式"<<"家庭地址"<<"监护人姓名"<<"操作";
    tablewidget->setHorizontalHeaderLabels(header);

    ly_H_Table = new QHBoxLayout();
    ly_H_Table->addWidget(tablewidget);
}

void DataManagement::init_PageTableInfo_new()
{
    tableView = new QTableView(this);
    connect(tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(slot_tableclick(QModelIndex)));
    tableView->setFixedWidth(thisWidth);
    tableView->setMaximumHeight(6000);

    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //使其不可编辑
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode ( QAbstractItemView::SingleSelection);
    ly_H_Table = new QHBoxLayout();
    ly_H_Table->addWidget(tableView);


}

void DataManagement::init_PageAllLayout()
{
    ly_V_all = new QVBoxLayout();
    ly_V_all->addSpacing(10);
    ly_V_all->addLayout(ly_H_Top);

    QLabel *m_label = new QLabel(this);
    m_label->setFrameShape(QFrame::HLine);
    m_label->setFrameShadow(QFrame::Raised);
    m_label->setMaximumSize(QSize(6000, 1));
    ly_V_all->addWidget(m_label);
    ly_V_all->addLayout(ly_H_Sec);

    QLabel *m_label2 = new QLabel(this);
    m_label2->setFrameShape(QFrame::HLine);
    m_label2->setFrameShadow(QFrame::Raised);
    m_label2->setMaximumSize(QSize(6000, 1));
    ly_V_all->addWidget(m_label2);
    ly_V_all->addLayout(ly_H_Table);

    // ly_V_all->addStretch();

    ly_V_all->setGeometry(QRect(0,0,thisWidth,thisHeight));

}

void DataManagement::testTableData()
{

    for(int i=0;i<40;i++)
    {
        QString numberID = "201807070"+QString::number((i+1)/10)+QString::number((i+1)%10);
        int row = tablewidget->rowCount();
        tablewidget->insertRow(row);
        QTableWidgetItem* item0 =new QTableWidgetItem(numberID);item0->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem* item1 =new QTableWidgetItem("黄志杰");item1->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem* item2 =new QTableWidgetItem("1岁6个月");item2->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem* item3 =new QTableWidgetItem("18862610351");item3->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem* item4 =new QTableWidgetItem("上海市长宁区协和路787号B北609室");item4->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem* item5 =new QTableWidgetItem("黄飞鸿");//item5->setTextAlignment(Qt::AlignCenter);
        //QTableWidgetItem* item6 =new QTableWidgetItem("查看");item0->setTextAlignment(Qt::AlignCenter);
        tablewidget->setItem(row,0,item0);
        tablewidget->setItem(row,1,item1);
        tablewidget->setItem(row,2,item2);
        tablewidget->setItem(row,3,item3);
        tablewidget->setItem(row,4,item4);
        tablewidget->setItem(row,5,item5);
        //tablewidget->setItem(row,6,item6);

#if 0
        QHBoxLayout *hLayout  = new QHBoxLayout();
        QWidget *widget = new QWidget(tablewidget);
        QComboBox* cmbx6 = new QComboBox;//(tablewidget);
        cmbx6->setFixedSize(80,20);
        cmbx6->insertItem(0,"查看");cmbx6->insertItem(1,"修改");cmbx6->insertItem(2,"删除");
        hLayout->addWidget(cmbx6,0,Qt::AlignCenter);
        widget->setLayout(hLayout);

        tablewidget->setCellWidget(row,6,widget);
#else
        QComboBox* cmbx6 = new QComboBox;//(tablewidget);
        cmbx6->insertItem(0,"查看");cmbx6->insertItem(1,"修改");cmbx6->insertItem(2,"删除");
        cmbx6->setStyleSheet(                "background-color:rgb(255,255,255);"
                                             "color: black;   "
                                     "border-radius: 2px;  "
                                    "        border: 2px groove gray; "
                                    "  border-style: outset;");
        tablewidget->setCellWidget(row,6,cmbx6);
#endif
    }

}

void DataManagement::init_TableViewData()
{

    m_model = new TableModel();
    tableView->setModel(m_model);
    QStringList headers;
    headers << "ID号" << "姓名"<<"生日"<<"性别"<<"年龄"<<"监护人"<<"联系电话"<<"联系地址"<<"历史报告";
    m_model->setHorizontalHeader(headers);

    InfoDaoImp idi(this);
    QMySQlQueryModel* model = new QMySQlQueryModel();
    idi.selectData_PatientInfoManagement(ALL_Patient,"",model);
    QVector<QStringList> data;
    for(int i=0;i<model->rowCount();i++)
    {
       data.append(QStringList() << model->index(i,0).data().toString()
                                 << model->index(i,1).data().toString()
                                 << model->index(i,2).data().toString()
                                 << model->index(i,3).data().toString()
                                 << model->index(i,4).data().toString()
                                 << model->index(i,5).data().toString()
                                 << model->index(i,6).data().toString()
                                 << model->index(i,7).data().toString());
    }
    delete model;
    m_model->setData(data);
    m_buttonDelegate = new ButtonDelegate(this);
    tableView->setItemDelegateForColumn(8, m_buttonDelegate);
    emit m_model->layoutChanged();
    tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
}
