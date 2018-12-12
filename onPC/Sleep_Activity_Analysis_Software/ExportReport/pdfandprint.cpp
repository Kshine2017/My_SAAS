#include "pdfandprint.h"
#include "poppler-qt5.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QPixmap>
#include "print/myprintpreviewdialog.h"
#include "DataContainer/datacontainer.h"
#include <QMessageBox>
#include "opration/movefile.h"
#include <QDebug>
#include "DataBase/infodaoimp.h"
#define dateedit_format "yyyy-MM-dd"
#define dateedit_format_all "yyyy-MM-dd hh:mm:ss"
#define openfile_enable false
PDFandPrint::PDFandPrint(QWidget *parent, int WWW, int HHH) : QWidget(parent)
{

    if(WWW&&HHH)
    {
        //创建时指定大小
        thisWidth=WWW;
        thisHeight=HHH;
    }
    else
    {
        //默认子页面大小
        thisWidth=1920;
        thisHeight=1080;
        this->setStyleSheet("background-color:yellow;");
    }
    this->setFixedSize(thisWidth,thisHeight);
    this->setAutoFillBackground(true);
    init_PageTopInfo();
    init_PageSecInfo();
    init_PageThrInfo();
    init_all_layout();
    if(DataContainer::getInstance()->KFlag_PDF)
    autoOpenFile();
}

void PDFandPrint::init_PageTopInfo()
{
    pageName =new  QLabel(this);
    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor(92,123,137));
    pageName->setPalette(pa);
    pageName->setFont(QFont("楷体",22,75));
    pageName->setText("生成报告");

    ly_H_Top = new QHBoxLayout();
    ly_H_Top->addSpacing(10);
    ly_H_Top->addWidget(pageName);
    ly_H_Top->addStretch();
    ly_H_Top->addSpacing(10);


}

void PDFandPrint::init_PageSecInfo()
{
    btn_print = new QPushButton(this);
    btn_print->setFixedSize(130,30);
    btn_print->setIcon(QIcon(":/picture/huangzhijie/打印.png"));
    btn_print->setText(" 打印");
    btn_print->setStyleSheet(""
                                  "QPushButton{font:18px;font-weight:bold;color:rgb(96 , 125 , 139);background-color:transparent;}"
                                  "QPushButton:hover {font:20px;color:rgb(136 , 136 , 136);}"
                                  "QPushButton:pressed { color:rgb(0 , 0 , 0);}"
                                  "");
    connect(btn_print,SIGNAL(clicked(bool)),this,SLOT(slot_print()));




    btn_printpreview = new QPushButton(this);
    btn_printpreview->setFixedSize(130,30);
    btn_printpreview->setIcon(QIcon(":/picture/huangzhijie/打印预览.png"));
    btn_printpreview->setText(" 打印预览");
    btn_printpreview->setStyleSheet(""
                                  "QPushButton{font:18px;font-weight:bold;color:rgb(96 , 125 , 139);background-color:transparent;}"
                                  "QPushButton:hover {font:20px;color:rgb(136 , 136 , 136);}"
                                  "QPushButton:pressed { color:rgb(0 , 0 , 0);}"
                                  "");
    connect(btn_printpreview,SIGNAL(clicked(bool)),this,SLOT(slot_printpreview()));

    btn_outputPDF = new QPushButton(this);
    btn_outputPDF->setFixedSize(150,30);
    btn_outputPDF->setIcon(QIcon(":/picture/huangzhijie/导出.png"));
    btn_outputPDF->setText(" 导出PDF文件");
    btn_outputPDF->setStyleSheet(""
                                  "QPushButton{font:18px;font-weight:bold;color:rgb(96 , 125 , 139);background-color:transparent;}"
                                  "QPushButton:hover {font:20px;color:rgb(136 , 136 , 136);}"
                                  "QPushButton:pressed { color:rgb(0 , 0 , 0);}"
                                  "");
    connect(btn_outputPDF,SIGNAL(clicked(bool)),this,SLOT(slot_outputPDF()));

    btn_saveTOdatabase = new QPushButton(this);
    btn_saveTOdatabase->setFixedSize(150,30);
    btn_saveTOdatabase->setIcon(QIcon(":/picture/huangzhijie/保存.png"));
    btn_saveTOdatabase->setText(" 保存到数据库");
    btn_saveTOdatabase->setStyleSheet(""
                                  "QPushButton{font:18px;font-weight:bold;color:rgb(96 , 125 , 139);background-color:transparent;}"
                                  "QPushButton:hover {font:20px;color:rgb(136 , 136 , 136);}"
                                  "QPushButton:pressed { color:rgb(0 , 0 , 0);}"
                                  "");
    connect(btn_saveTOdatabase,SIGNAL(clicked(bool)),this,SLOT(slot_saveToDatabase()));

    ly_H_Sec = new QHBoxLayout();

    ly_H_Sec->addSpacing(10);
    ly_H_Sec->addWidget(btn_print);
    ly_H_Sec->addSpacing(40);
    ly_H_Sec->addWidget(btn_printpreview);
    ly_H_Sec->addSpacing(40);
    ly_H_Sec->addWidget(btn_outputPDF);
    ly_H_Sec->addSpacing(40);
    ly_H_Sec->addWidget(btn_saveTOdatabase);
    ly_H_Sec->addStretch();
    ly_H_Sec->addSpacing(10);
}

