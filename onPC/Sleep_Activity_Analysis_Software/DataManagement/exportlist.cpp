#include "exportlist.h"
#include "DataBase/infodaoimp.h"
#include <QHeaderView>
#include <QDebug>
#include "openpdf.h"

ExportList::ExportList(QString ID,QString Name, QWidget *parent) : QDialog(parent)
{
    setFixedSize(600,400);
    setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("历史报告");
//    lb_test = new QLabel(this);
//    lb_test->setText(ID);

    init(); //初始化

    initPaintInfo(ID,Name);
    InfoDaoImp idi(this);
    QMySQlQueryModel* model =new QMySQlQueryModel();
    idi.selectData_ReportInfo_by_ID(ID,model);
    tableView->setModel(model);
    tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    IIIID=ID;
    show();
}
/**
 * @brief init  初始化布局
**/
void ExportList::init()
{
    mainLayout = new QVBoxLayout(this);
    labelLayout = new QHBoxLayout();
    btnLayout = new QHBoxLayout();
    tableView = new QTableView();
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); //使其不可编辑
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode ( QAbstractItemView::SingleSelection);
    connect(tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slot_openPDF(QModelIndex)));
    labelID = new QLabel();            //观察对象ID
    labelName = new QLabel();          //观察对象姓名

    btnSure = new QPushButton("确认打开");       //确认打开按钮
    btnCancel = new QPushButton("取消");     //取消按钮

    labelLayout->addStretch(1);
    labelLayout->addWidget(labelID);
    labelLayout->addStretch(1);
    labelLayout->addWidget(labelName);
    labelLayout->addStretch(6);


    btnLayout->addStretch(6);
    btnLayout->addWidget(btnSure);
    btnLayout->addStretch(1);
    btnLayout->addWidget(btnCancel);
    btnLayout->addStretch(1);

    mainLayout->addLayout(labelLayout);
    mainLayout->addWidget(tableView);
    mainLayout->addLayout(btnLayout);

}
/**
 * @brief initPaintInfo     初始化观察对象信息
 * @param ID                ID号
 * @param Name              姓名
 */

void ExportList::initPaintInfo(QString ID, QString Name)
{
    labelID->setText(" ID：" + ID);
    labelName->setText("姓名：" + Name);
}

void ExportList::slot_openPDF(QModelIndex index)
{
    QString pdfname = tableView->model()->index(index.row(),1).data().toString();
    if(pdfname.isEmpty())return;
    QString filepath="./Active_Report/"+IIIID+"/"+pdfname;
    new OpenPDF(this,filepath);
}

