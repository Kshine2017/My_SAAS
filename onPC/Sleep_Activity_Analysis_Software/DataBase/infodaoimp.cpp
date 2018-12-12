#include "infodaoimp.h"
#include "databaseconnect.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>

InfoDaoImp::InfoDaoImp(QWidget *parent) : QWidget(parent)
{

}

/*
               "ID              text primary key autoincrement,"
               "Name            text not null,"
               "Gander          text,"
               "BrithDay        text,"
               "Age             integer,"
               "WatcherName     text,"
               "WatcherPhone    text,"
               "WatcherAdress   text,"
               "CreationDate    text,"
               "Creator         text,"
               "ModificationDate text,"
               "Modifier        text"
*/
bool InfoDaoImp::insertData_PatientInfo(
        QString ID,QString Name,QString Gander,QString BrithDay,QString Age,
        QString WatcherName,QString WatcherPhone,QString WatcherAdress,
        QString CreationDate,QString Creator,QString ModificationDate,QString Modifier
        )
{
    DataBaseConnect::getInstance()->createconnect();
    QSqlQuery query;
    query.prepare("insert into tb_PatientInfo values("
                  ":ID,"
                  ":Name,"
                  ":Gander,"
                  ":BrithDay,"
                  ":Age,"
                  ":WatcherName,"
                  ":WatcherPhone,"
                  ":WatcherAdress,"
                  ":CreationDate,"
                  ":Creator,"
                  ":ModificationDate,"
                  ":Modifier);");
    query.bindValue(":ID",(const QVariant)ID);
    query.bindValue(":Name",(const QVariant)Name);
    query.bindValue(":Gander",(const QVariant)Gander);
    query.bindValue(":BrithDay",(const QVariant)BrithDay);
    query.bindValue(":Age",(const QVariant)Age);
    query.bindValue(":WatcherName",(const QVariant)WatcherName);
    query.bindValue(":WatcherPhone",(const QVariant)WatcherPhone);
    query.bindValue(":WatcherAdress",(const QVariant)WatcherAdress);
    query.bindValue(":CreationDate",(const QVariant)CreationDate);
    query.bindValue(":Creator",(const QVariant)Creator);
    query.bindValue(":ModificationDate",(const QVariant)ModificationDate);
    query.bindValue(":Modifier",(const QVariant)Modifier);
    bool ret = query.exec();
   //------------------------------------------------------------------
    if(!ret)qDebug()<<"insert tb_PatientInfo sql语句失败！";
    DataBaseConnect::getInstance()->destoryconnect();
    return ret;
}

bool InfoDaoImp::selectData_PatientInfo(enum_patient ep, QString Name,QMySQlQueryModel* model)
{
    Q_UNUSED(ep)
    Q_UNUSED(Name)

    DataBaseConnect::getInstance()->createconnect();
    QString sss="select "
                "ID,"
                "Name,"
                "Gander,"
                "Age,"
                "WatcherName,"
                "WatcherPhone"
                " from tb_PatientInfo ;";

    model->setQuery(sss);
    model->setHeaderData(0,Qt::Horizontal,QVariant("ID"));
    model->setHeaderData(1,Qt::Horizontal,QVariant("姓名"));
    model->setHeaderData(2,Qt::Horizontal,QVariant("性别"));
    model->setHeaderData(3,Qt::Horizontal,QVariant("年龄"));
    model->setHeaderData(4,Qt::Horizontal,QVariant("监护人"));
    model->setHeaderData(5,Qt::Horizontal,QVariant("电话"));

    DataBaseConnect::getInstance()->destoryconnect();
    return true;
}

bool InfoDaoImp::selectData_PatientInfoManagement(enum_patient ep, QString Name, QMySQlQueryModel *model, int sortIndex)
{
    Q_UNUSED(ep)
    Q_UNUSED(Name)
    QString sort="";
    if(sortIndex==1)sort=" order by BrithDay desc ";//年龄升序，按生日降序
    else if(sortIndex==2)sort=" order by BrithDay asc ";//年龄降序
    DataBaseConnect::getInstance()->createconnect();
    QString sss="select "
                "ID,"
                "Name,"
                "BrithDay,"
                "Gander,"
                "Age,"
                "WatcherName,"
                "WatcherPhone,"
                "WatcherAdress"
                " from tb_PatientInfo "+sort+";";

    model->setQuery(sss);
    DataBaseConnect::getInstance()->destoryconnect();
    return true;
}

