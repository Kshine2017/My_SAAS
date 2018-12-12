#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QTimer>
#include <QPushButton>
class Homepage : public QWidget
{
    Q_OBJECT
public:
    explicit Homepage(QWidget *parent = 0, int WWW=0, int HHH=0);
    int thisWidth;              //屏宽
    int thisHeight;             //屏高

signals:

public slots:

};

#endif // HOMEPAGE_H
