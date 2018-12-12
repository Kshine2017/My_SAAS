#include "myprintpreviewdialog.h"
#include "DataContainer/datacontainer.h"
#include <QPrintPreviewDialog>
#include <QPainter>
#include "poppler-qt5.h"
#include <QFileInfo>
#include <windows.h>
#include <QDebug>

#include <QPrintDialog>

#define fontsize        10
#define rowHeight       35                //行高
#define rowCount         7                //行数
#define columnCount      7                //列数
#define columnWidth     95                //列宽

#define Y_tablestart    630             //表格起始纵坐标
#define D_zhu           250             //注 向上间距(以表格起始点为基准)
#define D_advice        20              //指导意见 向上间距（以注为基准）
/*
备注：下方那些坐标数据出自黄志杰之手。
*/
MyPrintPreViewDialog::MyPrintPreViewDialog(QWidget *parent):QDialog(parent)
{
    choosenum=0;
}

MyPrintPreViewDialog::~MyPrintPreViewDialog()
{

}

bool MyPrintPreViewDialog::printPDF(QString fileName)
{
    QPrinter printer;
    QString printerName = printer.printerName();
    // no default printer or no file return false
    if( printerName.size() == 0 || fileName.size() == 0)
        return false;
    QFileInfo fi(fileName);
    QString filePath = fi.absoluteFilePath();
    if(!fi.exists())
        return false;
    int ret = 0;
    ret = (int)ShellExecuteW(NULL,
                             QString("print").toStdWString().c_str(),
                             filePath.toStdWString().c_str(),
                             NULL,
                             NULL,
                             SW_HIDE);
    // if return number bigger than 31 indicate succ
    qDebug()<<"ret:"<<ret<<"--SE_ERR_NOASSOC:"<<SE_ERR_NOASSOC;
    qDebug()<<"filePath:"<<filePath.toStdWString().c_str();
    if (ret > SE_ERR_NOASSOC)
        return true;
    return false;
}
bool MyPrintPreViewDialog::print()
{

    //1,创建打印机
    QPrinter printer(QPrinter::HighResolution);
    //2,自定义纸张大小
    printer.setPageSize(QPrinter::Custom);//页
    //4，创建画家
    double rate =6.0;
    QPainter painter;
    painter.begin(&printer);
    //5.打印内容
    //QRect rect = this->geometry();
    QPixmap pixmap;
    pixmap.load("./TEMPIC/"+QString::number(choosenum)+".png");
    painter.drawPixmap(50*rate,185*rate,750*rate,(25+choosenum*60)*rate,pixmap);

    //画表格
    QPen pen = painter.pen();
    pen.setWidth(1);
    painter.setPen(pen);
    QFont font("宋体",fontsize);
    painter.setFont(font);
    //画行线
    for (int i = 0; i <= rowCount; i++)
    {
        painter.drawLine(60*rate, (Y_tablestart + rowHeight*i)*rate, (60 + columnWidth * columnCount)*rate, (Y_tablestart + rowHeight * i)*rate);
    }

    //画列线
    for (int i = 0; i <= columnCount; i++)
    {
        painter.drawLine((60 + i * columnWidth)*rate, Y_tablestart*rate, (60 + i * columnWidth)*rate, (Y_tablestart + rowCount * rowHeight)*rate);
    }
    QStringList headStrList;
    headStrList.append("日    期");
    headStrList.append("白天睡眠次数");
    headStrList.append("白天深睡时长");
    headStrList.append("白天浅睡时长");
    headStrList.append("夜晚睡眠次数");
    headStrList.append("夜晚深睡时长");
    headStrList.append("夜晚浅睡时长");
    //表头
    for (int i = 0; i < columnCount; i++)
    {
        painter.drawText((60 + i * columnWidth + columnWidth - 90)*rate , (Y_tablestart + rowHeight - 28)*rate,100*rate,30*rate,1,headStrList[i]);
    }
    // 标题
    painter.setFont(QFont("宋体",24,40));
    QString title = "睡眠报告";
    painter.drawText(350*rate, 20*rate,300*rate,60*rate,0,title);
    painter.setFont(QFont("宋体",10));

    //编号标签
    QString id = "编号："+ID;
    painter.drawText(10*rate, 100*rate ,200*rate,14*rate,0,id);

    //姓名标签
    QString name = "姓名："+Name;
    painter.drawText(140*rate, 100*rate ,200*rate,14*rate,0,name);

    //性别标签
    QString gender = "性别："+Gander;
    painter.drawText(250*rate, 100*rate ,200*rate,14*rate,0,gender);

    //出生日期标签
    QString bornData = "出生日期："+Brithday;
    painter.drawText(320*rate, 100*rate ,400*rate,14*rate,0,bornData);

    //年龄标签
    QString age = "年龄："+Age;
    painter.drawText(505*rate, 100*rate ,300*rate,14*rate,0,age);

    //日期标签
    QString date = "日期："+Date;
    painter.drawText(610*rate, 100*rate ,300*rate,14*rate,0,date);


    //图示
    QString attogram = "图示：";
    painter.drawText(40*rate, 160*rate ,40*rate,14*rate,0,attogram);

    //注
    QString str = "*注：上面的每一天都是从第一天的12:00到第二天的12:00。";
    painter.drawText(80*rate, (Y_tablestart+D_zhu)*rate ,600*rate,14*rate,0,str);

    //每日统计
    QString daily = "每日统计:";
    painter.drawText(40*rate, (Y_tablestart-30)*rate ,60*rate,14*rate,0,daily);


    for(int i=0;i<choosenum;i++)
    {
        for(int j=0;j<columnCount;j++)
        {
            painter.drawText((60 + j * columnWidth + columnWidth - 90)*rate , (Y_tablestart + (i+2)*rowHeight - 28)*rate,100*rate,30*rate,1,strlist[j+columnCount*i]);
        }
    }
    //指导意见：
    //注
    QString advice = "指导意见:";
    painter.drawText(60*rate, (Y_tablestart+D_zhu+D_advice)*rate ,600*rate,14*rate,0,advice);
    int nnn=0;
    for (int i = 1; i <= strlist_advice.length(); i++)
    {
        QString tem=strlist_advice[i-1];
        nnn+=tem.count('\n');
        painter.drawText((80)*rate , (Y_tablestart+D_zhu+D_advice+(i+nnn)*20)*rate,800*rate,60*rate,1,tem);
    }



    painter.end();
    return true;
}

