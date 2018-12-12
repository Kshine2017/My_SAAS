#ifndef PDFANDPRINT_H
#define PDFANDPRINT_H

#include <QWidget>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
class PDFandPrint : public QWidget
{
    Q_OBJECT
public:
    explicit PDFandPrint(QWidget *parent = 0, int WWW=0, int HHH=0);

    QScrollArea* scrollArea;
    QPushButton* btn_file;//测试打开pdf的按钮
    int thisWidth;              //屏宽
    int thisHeight;             //屏高
    //top
    QLabel* pageName;           //页面名称：数据分析
    //sec
    QPushButton* btn_print;         //打印按钮
    QPushButton* btn_printpreview;  //打印预览按钮
    QPushButton* btn_outputPDF;
    QPushButton* btn_saveTOdatabase;
    //初始化函数
    void init_PageTopInfo();
    void init_PageSecInfo();
    void init_PageThrInfo();
    void init_all_layout();
    void autoOpenFile();
    //布局
    QVBoxLayout* ly_V_all;
    QHBoxLayout* ly_H_Top;
    QHBoxLayout* ly_H_Sec;


signals:
    void signal_getinDataAnalysisWindow();
    void signal_getinExportReportWindow();
public slots:
    void slot_btn_file();
    void slot_print();
    void slot_printpreview();
    void slot_saveToDatabase();
    void slot_outputPDF();
};

#endif // PDFANDPRINT_H