bool InfoDaoImp::selectData_PatientInfoManagement(int cmbxIndex, QString str, QMySQlQueryModel *model, int sortIndex)
{
    QString AAA ="",BBB="";

         if(cmbxIndex == 1){AAA="ID = '";           BBB="'";}
    else if(cmbxIndex == 2){AAA="Name like '%";     BBB="%'";}
    else if(cmbxIndex == 3){AAA="Gander = '";       BBB="'";}
    else if(cmbxIndex == 4){AAA="WatcherName like '%"; BBB="%'";}
    QString sort="";
    if(sortIndex==1)sort=" order by BrithDay desc ";//年龄升序，按生日降序
    else if(sortIndex==2)sort=" order by BrithDay asc ";//年龄降序
    DataBaseConnect::getInstance()->createconnect();
    QString sss="select "
                "ID,"
                "Name,"
                "BrithDay,"
                "Gander,"
                "Age,"
                "WatcherName,"
                "WatcherPhone,"
                "WatcherAdress"
                " from tb_PatientInfo where "+AAA+str+BBB+sort+";";
    qDebug()<<"数据管理："<<sss;
    model->setQuery(sss);
    DataBaseConnect::getInstance()->destoryconnect();
    return true;
}

bool InfoDaoImp::selectData_PatientInfo_byID(const QString& ID,QString& Name,
QString& Gender,
 QString& BrithDay,
QString& Age,
QString& WatcherName,
QString& WatcherPhone,
QString& WatcherAdress)
{

    DataBaseConnect::getInstance()->createconnect();
    QSqlQuery query;
    query.prepare("select  "
                  "Name, "
                  "Gander,"
                  "BrithDay,"
                  "Age,"
                  "WatcherName,"
                  "WatcherPhone, "
                  "WatcherAdress "
                  " from tb_PatientInfo  where ID = :ID;"
);
    query.bindValue(":ID",(const QVariant)ID);
    bool ret = query.exec();
   //------------------------------------------------------------------
    if(!ret)qDebug()<<"update tb_PatientInfo sql语句失败！";
    if(!query.next())
    {
        qDebug()<<"未查询到ID："<<ID <<"的用户";
    }
    else
    {
        Name=query.value(0).toString();
        Gender=query.value(1).toString();
        BrithDay=query.value(2).toString();
        Age=query.value(3).toString();
        WatcherName=query.value(4).toString();
        WatcherPhone=query.value(5).toString();
        WatcherAdress=query.value(6).toString();

    }
    DataBaseConnect::getInstance()->destoryconnect();
    return ret;
}

bool InfoDaoImp::updateData_PatientInfo(QString ID, QString Name, QString Gander, QString BrithDay, QString Age, QString WatcherName, QString WatcherPhone, QString WatcherAdress, QString ModificationDate, QString Modifier)
{
    DataBaseConnect::getInstance()->createconnect();
    QSqlQuery query;

    query.prepare("update tb_PatientInfo set "
                  "Name = :Name, "
                  "Gander = :Gander ,"
                  "BrithDay = :BrithDay,"
                  "Age=:Age,"
                  "WatcherName=:WatcherName,"
                  "WatcherPhone=:WatcherPhone, "
                  "WatcherAdress=:WatcherAdress,"
                  "ModificationDate=:ModificationDate,"
                  "Modifier=:Modifier "
                  "where ID = :ID;"
);
    query.bindValue(":ID",(const QVariant)ID);
    query.bindValue(":Name",(const QVariant)Name);
    query.bindValue(":Gander",(const QVariant)Gander);
    query.bindValue(":BrithDay",(const QVariant)BrithDay);
    query.bindValue(":Age",(const QVariant)Age);
    query.bindValue(":WatcherName",(const QVariant)WatcherName);
    query.bindValue(":WatcherPhone",(const QVariant)WatcherPhone);
    query.bindValue(":WatcherAdress",(const QVariant)WatcherAdress);
    query.bindValue(":ModificationDate",(const QVariant)ModificationDate);
    query.bindValue(":Modifier",(const QVariant)Modifier);
    bool ret = query.exec();
   //------------------------------------------------------------------
    if(!ret)qDebug()<<"update tb_PatientInfo sql语句失败！";
    DataBaseConnect::getInstance()->destoryconnect();
    return ret;

}
/*
               "FileName                        text primary key not null, "
               "ID                              text,  "
               "Name                            text, "
               "Date                            text, "
               "Number                          text, "
               "SleepInfo                       text, "
               "CreationDate                    text, "
               "Creator                         text, "
               "ModificationDate                text, "
               "Modifier                        text, "
               "DeleteFlag                      text "
*/