bool MyPrintPreViewDialog::printpreview()
{
    //1,创建打印机
    QPrinter printer(QPrinter::HighResolution);
    //2,自定义纸张大小
    printer.setPageSize(QPrinter::Custom);//页
    //printer.setPaperSize(QSizeF(600,230),QPrinter::Point);//纸张大小
    //3，创建预览窗口
    QPrintPreviewDialog preview(&printer, this);
    //preview.setMinimumSize(841,400);//841==100%显示比例

    connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPreviewSlot(QPrinter*)));
    if(preview.exec ()==QDialog::Accepted)
    {
        //点击预览打印
    }
    return true;
}

void MyPrintPreViewDialog::exportPDF()
{
    //1,创建打印机
    QPrinter printer(QPrinter::HighResolution);
    //2,自定义纸张大小
    printer.setPageSize(QPrinter::Custom);//页
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("./TEMPIC/tempdf.pdf");
    //4，创建画家
    double rate =12.0;
    QPainter painter;
    painter.begin(&printer);
    //5.打印内容
    //QRect rect = this->geometry();
    QPixmap pixmap;
    pixmap.load("./TEMPIC/"+QString::number(choosenum)+".png");
    painter.drawPixmap(50*rate,185*rate,750*rate,(25+choosenum*60)*rate,pixmap);

    //画表格
    QPen pen = painter.pen();
    pen.setWidth(1);
    painter.setPen(pen);
    QFont font("宋体",fontsize);
    painter.setFont(font);
    //画行线
    for (int i = 0; i <= rowCount; i++)
    {
        painter.drawLine(60*rate, (Y_tablestart + rowHeight*i)*rate, (60 + columnWidth * columnCount)*rate, (Y_tablestart + rowHeight * i)*rate);
    }

    //画列线
    for (int i = 0; i <= columnCount; i++)
    {
        painter.drawLine((60 + i * columnWidth)*rate, Y_tablestart*rate, (60 + i * columnWidth)*rate, (Y_tablestart + rowCount * rowHeight)*rate);
    }
    QStringList headStrList;//("日    期", "就寝时间","入睡时间", "起床时间", "卧床时长", "睡眠时长", "睡眠效率");
    headStrList.append("日    期");
    headStrList.append("白天睡眠次数");
    headStrList.append("白天深睡时长");
    headStrList.append("白天浅睡时长");
    headStrList.append("夜晚睡眠次数");
    headStrList.append("夜晚深睡时长");
    headStrList.append("夜晚浅睡时长");
    //表头
    for (int i = 0; i < columnCount; i++)
    {
        painter.drawText((60 + i * columnWidth + columnWidth - 90)*rate , (Y_tablestart + rowHeight - 28)*rate,100*rate,30*rate,1,headStrList[i]);
    }
    // 标题
    painter.setFont(QFont("宋体",24,40));
    QString title = "睡眠报告";
    painter.drawText(350*rate, 20*rate,300*rate,60*rate,0,title);
    painter.setFont(QFont("宋体",10));

    //编号标签
    QString id = "编号："+ID;
    painter.drawText(10*rate, 100*rate ,200*rate,14*rate,0,id);

    //姓名标签
    QString name = "姓名："+Name;
    painter.drawText(140*rate, 100*rate ,200*rate,14*rate,0,name);

    //性别标签
    QString gender = "性别："+Gander;
    painter.drawText(250*rate, 100*rate ,200*rate,14*rate,0,gender);

    //出生日期标签
    QString bornData = "出生日期："+Brithday;
    painter.drawText(320*rate, 100*rate ,400*rate,14*rate,0,bornData);

    //年龄标签
    QString age = "年龄："+Age;
    painter.drawText(505*rate, 100*rate ,300*rate,14*rate,0,age);

    //日期标签
    QString date = "日期："+Date;
    painter.drawText(610*rate, 100*rate ,300*rate,14*rate,0,date);


    //图示
    QString attogram = "图示：";
    painter.drawText(40*rate, 160*rate ,40*rate,14*rate,0,attogram);

    //注
    QString str = "*注：上面的每一天都是从第一天的12:00到第二天的12:00。";
    painter.drawText(80*rate, (Y_tablestart+D_zhu)*rate ,600*rate,14*rate,0,str);

    //每日统计
    QString daily = "每日统计:";
    painter.drawText(40*rate, (Y_tablestart-30)*rate ,60*rate,14*rate,0,daily);

    for(int i=0;i<choosenum;i++)
    {
        for(int j=0;j<columnCount;j++)
        {
            painter.drawText((60 + j * columnWidth + columnWidth - 90)*rate , (Y_tablestart + (i+2)*rowHeight - 28)*rate,100*rate,30*rate,1,strlist[j+columnCount*i]);
        }
    }
    //指导意见：
    //注
    QString advice = "指导意见:";
    painter.drawText(60*rate, (Y_tablestart+D_zhu+D_advice)*rate ,600*rate,14*rate,0,advice);
    int nnn=0;
    for (int i = 1; i <= strlist_advice.length(); i++)
    {
        QString tem=strlist_advice[i-1];
        nnn+=tem.count('\n');
        painter.drawText((80)*rate , (Y_tablestart+D_zhu+D_advice+(i+nnn)*20)*rate,800*rate,60*rate,1,tem);
    }
    painter.end();


}


