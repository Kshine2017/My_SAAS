#ifndef MYPRINTPREVIEWDIALOG_H
#define MYPRINTPREVIEWDIALOG_H

#include <QPrinter>
#include <QDialog>

#include <QScrollArea>

/*
类描述：功能类，打印
*/
class MyPrintPreViewDialog:public QDialog
{
    Q_OBJECT
public:
    MyPrintPreViewDialog(QWidget* parent=0);
    ~MyPrintPreViewDialog();
     int choosenum;
     //对象信息--------------------------------------
     QString ID;
     QString Name;
     QString Gander;
     QString Brithday;
     QString Age;
     QString Date;
    //--------------------------------------------
     QStringList strlist;       //数据
     //----------------------------------
     QStringList strlist_advice;
     bool print();                           //打印
     bool printpreview();
     void exportPDF();
     bool printPDF(QString fileName);
private slots:
    void printPreviewSlot(QPrinter *printer);//打印预览

};

#endif // MYPRINTPREVIEWDIALOG_H
