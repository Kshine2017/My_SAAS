#ifndef MOVEFILE_H
#define MOVEFILE_H

#include <QObject>

class MoveFile : public QObject
{
    Q_OBJECT
public:
    explicit MoveFile(QObject *parent = 0);
    void movePDFtoDir();
    bool PDFisExist();
    bool PDFisExist_X();
    bool deletePDF();
    bool deletePDF_X();
    void outputPDF();



    //----------------------
    QString filename;
    QString toWhere;
    QString name;
    QString targetFilename;
private:

signals:
    void work_over();                       //工作结束，通知主线程关闭次线程和自己
    void work_message(QString text);        //发送消息给主线程
    void work_Progress(qint64 progress);    //发送进度数值
    void work_TotalSize(qint64);

public slots:
};

#endif // MOVEFILE_H
