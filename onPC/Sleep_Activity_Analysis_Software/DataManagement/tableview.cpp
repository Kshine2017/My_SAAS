#include "tableview.h"

//#include "tablemodel.h"
#include "buttondelegate.h"
#include "DataBase/infodaoimp.h"
#include "exportlist.h"
TableView::TableView(QWidget *parent) :
    QTableView(parent)
{
    //iniData();
}

TableView::~TableView()
{
   // delete m_model;
}

void TableView::iniData()
{
//    m_model = new TableModel();
//    this->setModel(m_model);
//    QStringList headers;
//    headers << "ID号" << "姓名"<<"生日"<<"性别"<<"年龄"<<"监护人"<<"联系电话"<<"联系地址"<<"历史报告";
//    m_model->setHorizontalHeader(headers);

//    InfoDaoImp idi(this);
//    QMySQlQueryModel* model = new QMySQlQueryModel() ;
//    idi.selectData_PatientInfoManagement(ALL_Patient,"",model);
//    QVector<QStringList> data;
//    for(int i=0;i<model->rowCount();i++)
//    {
//       data.append(QStringList() << model->index(i,0).data().toString()
//                                 << model->index(i,1).data().toString()
//                                 << model->index(i,2).data().toString()
//                                 << model->index(i,3).data().toString()
//                                 << model->index(i,4).data().toString()
//                                 << model->index(i,5).data().toString()
//                                 << model->index(i,6).data().toString()
//                                 << model->index(i,7).data().toString());
//    }

//    m_model->setData(data);

//    m_buttonDelegate = new ButtonDelegate(this);
//    this->setItemDelegateForColumn(8, m_buttonDelegate);
//    emit m_model->layoutChanged();
}
