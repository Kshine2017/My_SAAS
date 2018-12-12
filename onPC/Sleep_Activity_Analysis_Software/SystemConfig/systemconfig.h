#ifndef SYSTEMCONFIG_H
#define SYSTEMCONFIG_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>
#include <QTextEdit>

/*
类描述：界面类《系统》，对硬件进行设置，对软件进行设置，对管理用户进行设置
关键接口：无
*/
class SystemConfig : public QWidget
{
    Q_OBJECT
public:
    explicit SystemConfig(QWidget *parent = 0, int WWW=0, int HHH=0);

signals:
    void signal_restroeDeviceData();    //还原设备数据
private:
    QWidget *parentthis;                //父类
    QTabWidget* tabwidget;              //切换类
    int thisWidth;                      //屏宽
    int thisHeight;                     //屏高
    QString DeviceDiskNumber;           //设备所在盘符
    QLabel* pageName;                   //页面名称：数据分析

    QPushButton* btn_DeviceSet;         //按钮，设备设置
    QPushButton* btn_SoftwareSet;       //按钮，软件设置
    QPushButton* btn_PersonalInfoSet;   //按钮，用户信息设置

    QWidget* widgetDevice;              //设备设置界面
    QWidget* widgetSoftware;            //软件设置界面
    QWidget* widgetPersonalInfo;        //用户信息设置界面

    //--widgetDevice-------------------//设备设置界面组成
    QPushButton* btn_CheckDeviceStatus;
    QPushButton* btn_ReadLogFile;
    QPushButton* btn_RestoreDeviceDefaultData;
    QPushButton* btn_FormatDevice;
    QPushButton* btn_DeviceParameters;
    QTextEdit* textedit;
    //--widgetSoftware-----------------//软件设置界面组成
    QPushButton* btn_SetPageStytle;
    QPushButton* btn_SetTipsInfo;


    //--widgetPersonalInfo-------------//用户界面组成
    QPushButton* btn_SetLoginInfo;




    //初始化
    void init_PageTopInfo();
    void init_PageSecInfo();

    void init_widgetDevice();
    void init_widgetSoftWare();
    void init_widgetPersonalInfo();

public slots:
    void slot_ShowMesageFromThread(QString text);//显示线程信息
    void slot_SowWorkProgress(qint64 progress);  //显示工作进度
    void slot_btn_checkdevicestatus();           //检查设备连接状态
    void slot_btn_fomatDevice();                 //格式化设备
    void slot_btn_restoreDeviceData();           //还原设备数据文件
    void slot_btn_readlogfile();                 //读取日志文件

    //-----------------------
    void slot_btn_manageTipsInfo();              //管理建议信息
    void slot_btn_usart();

};

#endif // SYSTEMCONFIG_H
