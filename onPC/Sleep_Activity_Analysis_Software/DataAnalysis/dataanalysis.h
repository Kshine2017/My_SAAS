#ifndef DATAANALYSIS_H
#define DATAANALYSIS_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include "graphplot/Myplot.h"
#include "graphplot/markframe.h"
#include "graphplot/mytimescale.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVector>

/*
类描述：界面类《数据分析》，主要功能是打开文件和分析数据，并展示
关键接口：无
*/
class DataAnalysis : public QWidget
{
    Q_OBJECT
public:
    explicit DataAnalysis(QWidget *parent = 0,int WWW=0,int HHH=0);
    ~DataAnalysis();

signals:
    void signal_restroeDeviceData();//信号：还原设备数据
    void signal_copyDeviceData();   //信号：复制设备文件
    void signal_ReadFileData(const QStringList& strlist);//信号：读取文件数据
    void signal_CompressDeal();     //信号：压缩数据
    void signal_DataAnalyze();      //信号：分析数据
    void signal_HaveDrawnpic();     //信号：绘制完成
    void work_over();               //信号：工作结束，通知主线程关闭次线程和自己
    void work_message(QString text);//信号：发送消息给主线程
    void work_Progress(qint64 progress);//信号：发送进度数值64位数值（文件的大小）
    void work_Progress_int(int num);//信号：发送进度数值（文件的个数）
    void work_TotalSize(qint64);    //信号：总文件大小
    void work_TotalNumber(int);     //信号：总文件个数

public slots:
    void slot_ShowMesageFromThread(QString text);   //槽：显示线程发来的信息
    void slot_SowWorkProgress(qint64 progress);     //槽：显示工作进度
    void slot_Start_OpenFile(const QStringList &strlist);//槽：打开文件
    void slot_Develop_DrawPicture();                //槽：绘制图形
    void slot_uploaddata();                         //槽：上传数据到PC
    void slot_openchoose();                         //槽：打开选择文件的界面
    void slot_saveresult();                         //槽：保存分析结果
    void slot_currentUser();                        //槽：当前用户信息界面
    void slot_createUsr();                          //槽：新增用户
    void slot_switchUsr();                          //槽：切换用户
    void getNewUser();                              //槽：按钮文字刷新显示
    void slot_checkboxACT(int);                     //槽：checkbox活动
    void slot_checkboxLGT(int);                     //槽：checkbox光强
    void slot_checkboxVOC(int);                     //槽：checkbox声音
    void slot_checkboxALL(int);                     //槽：checkbox全部


    //放大第一天活动图
    void slot_doubleClick1(QMouseEvent*);
    //放大第二天活动图
    void slot_doubleClick2(QMouseEvent*);
    //放大第三天活动图
    void slot_doubleClick3(QMouseEvent*);
    //放大第四天活动图
    void slot_doubleClick4(QMouseEvent*);
    //放大第五天活动图
    void slot_doubleClick5(QMouseEvent*);
    //放大第六天活动图
    void slot_doubleClick6(QMouseEvent*);

private:
    int thisWidth;                  //屏宽
    int thisHeight;                 //屏高
    bool flag_clicked_disPlay = false;  //显示图形标志
    //标记条--------------------------------------------
    MarkFrame* Markbar1;            //标记条1
    MarkFrame* Markbar2;
    MarkFrame* Markbar3;
    MarkFrame* Markbar4;
    MarkFrame* Markbar5;
    MarkFrame* Markbar6;    
    //QLabel--------------------------------------------
    QLabel* lb_Date;                //日期标签标题
    QLabel* lb_Day1;                //日期标签
    QLabel* lb_Day2;
    QLabel* lb_Day3;
    QLabel* lb_Day4;
    QLabel* lb_Day5;
    QLabel* lb_Day6;
    QLabel* pageName;               //页面名称：数据分析
    //时间刻度--------------------------------------------
    QLabel* lb_Time_Scale;          //时间刻度图（原先使用图片，后来由程序绘制，此标签不再使用）
    MyTimeScale* my_Time_Scale;     //程序绘制时间刻度
    //绘图对象--------------------------------------------
    MyPlot* myplot1;
    MyPlot* myplot2;
    MyPlot* myplot3;
    MyPlot* myplot4;
    MyPlot* myplot5;
    MyPlot* myplot6;
    //按钮-----------------------------------------------
    QPushButton* btn_createUser;    //新建用户
    QPushButton* btn_switchUser;    //切换用户
    QPushButton* btn_UPLoadData;
    QPushButton* btn_OpenAndChoose;
    QPushButton* btn_SaveResult;
    QPushButton* btn_currentUser;   //当前用户
    QLabel* lb_currentUser;         //当前用户标签
    //checkbox-------------------------------------------
    QCheckBox* ckb_voice;           //显示控制按钮（勾选框）
    QCheckBox* ckb_light;
    QCheckBox* ckb_active;
    QCheckBox* ckb_all;
    //布局------------------------------------------------
    QVBoxLayout* ly_V_all;
    QHBoxLayout* ly_H_Top;
    QHBoxLayout* ly_H_Sec;
    QHBoxLayout* ly_H_Title;
    QHBoxLayout* ly_H_plot1;
    QHBoxLayout* ly_H_plot2;
    QHBoxLayout* ly_H_plot3;
    QHBoxLayout* ly_H_plot4;
    QHBoxLayout* ly_H_plot5;
    QHBoxLayout* ly_H_plot6;
    //界面布局，初始化函数-----------------------------------
    void init_PageTopInfo();        //顶部
    void init_PageSecInfo();        //菜单栏
    void init_PageTitleInfo();      //标签行
    void init_PagePlotInfo();       //绘图控件
    void init_PageAllLayout();      //布局
    void init_MarkFrame();          //标记条
    void init_allStatus();          //初始绘图
    void AutoMark();                //自动标记（已弃）
};

#endif // DATAANALYSIS_H
