#include "onetable.h"
#include <QStringList>
#include <QHeaderView>
#include "DataContainer/datacontainer.h"
OneTable::OneTable(int outWWW, int outHHH, QWidget *parent,int Flag_day,int Flag_DUAN) : QWidget(parent)
{
    Q_UNUSED(outWWW)
    Q_UNUSED(outHHH)
    this->setFixedSize(TABLEWWW,TABLEHHH);
    tablewidget = new QTableWidget(this);
    tablewidget->setFixedSize(TABLEWWW,TABLEHHH);
    tablewidget->setColumnCount(4);
    tablewidget->setRowCount(1);
    QStringList header;
    header <<"睡眠开始"<<"睡眠结束"<<"浅睡时长"<<"深睡时长";
    tablewidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tablewidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tablewidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tablewidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    tablewidget->setHorizontalHeaderLabels(header);

    //填入数据
    getDataFromDC(Flag_day,Flag_DUAN);
    QTableWidgetItem* item0 =new QTableWidgetItem(str0);item0->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem* item1 =new QTableWidgetItem(str1);item1->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem* item2 =new QTableWidgetItem(str2+"小时");item2->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem* item3 =new QTableWidgetItem(str3+"小时");item3->setTextAlignment(Qt::AlignCenter);
    tablewidget->setItem(0,0,item0);tablewidget->setItem(0,1,item1);tablewidget->setItem(0,2,item2);tablewidget->setItem(0,3,item3);
}

void OneTable::getDataFromDC(int Flag_Day,int Flag_DUAN)
{
    DataContainer* dc =DataContainer::getInstance();
    QVector<struct_Duan> temDUAN;
    QVector<double>* tem144Shalow;
    QVector<double>* tem144Deep;
    struct_Duan          temD;
         if(Flag_Day==1){temDUAN=dc->SUAN1;tem144Shalow=&dc->shallowSleep144_Day1;tem144Deep=&dc->deepSleep144_Day1;}
    else if(Flag_Day==2){temDUAN=dc->SUAN2;tem144Shalow=&dc->shallowSleep144_Day2;tem144Deep=&dc->deepSleep144_Day2;}
    else if(Flag_Day==3){temDUAN=dc->SUAN3;tem144Shalow=&dc->shallowSleep144_Day3;tem144Deep=&dc->deepSleep144_Day3;}
    else if(Flag_Day==4){temDUAN=dc->SUAN4;tem144Shalow=&dc->shallowSleep144_Day4;tem144Deep=&dc->deepSleep144_Day4;}
    else if(Flag_Day==5){temDUAN=dc->SUAN5;tem144Shalow=&dc->shallowSleep144_Day5;tem144Deep=&dc->deepSleep144_Day5;}
    else if(Flag_Day==6){temDUAN=dc->SUAN6;tem144Shalow=&dc->shallowSleep144_Day6;tem144Deep=&dc->deepSleep144_Day6;}

    if(Flag_DUAN<temDUAN.length())//防止段错误
        temD = temDUAN[Flag_DUAN];//得到两个坐标点 temD.startX 和 temD.endX
    //转换到容器下标  逆tem.startX=i*rate+border;  tem.endX=(i)*rate+DUAN_BORDER;
    int len =144;//10分钟
    double rate = 1.0*(dc->MarkBarWidth-DUAN_BORDER*2)/len;
    KshineStart=(temD.startX-DUAN_BORDER)/rate;
    KshineEnd=(temD.endX-DUAN_BORDER)/rate;
    //时长
    LenthOfShalow=0;
    LenthOfDeep=0;
    for(int i=KshineStart;i<=KshineEnd;i++)
    {
        if(i >= len)break;
        if((*tem144Shalow)[i])LenthOfShalow++;
        if((*tem144Deep)[i])LenthOfDeep++;
    }
    //将KshineStart KshineEnd  LenthOfShalow LenthOfDeep 转化为字符串
    if(KshineStart>72)KshineStart-=72;
    else KshineStart+=72;
    if(KshineEnd>72)KshineEnd-=72;
    else KshineEnd+=72;
    str0 = QString::number(KshineStart/6)+":"+QString::number((KshineStart%6)*10);
    str1 = QString::number(KshineEnd/6)+":"+QString::number((KshineEnd%6)*10);

    str2 = QString::number(LenthOfShalow/6)+"."+QString::number((LenthOfShalow%6)*100/6);
    str3 = QString::number(LenthOfDeep/6)+"."+QString::number((LenthOfDeep%6)*100/6);
}
