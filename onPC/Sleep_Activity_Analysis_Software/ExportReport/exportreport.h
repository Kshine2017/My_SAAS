#ifndef EXPORTREPORT_H
#define EXPORTREPORT_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "graphplot/statistics.h"
#include <QScrollArea>

/*
类描述：界面类《生成报告》，主要功能是展示分析数据，生成报告文件，打印。
关键接口：无
*/
class ExportReport : public QWidget
{
    Q_OBJECT
public:
    explicit ExportReport(QWidget *parent = 0,int WWW=0,int HHH=0);

signals:
    void signal_getinDataAnalysisWindow();
public slots:
    void slot_print();          //槽：打印
    void slot_printpreview();
    void slot_saveResult();
private:
    int thisWidth;              //屏宽
    int thisHeight;             //屏高
    //top
    QLabel* pageName;           //页面名称：数据分析
    //sec
    QPushButton* btn_print;         //打印按钮
    QPushButton* btn_printpreview;  //打印预览按钮
    QPushButton* btn_saveResult;
    //info
    QWidget* WidgetLB;          //底色板
    QLabel* lb_numberID;        //婴儿或观测对象的ID号
    QLabel* lb_Date;            //婴儿或观测对象的生日
    QLabel* lb_name;            //婴儿或观测对象的姓名
    QLabel* lb_age;             //婴儿或观测对象的年龄
    QLabel* lb_gander;          //婴儿或观测对象的性别
    //临时使用的图片
    QLabel* lb_report1;
    QLabel* lb_report2;
    QLabel* lb_report3;
    //一天的数据表
    QWidget* SWidget;           //底板
    Statistics* SDAY1;          //第一天的数据表
    Statistics* SDAY2;          //第二天的数据表
    Statistics* SDAY3;          //第三天的数据表
    Statistics* SDAY4;          //第四天的数据表
    Statistics* SDAY5;          //第五天的数据表
    Statistics* SDAY6;          //第六天的数据表
    QScrollArea *scrollArea;
    //布局
    QVBoxLayout* ly_V_all;
    QHBoxLayout* ly_H_Top;
    QHBoxLayout* ly_H_Sec;
    QHBoxLayout* ly_H_Thrid_up;
    QHBoxLayout* ly_H_Thrid_down;
    QVBoxLayout* ly_V_Thrid;
    //初始化函数
    void init_PageTopInfo();
    void init_PageSecInfo();
    void init_Statistics();
    void init_PageThirdInfo();
    void init_PageReportInfo();
    void init_PageAllLayout();
    //测试数据
    void testTableData();
    void calculate();
};

#endif // EXPORTREPORT_H
