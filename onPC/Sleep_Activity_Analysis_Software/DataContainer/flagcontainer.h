#ifndef FLAGCONTAINER_H
#define FLAGCONTAINER_H

#include <QObject>
#include <QString>
#include <QVector>
#define KERRO_Flag  -1
struct FSleep{
    int start;  //开始
    int end;    //结束
    int year;   //开始的年
    int month;  //开始的月
    int day;    //开始的天
    int row;    //开始的0~3599
};

struct FPower{
    int start;  //开始
    int end;    //结束
    int year;   //开始的年
    int month;  //开始的月
    int day;    //开始的天
    int row;    //开始的0~3599
};
class FlagContainer : public QObject
{
    Q_OBJECT
public:
    static FlagContainer* getInstance();
    ~FlagContainer();
    //标记数据成员
    QVector<FPower>     VPower;         //成对出现的开关机
    QVector<FSleep>     VSleep;         //成对出现的睡眠标志

signals:

public slots:

private:
    static FlagContainer* instance;
    explicit FlagContainer(QObject *parent = 0);
};

#endif // FLAGCONTAINER_H
