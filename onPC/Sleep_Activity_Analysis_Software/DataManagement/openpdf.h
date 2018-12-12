#ifndef OPENPDF_H
#define OPENPDF_H

#include <QDialog>
#include <QScrollArea>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
class OpenPDF : public QDialog
{
    Q_OBJECT
public:
    explicit OpenPDF(QWidget *parent = 0, QString filepath="");
    ~OpenPDF();
    int thisWidth;
    int thisHeight;
    QString fileName;
    QScrollArea* scrollArea;
    QPushButton *btn_print;
    QPushButton* btn_outputPDF;
    QHBoxLayout* ly_H_Sec;
    QVBoxLayout* ly_V_all;
signals:

public slots:
    void slot_print();
    void slot_outputPDF();
};

#endif // OPENPDF_H
