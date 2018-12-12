#ifndef DATABASECONNECT_H
#define DATABASECONNECT_H

#include <QSqlDatabase>

/*
类描述：数据库连接类，包括数据库的创建，表的创建
关键接口：
    void createconnect();
    void destoryconnect();
*/
class DataBaseConnect
{
public:
    static DataBaseConnect* getInstance();  //公开接口，用于得到对象
    void createconnect();                   //创建数据库的连接
    void destoryconnect();                  //断开数据库的连接
private:
    static DataBaseConnect* instance;
    QSqlDatabase db;
    DataBaseConnect();
};
#endif // DATABASECONNECT_H