bool InfoDaoImp::insertData_ReportInfo(QString FileName,QString ID,QString Name,
                                       QString Date,QString Number,QString SleepInfo,
                                       QString CreationDate,QString Creator,QString ModificationDate,QString Modifier,QString DeleteFlag)
{
    DataBaseConnect::getInstance()->createconnect();
    QSqlQuery query;
    query.prepare("insert into tb_AnalysisRecord values("
                  ":FileName,"
                  ":ID,"
                  ":Name,"
                  ":Date,"
                  ":Number,"
                  ":SleepInfo,"
                  ":CreationDate,"
                  ":Creator,"
                  ":ModificationDate,"
                  ":Modifier,"
                  ":DeleteFlag);"
                  );
    query.bindValue(":FileName" ,(const QVariant)FileName);
    query.bindValue(":ID"       ,(const QVariant)ID);
    query.bindValue(":Name"     ,(const QVariant)Name);
    query.bindValue(":Date"     ,(const QVariant)Date);
    query.bindValue(":Number"   ,(const QVariant)Number);
    query.bindValue(":SleepInfo",(const QVariant)SleepInfo);
    query.bindValue(":CreationDate"     ,(const QVariant)CreationDate);
    query.bindValue(":Creator"          ,(const QVariant)Creator);
    query.bindValue(":ModificationDate" ,(const QVariant)ModificationDate);
    query.bindValue(":Modifier"         ,(const QVariant)Modifier);
    query.bindValue(":DeleteFlag"         ,(const QVariant)DeleteFlag);
    bool ret = query.exec();
   //------------------------------------------------------------------
    if(!ret)qDebug()<<"insert tb_AnalysisRecord sql语句失败！"<<query.lastError();
    DataBaseConnect::getInstance()->destoryconnect();
    return ret;
}

bool InfoDaoImp::selectData_ReportInfo_by_ID(QString ID, QMySQlQueryModel *model)
{
    DataBaseConnect::getInstance()->createconnect();
    QString sss="select "
                "Date,"
                "FileName "
                " from tb_AnalysisRecord where ID = '";
    sss+=ID+"';";

    model->setQuery(sss);
    model->setHeaderData(0,Qt::Horizontal,QVariant("生成日期"));
    model->setHeaderData(1,Qt::Horizontal,QVariant("PDF文件名称"));
    DataBaseConnect::getInstance()->destoryconnect();
    return true;
}


bool InfoDaoImp::insertData_TipsInfo( QString text, QString level, QString CreationDate, QString Creator, QString ModificationDate, QString Modifier)
{
    DataBaseConnect::getInstance()->createconnect();
    QSqlQuery query;
    query.prepare("insert into tb_Tips values("
                  ":text,"
                  ":level,"
                  ":CreationDate,"
                  ":Creator,"
                  ":ModificationDate,"
                  ":Modifier);");
    query.bindValue(":text",(const QVariant)text);
    query.bindValue(":level",(const QVariant)level);
    query.bindValue(":CreationDate",(const QVariant)CreationDate);
    query.bindValue(":Creator",(const QVariant)Creator);
    query.bindValue(":ModificationDate",(const QVariant)ModificationDate);
    query.bindValue(":Modifier",(const QVariant)Modifier);
    bool ret = query.exec();
   //------------------------------------------------------------------
    if(!ret)qDebug()<<"insert tb_Tips sql语句失败！"<<query.lastError();
    DataBaseConnect::getInstance()->destoryconnect();
    return ret;
}

bool InfoDaoImp::selectData_AllTipsInfo(QMySQlQueryModel* model)
{
    DataBaseConnect::getInstance()->createconnect();
    QString sss="select "
                "text,"
                "level "
                " from tb_Tips ;";
    model->setQuery(sss);
    model->setHeaderData(0,Qt::Horizontal,QVariant("文字内容"));
    model->setHeaderData(1,Qt::Horizontal,QVariant("等级"));
    DataBaseConnect::getInstance()->destoryconnect();
    return true;
}
