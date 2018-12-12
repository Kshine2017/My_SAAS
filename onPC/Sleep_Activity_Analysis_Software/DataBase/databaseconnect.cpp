#include "databaseconnect.h"
#include <QSqlQuery>

DataBaseConnect *DataBaseConnect::instance=0;


DataBaseConnect::DataBaseConnect()
{
    db = QSqlDatabase::addDatabase("QSQLITE");  //确定数据库的类型sqlite
}


DataBaseConnect* DataBaseConnect::getInstance()             //对外接口
{
    if(instance==0)
        instance=new DataBaseConnect();
    return instance;
}

void DataBaseConnect::createconnect()                 //创建数据路的连接
{
    db.setDatabaseName("Believe.db");              //打开数据库，如果没有，则创建一个
    db.open();

    QSqlQuery query;                            //如果没有表，则创建一个用户表

    //表1    观察对象信息
    query.exec("create table if not exists tb_PatientInfo("
               "ID              text primary key not null,"
               "Name            text not null,"
               "Gander          text,"
               "BrithDay        text,"
               "Age             text,"
               "WatcherName     text,"
               "WatcherPhone    text,"
               "WatcherAdress   text,"
               "CreationDate    text,"
               "Creator         text,"
               "ModificationDate text,"
               "Modifier        text"
               ");");

    //表2    软件用户
    query.exec("create table if not exists tb_SoftwareUser("
               "Account         text primary key not null,"
               "Password        text not null, "
               "Name            text not null,"
               "CreationDate    text,"
               "Creator         text,"
               "ModificationDate text,"
               "Modifier        text"
               " );");
    //表3    分析报告
    query.exec("create table if not exists tb_AnalysisRecord("
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
               ");");
    //表4    医生建议
    query.exec("create table if not exists tb_Tips("
               "text            text primary key not null, "
               "level           text,"
               "CreationDate    text,"
               "Creator         text,"
               "ModificationDate text,"
               "Modifier        text"
               " );");

}

void DataBaseConnect::destoryconnect()                //断开数据库的连接
{
    db.close();
    db.removeDatabase("Believe.db");
}

