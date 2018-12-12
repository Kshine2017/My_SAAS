#ifndef QMYSQLQUERYMODEL_H
#define QMYSQLQUERYMODEL_H
#include<QSqlQueryModel>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
/*
类描述：model格式类，用于居中对齐
关键接口：
    略
*/
class QMySQlQueryModel : public QSqlQueryModel
{
public:
    explicit  QMySQlQueryModel();
public:
    QVariant data(const QModelIndex &index, int role) const;

};

#endif // QMYSQLQUERYMODEL_H