void PDFandPrint::init_PageThrInfo()
{
    scrollArea = new QScrollArea(this);
    scrollArea->setFixedSize(thisWidth,thisHeight-70);
    scrollArea->setFrameShape(QFrame::NoFrame); //设置滑动区域无边框
    if(openfile_enable)
    {
        btn_file = new QPushButton(this);
        connect(btn_file,SIGNAL(clicked(bool)),this,SLOT(slot_btn_file()));
    }
}

void PDFandPrint::init_all_layout()
{
    ly_V_all = new QVBoxLayout();
    ly_V_all->addSpacing(10);
    ly_V_all->addLayout(ly_H_Top);

    QLabel *m_label = new QLabel(this);
    m_label->setFrameShape(QFrame::HLine);
    m_label->setFrameShadow(QFrame::Raised);
    m_label->setMaximumSize(QSize(6000, 1));
    ly_V_all->addWidget(m_label);

    ly_V_all->addLayout(ly_H_Sec);

    QLabel *m_label2 = new QLabel(this);
    m_label2->setFrameShape(QFrame::HLine);
    m_label2->setFrameShadow(QFrame::Raised);
    m_label2->setMaximumSize(QSize(6000, 1));
    ly_V_all->addWidget(m_label2);

    ly_V_all->addWidget(scrollArea);


    ly_V_all->addStretch();

    ly_V_all->setGeometry(QRect(0,0,thisWidth,thisHeight));
}

