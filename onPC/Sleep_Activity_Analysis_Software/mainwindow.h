#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>


#include "DataAnalysis/dataanalysis.h"
#include "DataManagement/datamanagement.h"
#include "ExportReport/exportreport.h"
#include "SystemConfig/systemconfig.h"
#include "Login/loginsecondwidget.h"
#include "ExportReport/pdfandprint.h"
#include "HomePage/homepage.h"

//namespace Ui {
//class MainWindow;
//}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:
    void slot_quitButton();
    //创建哥功能界面
    void backto_HomePage();
    void create_ManagementPageWindow();
    void create_AnalzyPageWindow();
    void create_ExportReportWindow();
    void create_UserPageWindow();
    void create_SystemConfig();
    void create_pdfprint();
    //-----------------------
    void slot_LOGO();
    void slot_createPatient();
    void slot_choosePatient();
private:
    //主界面label
    //顶部标签
    QLabel* lb_UP_background;//上方长条背景
    QLabel* lb_LOGO;        //logo图片
    QPushButton* btn_LOGO;        //logo图片
    QLabel* lb_CompanyName; //公司名称
    QLabel* lb_SoftWareName;//软件名称
    QLabel* lb_Version;     //版本
    QHBoxLayout* ly_lb_H;   //标签的水平布局
    QHBoxLayout* ly_lb_H2;   //标签的水平布局
    void init_UP_Label();
    //左侧按钮
    QLabel* lb_Left_Background;
    QPushButton* btn_Home;              //回到主界面
    QPushButton* btn_DataAnalyze;   //界面1
    QPushButton* btn_DataManage;    //界面2
    QPushButton* btn_ExportReport;  //界面3
    QPushButton* btn_SystemConfig;  //界面4
    QPushButton* btn_pdfprint;      //新界面，2.5
    QPushButton* btn_Quit;          //退出
    QVBoxLayout* ly_btn_V;          //按钮的垂直布局
    void init_Left_Btn();
    //驻留窗口
    QDockWidget* UpDock;//上方驻留条
    QWidget* upwidget;

    QDockWidget* DockWindow;//左侧工具条
    QWidget* UserWindow; //右侧
    void create_CCNdialog();//创建、选择、无 的界面
    void initUSB();
    char FirstDriveFromMask (ULONG unitmask);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
private:
//    Ui::MainWindow *ui;
    int thisWIDTH;
    int thisHEIGHT;
    int Page_flag;


    LoginSecondWidget*  LSdialog;
    DataAnalysis*       analysisWindow;
    DataManagement*     manageWindow;
    ExportReport*       reportWindow;
    SystemConfig*       systemWindow;
    PDFandPrint*        pdfprintWindow;
    Homepage*           homewindow;
};

#endif // MAINWINDOW_H
