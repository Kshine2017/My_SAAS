#include "openpdf.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QLabel>
#include <QVBoxLayout>
#include "poppler-qt5.h"
#include "print/myprintpreviewdialog.h"
#include <QDebug>
#include <QMessageBox>
#include "opration/movefile.h"
#include <QFileDialog>
OpenPDF::OpenPDF(QWidget *parent,QString filepath) : QDialog(parent)
{
    QDesktopWidget* desktop = QApplication::desktop();
    thisWidth=desktop->width();
    thisHeight=desktop->height();
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(thisHeight/1.414+30,thisHeight-70);
    setWindowTitle("打开报告");
    scrollArea = new QScrollArea(this);
    scrollArea->setFixedSize(thisHeight/1.414+40,thisHeight-100);
    scrollArea->setFrameShape(QFrame::NoFrame);
    fileName = filepath;
    if(fileName.isNull() || !fileName.contains(".pdf")) return;
    Poppler::Document * document = Poppler::Document::load(fileName);
    QWidget * widget = new QWidget(this);

    QVBoxLayout *vboxLayout = new QVBoxLayout(this);

    for(int i = 0; i < document->numPages(); i ++) //根据获取到的pdf页数循环
    {
        QLabel * label = new QLabel(this);

        QImage image = document->page(i)->renderToImage(300, 300); //截取pdf文件中的相应图片
        //QPixmap fitpixmap = QPixmap::fromImage(image).scaled(thisWidth, thisHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
        QPixmap fitpixmap = QPixmap::fromImage(image).scaled(thisHeight/1.414, thisHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
        label->setPixmap(fitpixmap);
        vboxLayout->addWidget(label);
        vboxLayout->setAlignment(widget,Qt::AlignCenter);
    }
    widget->setLayout(vboxLayout); //设置布局
    scrollArea->setWidget(widget); //设置widget
    show();
    //------------------------
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
    ly_H_Sec = new QHBoxLayout();
    ly_H_Sec->addSpacing(10);
    ly_H_Sec->addWidget(btn_print);
    ly_H_Sec->addSpacing(40);
    ly_H_Sec->addWidget(btn_outputPDF);
    ly_H_Sec->addStretch();
    ly_H_Sec->addSpacing(10);
    ly_V_all = new QVBoxLayout();
    ly_V_all->addLayout(ly_H_Sec);
    ly_V_all->addWidget(scrollArea);
    setLayout(ly_V_all);
}

OpenPDF::~OpenPDF()
{
    delete ly_H_Sec;
    delete ly_V_all;
}

void OpenPDF::slot_print()
{
    MyPrintPreViewDialog MPPV;
    bool ret = MPPV.printPDF(fileName);
    qDebug()<<fileName;
    qDebug()<<"打印PDF文件："<<ret;
    if(!ret)QMessageBox::warning(this,"失败","未能成功打印，请尝试导出后在文件中打印！");
}

void OpenPDF::slot_outputPDF()
{
    QString TTTfileName = QFileDialog::getSaveFileName(this, tr("保存文件"), tr(""), tr("pdf文件 (*.pdf)"));
    qDebug()<<TTTfileName;
    MoveFile mf;
    mf.filename=fileName;
    mf.toWhere=TTTfileName;
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
}