void PDFandPrint::autoOpenFile()
{
    /*********************************************************************************
    * 思路：
    * 将打开的pdf每页提取成图片，将每张图片放入一个label控件中，
    * 设置一个垂直布局，将多个label控件垂直起来放入widget
    * 然后将widget放入scrollArea控件中
    * *******************************************************************************/
    QString fileName = "./TEMPIC/tempdf.pdf";

    if(fileName.isNull() || !fileName.contains(".pdf")) return;

    Poppler::Document * document = Poppler::Document::load(fileName); //将pdf文件加载进Document

    QWidget * widget = new QWidget(this);

    QVBoxLayout *vboxLayout = new QVBoxLayout(this);

    for(int i = 0; i < document->numPages(); i ++) //根据获取到的pdf页数循环
    {
    QLabel * label = new QLabel(this);

      QImage image = document->page(i)->renderToImage(216, 216); //截取pdf文件中的相应图片
   // QPixmap fitpixmap = QPixmap::fromImage(image).scaled(thisWidth, thisHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
      QPixmap fitpixmap = QPixmap::fromImage(image).scaled(thisWidth-40, thisWidth*1.414, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放

        label->setPixmap(fitpixmap);

        vboxLayout->addWidget(label);
        vboxLayout->setAlignment(widget,Qt::AlignCenter);
        }
        widget->setLayout(vboxLayout); //设置布局

        scrollArea->setWidget(widget); //设置widget
}

void PDFandPrint::slot_btn_file()
{
    /*********************************************************************************
    * 思路：
    * 将打开的pdf每页提取成图片，将每张图片放入一个label控件中，
    * 设置一个垂直布局，将多个label控件垂直起来放入widget
    * 然后将widget放入scrollArea控件中
    * *******************************************************************************/
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择文件"), tr(""), tr("pdf文件 (*.pdf)"));

    if(fileName.isNull() || !fileName.contains(".pdf")) return;

    Poppler::Document * document = Poppler::Document::load(fileName); //将pdf文件加载进Document

    QWidget * widget = new QWidget(this);

    QVBoxLayout *vboxLayout = new QVBoxLayout(this);

    for(int i = 0; i < document->numPages(); i ++) //根据获取到的pdf页数循环
    {
    QLabel * label = new QLabel(this);

      QImage image = document->page(i)->renderToImage(216, 216); //截取pdf文件中的相应图片
   // QPixmap fitpixmap = QPixmap::fromImage(image).scaled(thisWidth, thisHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
      QPixmap fitpixmap = QPixmap::fromImage(image).scaled(thisWidth, thisWidth*1.414, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放

        label->setPixmap(fitpixmap);

        vboxLayout->addWidget(label);
        vboxLayout->setAlignment(widget,Qt::AlignCenter);
        }
        widget->setLayout(vboxLayout); //设置布局

        scrollArea->setWidget(widget); //设置widget

}

void PDFandPrint::slot_print()
{

    DataContainer* dc = DataContainer::getInstance();
    if(dc->ChooseNUM==0)
    {
        QMessageBox::information(this,"提示","尚未选择文件进行分析，请至数据分析页面！",QMessageBox::Ok);
        emit signal_getinDataAnalysisWindow();
        return;
    }
    if(dc->KFlag_PDF==0)
    {
        QMessageBox::information(this,"提示","尚未生成PDF，请至睡眠统计页面，进行确认！",QMessageBox::Ok);
        emit signal_getinExportReportWindow();
        return;
    }
    MyPrintPreViewDialog MPPVD(this);
    MPPVD.choosenum=dc->ChooseNUM;
    MPPVD.ID=dc->ID;
    MPPVD.Name=dc->PatientName;
    MPPVD.Gander=dc->Gander;
    MPPVD.Brithday=dc->BrithDay;
    MPPVD.Age=dc->Age;
    MPPVD.Date=QDateTime::currentDateTime().toString(dateedit_format);
    //数据--------------------------------------
    for(int i=0;i<dc->strlist.length();i++)
    {
        MPPVD.strlist.append(dc->strlist[i]);
    }
    //建议---------------------------------
    for(int i=0;i<dc->ChooseNUM;i++)
    {
        QString tips;
        QString day="";
        if(i==0){ tips=dc->DoctorTips1;day=dc->day1;}
        if(i==1){ tips=dc->DoctorTips2;day=dc->day2;}
        if(i==2){ tips=dc->DoctorTips3;day=dc->day3;}
        if(i==3){ tips=dc->DoctorTips4;day=dc->day4;}
        if(i==4){ tips=dc->DoctorTips5;day=dc->day5;}
        if(i==5){ tips=dc->DoctorTips6;day=dc->day6;}
        if(!tips.isEmpty())
        MPPVD.strlist_advice.append(day+": "+tips);
    }
    //---------------------------------------
    MPPVD.print();
}

void PDFandPrint::slot_printpreview()
{
    DataContainer* dc = DataContainer::getInstance();
    if(dc->ChooseNUM==0)
    {
        QMessageBox::information(this,"提示","尚未选择文件进行分析，请至数据分析页面！",QMessageBox::Ok);
        emit signal_getinDataAnalysisWindow();
        return;
    }
    if(dc->KFlag_PDF==0)
    {
        QMessageBox::information(this,"提示","尚未生成PDF，请至睡眠统计页面，进行确认！",QMessageBox::Ok);
        emit signal_getinExportReportWindow();
        return;
    }
    MyPrintPreViewDialog MPPVD(this);
    MPPVD.choosenum=dc->ChooseNUM;
    MPPVD.ID=dc->ID;
    MPPVD.Name=dc->PatientName;
    MPPVD.Gander=dc->Gander;
    MPPVD.Brithday=dc->BrithDay;
    MPPVD.Age=dc->Age;
    MPPVD.Date=QDateTime::currentDateTime().toString(dateedit_format);
    //数据--------------------------------------
    for(int i=0;i<dc->strlist.length();i++)
    {
        MPPVD.strlist.append(dc->strlist[i]);
    }
    //建议---------------------------------
    for(int i=0;i<dc->ChooseNUM;i++)
    {
        QString tips;
        QString day="";
        if(i==0){ tips=dc->DoctorTips1;day=dc->day1;}
        if(i==1){ tips=dc->DoctorTips2;day=dc->day2;}
        if(i==2){ tips=dc->DoctorTips3;day=dc->day3;}
        if(i==3){ tips=dc->DoctorTips4;day=dc->day4;}
        if(i==4){ tips=dc->DoctorTips5;day=dc->day5;}
        if(i==5){ tips=dc->DoctorTips6;day=dc->day6;}
        if(!tips.isEmpty())
        MPPVD.strlist_advice.append(day+": "+tips);
    }
    MPPVD.printpreview();
}

void PDFandPrint::slot_saveToDatabase()
{
    DataContainer* dc = DataContainer::getInstance();
    MoveFile mf;
    mf.filename="./TEMPIC/tempdf.pdf";
    mf.toWhere="./Active_Report";
    mf.name =dc->ID;
    mf.targetFilename=dc->ID+QDateTime::currentDateTime().toString("yyyyMMdd")+"0000.pdf";
    //-----------------------
    if(mf.PDFisExist())
    {
        if(QMessageBox::question(this,"信息","是否覆盖当天已有的PDF文件？",QMessageBox::Yes|QMessageBox::No)==QMessageBox::No)
        {
            return;
        }
        else
        {
            mf.deletePDF();
        }
    }
    mf.movePDFtoDir();

    //保存到数据库-----------------------
    QString PDFname =   dc->ID+QDateTime::currentDateTime().toString("yyyyMMdd")+"0000.pdf";
    QString ID      =   dc->ID;
    QString Name    =   dc->PatientName;
    QString Date    =   QDateTime::currentDateTime().toString("yyyyMMdd");
    QString SleepInfo   =   "暂无";
    QString CDT     =   QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString CR      =   "admin";

    InfoDaoImp idi(this);

    idi.insertData_ReportInfo(PDFname,ID,Name,Date,"0000",SleepInfo,CDT,CR,"无","无","0");
    QMessageBox::information(this,"成功","保存成功！",QMessageBox::Yes);
}

void PDFandPrint::slot_outputPDF()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存文件"), tr(""), tr("pdf文件 (*.pdf)"));
    qDebug()<<fileName;
    MoveFile mf;
    mf.filename="./TEMPIC/tempdf.pdf";
    mf.toWhere=fileName;
    //-----------------------
    if(mf.PDFisExist_X())
    {
        if(QMessageBox::question(this,"信息","是否覆盖该位置已有的PDF文件？",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
        {
            return;
        }
        else
        {
            mf.deletePDF_X();
        }
    }
    mf.outputPDF();
    //QMessageBox::information(this,"成功","保存成功！",QMessageBox::Yes);
}
