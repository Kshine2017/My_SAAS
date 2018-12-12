#ifndef INFODAOIMP_H
#define INFODAOIMP_H

#include <QWidget>
#include "qmysqlquerymodel.h"
enum enum_patient
{
    ALL_Patient=0,
    By_Name
};

/*
类描述：数据库操作类，包括所有表的增删改查
关键接口：
    略
*/
class InfoDaoImp : public QWidget
{
    Q_OBJECT
public:
    explicit InfoDaoImp(QWidget *parent = 0);

    //儿童或者观测对象---------------------------------------
    //增
    bool insertData_PatientInfo(
            QString ID,QString Name,QString Gander,QString BrithDay,QString Age,
            QString WatcherName,QString WatcherPhone,QString WatcherAdress,
            QString CreationDate,QString Creator,QString ModificationDate,QString Modifier
            );
    //查全部
    bool selectData_PatientInfo(enum_patient ep, QString Name, QMySQlQueryModel *model);
    bool selectData_PatientInfoManagement(enum_patient ep, QString Name, QMySQlQueryModel *model,int sortIndex=0);
    bool selectData_PatientInfoManagement(int cmbxIndex, QString str, QMySQlQueryModel *model,int sortIndex=0);

    //查，根据ID号
    bool selectData_PatientInfo_byID(const QString &ID, QString &Name,
                                     QString &Gender,
                                     QString &BrithDay,
                                     QString &Age,
                                     QString &WatcherName,
                                     QString &WatcherPhone,
                                     QString &WatcherAdress);
    //修改，根据ID号
    bool updateData_PatientInfo(
            QString ID,QString Name,QString Gander,QString BrithDay,QString Age,
            QString WatcherName,QString WatcherPhone,QString WatcherAdress,
            QString ModificationDate,QString Modifier
            );
    //软件用户（诊断医师）---------------------------------------



    //个人报告记录---------------------------------------
    bool insertData_ReportInfo(QString FileName, QString ID, QString Name,
                               QString Date, QString Number, QString SleepInfo,
                               QString CreationDate, QString Creator, QString ModificationDate, QString Modifier, QString DeleteFlag="0");
    bool selectData_ReportInfo_by_ID(QString ID,QMySQlQueryModel* model);

    //建议性文字---------------------------------------
    //增
    bool insertData_TipsInfo(QString text, QString level,
            QString CreationDate, QString Creator, QString ModificationDate, QString Modifier
            );
    //查，所有
    bool selectData_AllTipsInfo(QMySQlQueryModel* model);

signals:

public slots:
};

#endif // INFODAOIMP_H