void MyPrintPreViewDialog::printPreviewSlot(QPrinter *printer)
{
    //4，创建画家
    double rate =6.0;
    QPainter painter(printer);//继承打印
    //5.打印内容
    //QRect rect = this->geometry();
    QPixmap pixmap;
    pixmap.load("./TEMPIC/"+QString::number(choosenum)+".png");

    painter.drawPixmap(50*rate,185*rate,750*rate,(25+choosenum*60)*rate,pixmap);

    //画表格
    QPen pen = painter.pen();
    pen.setWidth(1);
    painter.setPen(pen);
    QFont font("宋体",fontsize);
    painter.setFont(font);
    //画行线
    for (int i = 0; i <= rowCount; i++)
    {
        painter.drawLine(60*rate, (Y_tablestart + rowHeight*i)*rate, (60 + columnWidth * columnCount)*rate, (Y_tablestart + rowHeight * i)*rate);
    }

    //画列线
    for (int i = 0; i <= columnCount; i++)
    {
        painter.drawLine((60 + i * columnWidth)*rate, Y_tablestart*rate, (60 + i * columnWidth)*rate, (Y_tablestart + rowCount * rowHeight)*rate);
    }
    QStringList headStrList;//("日    期", "就寝时间","入睡时间", "起床时间", "卧床时长", "睡眠时长", "睡眠效率");
    headStrList.append("日    期");
    headStrList.append("白天睡眠次数");
    headStrList.append("白天深睡时长");
    headStrList.append("白天浅睡时长");
    headStrList.append("夜晚睡眠次数");
    headStrList.append("夜晚深睡时长");
    headStrList.append("夜晚浅睡时长");
    //表头
    for (int i = 0; i < columnCount; i++)
    {
        painter.drawText((60 + i * columnWidth + columnWidth - 90)*rate , (Y_tablestart + rowHeight - 28)*rate,100*rate,30*rate,1,headStrList[i]);
    }
    // 标题
    painter.setFont(QFont("宋体",24,40));
    QString title = "睡眠报告";
    painter.drawText(350*rate, 20*rate,300*rate,60*rate,0,title);
    painter.setFont(QFont("宋体",10));

    //编号标签
    QString id = "编号："+ID;
    painter.drawText(10*rate, 100*rate ,200*rate,14*rate,0,id);

    //姓名标签
    QString name = "姓名："+Name;
    painter.drawText(140*rate, 100*rate ,200*rate,14*rate,0,name);

    //性别标签
    QString gender = "性别："+Gander;
    painter.drawText(250*rate, 100*rate ,200*rate,14*rate,0,gender);

    //出生日期标签
    QString bornData = "出生日期："+Brithday;
    painter.drawText(320*rate, 100*rate ,400*rate,14*rate,0,bornData);

    //年龄标签
    QString age = "年龄："+Age;
    painter.drawText(505*rate, 100*rate ,300*rate,14*rate,0,age);

    //日期标签
    QString date = "日期："+Date;
    painter.drawText(610*rate, 100*rate ,300*rate,14*rate,0,date);


    //图示
    QString attogram = "图示：";
    painter.drawText(40*rate, 160*rate ,40*rate,14*rate,0,attogram);

    //注
    QString str = "*注：上面的每一天都是从第一天的12:00到第二天的12:00。";
    painter.drawText(80*rate, (Y_tablestart+D_zhu)*rate ,600*rate,14*rate,0,str);

    //每日统计
    QString daily = "每日统计:";
    painter.drawText(40*rate, (Y_tablestart-30)*rate ,60*rate,14*rate,0,daily);


    for(int i=0;i<choosenum;i++)
    {
        for(int j=0;j<columnCount;j++)
        {
            painter.drawText((60 + j * columnWidth + columnWidth - 90)*rate , (Y_tablestart + (i+2)*rowHeight - 28)*rate,100*rate,30*rate,1,strlist[j+columnCount*i]);
        }
    }

    //指导意见：
    //注
    QString advice = "指导意见:";
    painter.drawText(60*rate, (Y_tablestart+D_zhu+D_advice)*rate ,600*rate,14*rate,0,advice);
    int nnn=0;
    for (int i = 1; i <= strlist_advice.length(); i++)
    {
        QString tem=strlist_advice[i-1];
        nnn+=tem.count('\n');
        painter.drawText((80)*rate , (Y_tablestart+D_zhu+D_advice+(i+nnn)*20)*rate,800*rate,60*rate,1,tem);
    }


    painter.end();
}
