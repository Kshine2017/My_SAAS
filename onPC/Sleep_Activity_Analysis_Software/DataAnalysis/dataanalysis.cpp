#include "dataanalysis.h"
#include "DataContainer/datacontainer.h"
#include <QDebug>
#include "opration/restoredevicedefaultdata.h"
#include "opration/obtaindevicemeasuredata.h"
#include "opration/openfilegetdata.h"
#include "opration/readfiledata.h"
#include "opration/compressanddealdata.h"
#include "opration/qwaitdialog.h"
#include "opration/dataanalyze.h"
#include "opration/ksprogressbar.h"
#include "patient/patientinfo.h"
#include "patient/switchpatient.h"
#include "patient/currentpatient.h"
#include "graphplot/enlargeplot.h"
#define LABELDATEMAXWIDTH 100
DataAnalysis::DataAnalysis(QWidget *parent, int WWW, int HHH) : QWidget(parent)
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
    //setAttribute(Qt::WA_DeleteOnClose);
    this->setAutoFillBackground(true);
    qDebug()<<"页面：数据分析  尺寸："<<width()<<height();
    init_PageTopInfo();
    init_PageSecInfo();
    init_PageTitleInfo();
    init_PagePlotInfo();
    //所有页面布局
    init_PageAllLayout();
    //标记条初始化
    init_MarkFrame();
    qDebug()<<"数据分析页面：测试连接到数据容器"<<DataContainer::getInstance();

    /*这个函数是不会执行的******/
    init_allStatus();
}

DataAnalysis::~DataAnalysis()
{
    qDebug()<<"析构--数据分析页面";
}

void DataAnalysis::slot_ShowMesageFromThread(QString text)
{
    QMessageBox::information(this,"来自线程的提示",text,QMessageBox::Ok);
}

void DataAnalysis::slot_SowWorkProgress(qint64 progress)
{
    Q_UNUSED(progress)
}

void DataAnalysis::slot_uploaddata()
{
    qDebug()<<"数据分析页面---数据上载";
    //检查是否插入设备（设备路径是否已存在）
    DataContainer* dc=DataContainer::getInstance();
    QString patientname =dc->PatientName;
    if(patientname=="")
    {
        QMessageBox::warning(this,"重要提示：",QStringLiteral("未指定当前用户，请指定"),QMessageBox::Yes);
        return;
    }
    if(dc->DevicePath=="")    //若不存在，检查各个磁盘的名字（一方以防设备提前插入）
    {
        QFileInfoList list =  QDir::drives();  //获取当前系统的盘符
        bool bUsb = false;
        QStringList RemovableDeviceList;
        //QString deviceDisk;

        for(int i=0;i<list.count();i++)
        {
            UINT ret = GetDriveType((WCHAR *) list[i].filePath().utf16());
            if(ret == DRIVE_REMOVABLE)
            {
                //deviceDisk=list[i].filePath();
                RemovableDeviceList.append(list[i].filePath());
                //qDebug()<<deviceDisk<<"--->可移动盘";//"F:/"
                bUsb = true;
            }
        }
        if(!bUsb)
        {
            QMessageBox::warning(this,"重要提示：",QStringLiteral("未检测到设备！\n(1)请检查是否插入设备！\n(2)请检查是否开启设备电源!\n(3)请检查连接线是否插牢！\n(4)请在必要时联系技术人员！"),QMessageBox::Yes);
            return;
        }

        for(int j=0;j<RemovableDeviceList.length();j++)
        {
            QString lpRootPathName = RemovableDeviceList[j];//"C:\\" 此处使用"F:/" 格式也可以
            LPTSTR lpVolumeNameBuffer=new TCHAR[12];//磁盘卷标
            DWORD nVolumeNameSize=12;// 卷标的字符串长度
            DWORD VolumeSerialNumber;//硬盘序列号
            DWORD MaximumComponentLength;// 最大的文件长度
            LPTSTR lpFileSystemNameBuffer=new TCHAR[10];// 存储所在盘符的分区类型的长指针变量
            DWORD nFileSystemNameSize=10;// 分区类型的长指针变量所指向的字符串长度
            DWORD FileSystemFlags;// 文件系统的一此标志
            GetVolumeInformation(
                        (LPTSTR)lpRootPathName.utf16(),
                        lpVolumeNameBuffer,
                        nVolumeNameSize,
                        &VolumeSerialNumber,
                        &MaximumComponentLength,
                        &FileSystemFlags,
                        lpFileSystemNameBuffer,
                        nFileSystemNameSize
                        );
            QString diskname=QString::fromWCharArray(lpVolumeNameBuffer);
            qDebug() <<"--->可移动盘"<<lpRootPathName<< diskname;
            if(diskname=="BB-CARE")//体动机器磁盘名称
            {
                DataContainer::getInstance()->DevicePath=  lpRootPathName[0] + ":\\Active_Data";
                break;
            }
        }
        if(DataContainer::getInstance()->DevicePath=="")
        {
            QMessageBox::warning(this,"重要提示：",QStringLiteral("未检测到设备！\n(1)请检查是否插入设备！\n(2)请检查是否开启设备电源!\n(3)请检查连接线是否插牢！\n(4)请在必要时联系技术人员！"),QMessageBox::Yes);
            return;
        }
    }
    //得到设备路径
    qDebug()<<"设备地址："<<DataContainer::getInstance()->DevicePath;
    //放入线程读取数据
    qDebug()<<"开始---复制文件";
    ObtainDeviceMeasureData* ODMD = new ObtainDeviceMeasureData(patientname);//因为放到线程去执行，不需要继承this
    connect(this,SIGNAL(signal_copyDeviceData()),ODMD,SLOT(startCOPYFileFromDevice()));//启动
    connect(ODMD,SIGNAL(work_message(QString)),this,SLOT(slot_ShowMesageFromThread(QString)));
    ODMD->SourceDir=DataContainer::getInstance()->DevicePath;
    //--开拷贝函数线程--------------
    QThread* thread = new QThread(this);
    connect(ODMD,SIGNAL(work_over()),thread,SLOT(quit()));//退出线程
    QWaitDialog* wait = new QWaitDialog(this,"正在复制设备文件");
    connect(ODMD,SIGNAL(work_Progress(qint64)),wait,SLOT(slot_showSizeProgress(qint64)));//显示数据
    connect(ODMD,SIGNAL(work_over()),wait,SLOT(close()));//关闭
    connect(ODMD,SIGNAL(work_TotalSize(qint64)),wait,SLOT(slot_setTotalSize(qint64)));
    ODMD->moveToThread(thread);
    thread->start();//启动线程
    emit signal_copyDeviceData();//通知线程开始工作
}

void DataAnalysis::slot_openchoose()
{
    //打开勾选界面，勾选最多6天文件夹
    qDebug()<<"开发人员选项---全部操作（打开页面--选择--打开文件--压缩数据--分析数据--显示波形）";
    //定义一个工作流控制类，附件界面显示
    DataContainer* dc = DataContainer::getInstance();
    QString patientname = dc->PatientName;
    if(patientname=="")
    {
        QMessageBox::warning(this,"重要提示：",QStringLiteral("未指定当前用户，请指定"),QMessageBox::Yes);
        return;
    }
    OpenFileGetData* OFGD=new OpenFileGetData(patientname,this);
    connect(OFGD,SIGNAL(signal_strlistCheckFiles(QStringList)),this,SLOT(slot_Start_OpenFile(QStringList)));
}
void DataAnalysis::slot_Start_OpenFile(const QStringList &strlist)
{
    qDebug()<<"新程序--打开文件--分析数据--显示图形";
    int num=strlist.length();
    DataContainer* dc= DataContainer::getInstance();
    dc->ChooseNUM = num;
    for(int i=0;i<6;i++)
    {
        if(i==0){if(i<num)dc->day1=strlist[i];else dc->day1="Day1";}
        if(i==1){if(i<num)dc->day2=strlist[i];else dc->day2="Day2";}
        if(i==2){if(i<num)dc->day3=strlist[i];else dc->day3="Day3";}
        if(i==3){if(i<num)dc->day4=strlist[i];else dc->day4="Day4";}
        if(i==4){if(i<num)dc->day5=strlist[i];else dc->day5="Day5";}
        if(i==5){if(i<num)dc->day6=strlist[i];else dc->day6="Day6";}
    }
    if(num==0)return;
    //工作1--------读取数据-----------------------------------------------------------
    qDebug()<<"工作1******************************";
    ReadFileData* RFD =new ReadFileData();                                                          //读取CSV中的数据到容器中。每天的数据位86400
    connect(this,SIGNAL(signal_ReadFileData(QStringList)),RFD,SLOT(startReadFileData(QStringList)));//启动信号
    connect(RFD,SIGNAL(work_message(QString)),this,SLOT(slot_ShowMesageFromThread(QString)));       //信息传出
    QThread* thread1 = new QThread(this);                                                           //线程1
    connect(RFD,SIGNAL(work_over()),thread1,SLOT(quit()));                                          //完成工作
    RFD->moveToThread(thread1);                                                                     //工作甩到线程1中
    thread1->start();                                                                               //启动线程
    //工作2---------压缩数据----------------------------------------------------------此时 各个数组还没有读取导数据//但传递的是地址，所以可以在上一个工作结束前，先获取数据地址
    qDebug()<<"工作2******************************"<<"num ="<<num;
    CompressAndDealData* CADD=new CompressAndDealData();                                            //压缩原始数据，绘图用的数据2880（最小时间值为30秒），分析用的数据144（最小时间值10分钟）
    connect(RFD,SIGNAL(work_over()),CADD,SLOT(start()));                                            //上一个工作结束，立即开始下一个工作
    QThread* thread2 =new QThread(this);                                                            //线程2
    connect(CADD,SIGNAL(work_over()),thread2,SLOT(quit()));                                         //完成工作
    CADD->moveToThread(thread2);                                                                    //工作丢入线程2中
    thread2->start();                                                                               //启动线程
    //工作3-----------分析数据----------------------------------------------------
    qDebug()<<"工作3******************************"<<"num ="<<num;
    DataAnalyze* DA = new DataAnalyze();                                                            //数据分析，得到标记条的数据
    dc->MarkBarWidth=Markbar1->width();                                                             //别的页面需要用到这个长度值
    connect(CADD,SIGNAL(work_over()),DA,SLOT(startAnalyze()));                                      //上一份工作结束，立即开始下一份工作
    QThread* thread3 =new QThread(this);                                                            //线程3
    connect(DA,SIGNAL(work_over()),thread3,SLOT(quit()));                                           //完成工作
    DA->moveToThread(thread3);                                                                      //工作踢进线程3中
    thread3->start();                                                                               //启动线程
    //工作4--------------显示数据-------------------------------------------------
    qDebug()<<"工作4******************************";
                                                                                                    //在主线程中显示绘图
    connect(DA,SIGNAL(work_over()),this,SLOT(slot_Develop_DrawPicture()));                          //上一份工作结束，立即开始下一份工作
    //------------------------------------------------------------------
    //统一进度展示（进度条）
    KSProgressBar* KSPB =new KSProgressBar(this);
    connect(RFD,SIGNAL(work_TotalNumber(int)),KSPB,SLOT(slot_setToTalNumber(int)));                 //读取数据的工作，传入总的文件个数
    connect(RFD,SIGNAL(work_Progress_int(int)),KSPB,SLOT(slot_showNumProgress(int)));               //传入1个文件，并累计，得出进度百分比
    connect(RFD,SIGNAL(work_over()),KSPB,SLOT(slot_addWorkFlag()));//工作标志自加  1-->2              //分段进度条进入下一阶段

    connect(CADD,SIGNAL(work_TotalNumber(int)),KSPB,SLOT(slot_setToTalNumber(int)));                //压缩数据的工作，传入总的文件个数
    connect(CADD,SIGNAL(work_Progress_int(int)),KSPB,SLOT(slot_showNumProgress(int)));              //压缩1个文件，并累计，得出进度百分比
    connect(CADD,SIGNAL(work_over()),KSPB,SLOT(slot_addWorkFlag()));//工作标志自加 2-->3              //分段进度条进入下一阶段

    connect(DA,SIGNAL(work_TotalNumber(int)),KSPB,SLOT(slot_setToTalNumber(int)));                  //分析数据的工作，传入总的文件个数
    connect(DA,SIGNAL(work_Progress_int(int)),KSPB,SLOT(slot_showNumProgress(int)));                //分析1个文件，并累计，得出进度百分比
    connect(DA,SIGNAL(work_over()),KSPB,SLOT(slot_addWorkFlag()));//工作标志自加3-->4                 //分段进度条进入下一阶段

    connect(this,SIGNAL(signal_HaveDrawnpic()),KSPB,SLOT(slot_addWorkFlag()));                      //主线程绘图工作，完成后，进入下一阶段（实际已完成全部）

    //---------------------------------------------------------------
    emit signal_ReadFileData(strlist);                                                              //通知线程开始工作(读取)
    dc->KFlag_PDF=0;
}

//主线程显示绘图
void DataAnalysis::slot_Develop_DrawPicture()
{
    DataContainer* dc=DataContainer::getInstance();
    emit this->work_TotalNumber(dc->ChooseNUM);                                                     //分段进度条第四阶段进度总天数（以一天为计加数）
    if(dc->activeList2880_Day1.count() == 0 || dc->lightList2880_Day1.count()==0 || dc->voiceList2880_Day1.count()==0)
    {
        qDebug()<<"2880"<<dc->activeList2880_Day1.count()<<dc->lightList2880_Day1.count()<<dc->voiceList2880_Day1.count();
        QMessageBox::information(this,"生成图形异常","请确认是否打开文件和压缩数据！",QMessageBox::Ok);
        return;
    }
    //标签显示具体的日期
    lb_Day1->setText(dc->day1);
    lb_Day2->setText(dc->day2);
    lb_Day3->setText(dc->day3);
    lb_Day4->setText(dc->day4);
    lb_Day5->setText(dc->day5);
    lb_Day6->setText(dc->day6);
    //-------------------------------------
    //清除已绘图的显示
    if(dc->ChooseNUM==1){myplot2->clearDataKshine();myplot3->clearDataKshine();myplot4->clearDataKshine(); myplot5->clearDataKshine();myplot6->clearDataKshine();}
    if(dc->ChooseNUM==2){myplot3->clearDataKshine();myplot4->clearDataKshine(); myplot5->clearDataKshine();myplot6->clearDataKshine();}
    if(dc->ChooseNUM==3){myplot4->clearDataKshine();myplot5->clearDataKshine();myplot6->clearDataKshine();}
    if(dc->ChooseNUM==4){myplot5->clearDataKshine();myplot6->clearDataKshine();}
    if(dc->ChooseNUM==5){myplot6->clearDataKshine();}
    if(dc->ChooseNUM==6){}
    //清除已绘图显示
    if(dc->ChooseNUM==1){Markbar2->clearData();Markbar3->clearData();Markbar4->clearData(); Markbar5->clearData();Markbar6->clearData();}
    if(dc->ChooseNUM==2){Markbar3->clearData();Markbar4->clearData(); Markbar5->clearData();Markbar6->clearData();}
    if(dc->ChooseNUM==3){Markbar4->clearData();Markbar5->clearData();Markbar6->clearData();}
    if(dc->ChooseNUM==4){Markbar5->clearData();Markbar6->clearData();}
    if(dc->ChooseNUM==5){Markbar6->clearData();}
    if(dc->ChooseNUM==6){}
    //开始绘图：显示波形图和标记
    //-------------------------------------
    myplot1->temPIC->yAxis->setRange(0,500);
    myplot2->temPIC->yAxis->setRange(0,500);
    myplot3->temPIC->yAxis->setRange(0,500);
    myplot4->temPIC->yAxis->setRange(0,500);
    myplot5->temPIC->yAxis->setRange(0,500);
    myplot6->temPIC->yAxis->setRange(0,500);
    //光强，体动，声音
    if(dc->ChooseNUM>=1){myplot1->display_Act(dc->activeList2880_Day1,0);myplot1->display_Light(dc->lightList2880_Day1,1);myplot1->display_voice(dc->voiceList2880_Day1,2);Markbar1->autoMaticMark(dc->DUAN1);emit work_Progress_int(1);}
    if(dc->ChooseNUM>=2){myplot2->display_Act(dc->activeList2880_Day2,0);myplot2->display_Light(dc->lightList2880_Day2,1);myplot2->display_voice(dc->voiceList2880_Day2,2);Markbar2->autoMaticMark(dc->DUAN2);emit work_Progress_int(1);}
    if(dc->ChooseNUM>=3){myplot3->display_Act(dc->activeList2880_Day3,0);myplot3->display_Light(dc->lightList2880_Day3,1);myplot3->display_voice(dc->voiceList2880_Day3,2);Markbar3->autoMaticMark(dc->DUAN3);emit work_Progress_int(1);}
    if(dc->ChooseNUM>=4){myplot4->display_Act(dc->activeList2880_Day4,0);myplot4->display_Light(dc->lightList2880_Day4,1);myplot4->display_voice(dc->voiceList2880_Day4,2);Markbar4->autoMaticMark(dc->DUAN4);emit work_Progress_int(1);}
    if(dc->ChooseNUM>=5){myplot5->display_Act(dc->activeList2880_Day5,0);myplot5->display_Light(dc->lightList2880_Day5,1);myplot5->display_voice(dc->voiceList2880_Day5,2);Markbar5->autoMaticMark(dc->DUAN5);emit work_Progress_int(1);}
    if(dc->ChooseNUM>=6){myplot6->display_Act(dc->activeList2880_Day6,0);myplot6->display_Light(dc->lightList2880_Day6,1);myplot6->display_voice(dc->voiceList2880_Day6,2);Markbar6->autoMaticMark(dc->DUAN6);emit work_Progress_int(1);}
    //勾选框默认全部勾选-------------------------------------
    ckb_all->setCheckState(Qt::Checked);
    ckb_active->setCheckState(Qt::Checked);
    ckb_light->setCheckState(Qt::Checked);
    ckb_voice->setCheckState(Qt::Checked);
    qDebug()<<"Day1的图层数---------->"<< myplot1->temPIC->graphCount();//用于测试bug
    emit signal_HaveDrawnpic();                                        //主线程提示已完成全部
    flag_clicked_disPlay=true;                                         //置位双击放大标志
}
//保存自动分析和手动标记的结果
void DataAnalysis::slot_saveresult()
{
    DataContainer* dc =DataContainer::getInstance();
    int choosenum = dc->ChooseNUM;
    //保存截图
    QString dir_str = "./TEMPIC";
    // 检查目录是否存在，若不存在则新建
    QDir dir;
    if (!dir.exists(dir_str))
    {
        bool res = dir.mkpath(dir_str);
        qDebug() << "新建目录是否成功" << res;
    }
    //保存生成的报告所需要的图片元素。
    QPixmap pixmap = this->grab(QRect(0,lb_Date->y(),thisWidth,40+choosenum*myplot1->height()));
    pixmap.save("./TEMPIC/"+QString::number(choosenum)+".png","png");
    //点击确定，将手动标记结果保存到容器

    if(choosenum>=1)dc->DUAN1 = Markbar1->DUAN;
    if(choosenum>=2)dc->DUAN2 = Markbar2->DUAN;
    if(choosenum>=3)dc->DUAN3 = Markbar3->DUAN;
    if(choosenum>=4)dc->DUAN4 = Markbar4->DUAN;
    if(choosenum>=5)dc->DUAN5 = Markbar5->DUAN;
    if(choosenum>=6)dc->DUAN6 = Markbar6->DUAN;
    //-------生成睡眠段--------
    //整理动态段，冒泡排序（为应对标记条每次都多保存一段数据的情况，需要排序之后，手动删除（0,0））
    QVector<struct_Duan>* temDUAN;
    for(int pi=0;pi<choosenum;pi++)
    {
        if(pi==0){temDUAN = &dc->DUAN1;}
        else if(pi==1){temDUAN = &dc->DUAN2;}
        else if(pi==2){temDUAN = &dc->DUAN3;}
        else if(pi==3){temDUAN = &dc->DUAN4;}
        else if(pi==4){temDUAN = &dc->DUAN5;}
        else if(pi==5){temDUAN = &dc->DUAN6;}
        int num = temDUAN->length()-1;
        //将（0,0）放到duan尾，不进行冒泡（也删不掉）
        for(int k=0;k<num;k++)
        {
            if((*temDUAN)[k].startX==0&&(*temDUAN)[k].endX==0)//满足交换条件
            {
                int t = (*temDUAN)[k].startX;
                (*temDUAN)[k].startX=(*temDUAN)[num].startX;
                (*temDUAN)[num].startX=t;
                t = (*temDUAN)[k].endX;
                (*temDUAN)[k].endX=(*temDUAN)[num].endX;
                (*temDUAN)[num].endX=t;
            }
        }
        for(int i=0;i<num-1;i++)
        {
            for(int j=0;j<num-1-i;j++)
            {
                if((*temDUAN)[j].startX>(*temDUAN)[j+1].startX)
                {
                    int t = (*temDUAN)[j].startX;
                    (*temDUAN)[j].startX=(*temDUAN)[j+1].startX;
                    (*temDUAN)[j+1].startX=t;
                    t = (*temDUAN)[j].endX;
                    (*temDUAN)[j].endX=(*temDUAN)[j+1].endX;
                    (*temDUAN)[j+1].endX=t;
                }
            }
        }
//        //测试
//        for(int j=0;j<temDUAN->length();j++)
//            qDebug()<<"DUAN排序之后："<<(*temDUAN)[j].startX<<(*temDUAN)[j].endX;
    }
    //依序生成SUAN（保存睡眠的标记数据）
    QVector<struct_Duan>* temSUAN;
    for(int pi=0;pi<choosenum;pi++)
    {
        if(pi==0){temDUAN = &dc->DUAN1;temSUAN=&dc->SUAN1;}
        else if(pi==1){temDUAN = &dc->DUAN2;temSUAN=&dc->SUAN2;}
        else if(pi==2){temDUAN = &dc->DUAN3;temSUAN=&dc->SUAN3;}
        else if(pi==3){temDUAN = &dc->DUAN4;temSUAN=&dc->SUAN4;}
        else if(pi==4){temDUAN = &dc->DUAN5;temSUAN=&dc->SUAN5;}
        else if(pi==5){temDUAN = &dc->DUAN6;temSUAN=&dc->SUAN6;}
        temSUAN->clear();
        int num = temDUAN->length()-1;
        int mw=dc->MarkBarWidth;
        if(num==0)
        {
            struct_Duan temdata;
            temdata.startX=DUAN_BORDER;
            temdata.endX= mw-DUAN_BORDER;
            temSUAN->append(temdata);
        }
        else
        for(int i=0;i<num;i++)
        {
            //向左找SUAN---------------------------------------------
            if(i==0&&(*temDUAN)[i].startX>DUAN_BORDER)
            {
                struct_Duan temdata;
                temdata.startX=DUAN_BORDER;
                temdata.endX= (*temDUAN)[i].startX;
                temSUAN->append(temdata);
            }
            //向右找SUAN---------------------------------------------
            if(i==num-1)
            {
                if((*temDUAN)[i].endX < mw-DUAN_BORDER)
                {
                    struct_Duan temdata;
                    temdata.startX=(*temDUAN)[i].endX;
                    temdata.endX= mw-DUAN_BORDER;
                    temSUAN->append(temdata);
                }
            }
            else
            {
                struct_Duan temdata;
                temdata.startX=(*temDUAN)[i].endX;
                temdata.endX=(*temDUAN)[i+1].startX;
                temSUAN->append(temdata);
            }
        }
//        //测试
//        for(int i=0;i<temSUAN->length();i++)
//        {
//            qDebug()<<"生成SUAN:"<<(*temSUAN)[i].startX<<(*temSUAN)[i].endX;
//        }
    }
    QMessageBox::information(this,"成功","已保存辅助标记信息！",QMessageBox::Ok);
}

void DataAnalysis::slot_currentUser()
{
    connect(new CurrentPatient(this),SIGNAL(signal_Sure()),this,SLOT(getNewUser()));//创建小窗口：当前用户信息
}

void DataAnalysis::slot_createUsr()
{
    connect(new PatientInfo(this),SIGNAL(signal_Sure()),this,SLOT(getNewUser()));   //创建小窗口：新建用户信息
}

void DataAnalysis::slot_switchUsr()                                                 //创建小窗口，切换用户信息
{
    connect(new SwitchPatient(this),SIGNAL(signal_Sure()),this,SLOT(getNewUser()));

}

void DataAnalysis::getNewUser()
{
    QString patientname =DataContainer::getInstance()->PatientName;
    btn_currentUser->setText(patientname);
}

void DataAnalysis::slot_checkboxACT(int)
{
    DataContainer* dc =DataContainer::getInstance();
    if(ckb_active->checkState()==Qt::Unchecked)
    {
        ckb_all->setChecked(0);
        if(dc->ChooseNUM>=1){myplot1->temPIC->graph(0)->setVisible(false);myplot1->temPIC->replot();}
        if(dc->ChooseNUM>=2){myplot2->temPIC->graph(0)->setVisible(false);myplot2->temPIC->replot();}
        if(dc->ChooseNUM>=3){myplot3->temPIC->graph(0)->setVisible(false);myplot3->temPIC->replot();}
        if(dc->ChooseNUM>=4){myplot4->temPIC->graph(0)->setVisible(false);myplot4->temPIC->replot();}
        if(dc->ChooseNUM>=5){myplot5->temPIC->graph(0)->setVisible(false);myplot5->temPIC->replot();}
        if(dc->ChooseNUM>=6){myplot6->temPIC->graph(0)->setVisible(false);myplot6->temPIC->replot();}
    }
    else
    {
        if(dc->ChooseNUM>=1){myplot1->temPIC->graph(0)->setVisible(true);myplot1->temPIC->replot();}
        if(dc->ChooseNUM>=2){myplot2->temPIC->graph(0)->setVisible(true);myplot2->temPIC->replot();}
        if(dc->ChooseNUM>=3){myplot3->temPIC->graph(0)->setVisible(true);myplot3->temPIC->replot();}
        if(dc->ChooseNUM>=4){myplot4->temPIC->graph(0)->setVisible(true);myplot4->temPIC->replot();}
        if(dc->ChooseNUM>=5){myplot5->temPIC->graph(0)->setVisible(true);myplot5->temPIC->replot();}
        if(dc->ChooseNUM>=6){myplot6->temPIC->graph(0)->setVisible(true);myplot6->temPIC->replot();}
    }
}

void DataAnalysis::slot_checkboxLGT(int)
{
    DataContainer* dc =DataContainer::getInstance();
    if(ckb_light->checkState()==Qt::Unchecked)
    {
        ckb_all->setChecked(0);
        if(dc->ChooseNUM>=1){myplot1->temPIC->graph(1)->setVisible(false);myplot1->temPIC->replot();}
        if(dc->ChooseNUM>=2){myplot2->temPIC->graph(1)->setVisible(false);myplot2->temPIC->replot();}
        if(dc->ChooseNUM>=3){myplot3->temPIC->graph(1)->setVisible(false);myplot3->temPIC->replot();}
        if(dc->ChooseNUM>=4){myplot4->temPIC->graph(1)->setVisible(false);myplot4->temPIC->replot();}
        if(dc->ChooseNUM>=5){myplot5->temPIC->graph(1)->setVisible(false);myplot5->temPIC->replot();}
        if(dc->ChooseNUM>=6){myplot6->temPIC->graph(1)->setVisible(false);myplot6->temPIC->replot();}
    }
    else
    {
        if(dc->ChooseNUM>=1){myplot1->temPIC->graph(1)->setVisible(true);myplot1->temPIC->replot();}
        if(dc->ChooseNUM>=2){myplot2->temPIC->graph(1)->setVisible(true);myplot2->temPIC->replot();}
        if(dc->ChooseNUM>=3){myplot3->temPIC->graph(1)->setVisible(true);myplot3->temPIC->replot();}
        if(dc->ChooseNUM>=4){myplot4->temPIC->graph(1)->setVisible(true);myplot4->temPIC->replot();}
        if(dc->ChooseNUM>=5){myplot5->temPIC->graph(1)->setVisible(true);myplot5->temPIC->replot();}
        if(dc->ChooseNUM>=6){myplot6->temPIC->graph(1)->setVisible(true);myplot6->temPIC->replot();}
    }
}

void DataAnalysis::slot_checkboxVOC(int)
{
    DataContainer* dc =DataContainer::getInstance();
    if(ckb_voice->checkState()==Qt::Unchecked)
    {
        ckb_all->setChecked(0);
        if(dc->ChooseNUM>=1){myplot1->temPIC->graph(2)->setVisible(false);myplot1->temPIC->replot();}
        if(dc->ChooseNUM>=2){myplot2->temPIC->graph(2)->setVisible(false);myplot2->temPIC->replot();}
        if(dc->ChooseNUM>=3){myplot3->temPIC->graph(2)->setVisible(false);myplot3->temPIC->replot();}
        if(dc->ChooseNUM>=4){myplot4->temPIC->graph(2)->setVisible(false);myplot4->temPIC->replot();}
        if(dc->ChooseNUM>=5){myplot5->temPIC->graph(2)->setVisible(false);myplot5->temPIC->replot();}
        if(dc->ChooseNUM>=6){myplot6->temPIC->graph(2)->setVisible(false);myplot6->temPIC->replot();}
    }
    else
    {
        if(dc->ChooseNUM>=1){myplot1->temPIC->graph(2)->setVisible(true);myplot1->temPIC->replot();}
        if(dc->ChooseNUM>=2){myplot2->temPIC->graph(2)->setVisible(true);myplot2->temPIC->replot();}
        if(dc->ChooseNUM>=3){myplot3->temPIC->graph(2)->setVisible(true);myplot3->temPIC->replot();}
        if(dc->ChooseNUM>=4){myplot4->temPIC->graph(2)->setVisible(true);myplot4->temPIC->replot();}
        if(dc->ChooseNUM>=5){myplot5->temPIC->graph(2)->setVisible(true);myplot5->temPIC->replot();}
        if(dc->ChooseNUM>=6){myplot6->temPIC->graph(2)->setVisible(true);myplot6->temPIC->replot();}
    }
}

void DataAnalysis::slot_checkboxALL(int)
{
    DataContainer* dc =DataContainer::getInstance();
    if(ckb_all->checkState()==Qt::Checked)
    {
        ckb_active->setChecked(true);
        ckb_light->setChecked(true);
        ckb_voice->setChecked(true);

        if(dc->ChooseNUM>=1){myplot1->temPIC->graph(0)->setVisible(1);myplot1->temPIC->graph(1)->setVisible(1);myplot1->temPIC->graph(2)->setVisible(1);myplot1->temPIC->replot();}
        if(dc->ChooseNUM>=2){myplot2->temPIC->graph(0)->setVisible(1);myplot2->temPIC->graph(1)->setVisible(1);myplot2->temPIC->graph(2)->setVisible(1);myplot2->temPIC->replot();}
        if(dc->ChooseNUM>=3){myplot3->temPIC->graph(0)->setVisible(1);myplot3->temPIC->graph(1)->setVisible(1);myplot3->temPIC->graph(2)->setVisible(1);myplot3->temPIC->replot();}
        if(dc->ChooseNUM>=4){myplot4->temPIC->graph(0)->setVisible(1);myplot4->temPIC->graph(1)->setVisible(1);myplot4->temPIC->graph(2)->setVisible(1);myplot4->temPIC->replot();}
        if(dc->ChooseNUM>=5){myplot5->temPIC->graph(0)->setVisible(1);myplot5->temPIC->graph(1)->setVisible(1);myplot5->temPIC->graph(2)->setVisible(1);myplot5->temPIC->replot();}
        if(dc->ChooseNUM>=6){myplot6->temPIC->graph(0)->setVisible(1);myplot6->temPIC->graph(1)->setVisible(1);myplot6->temPIC->graph(2)->setVisible(1);myplot6->temPIC->replot();}
    }

}

void DataAnalysis::slot_doubleClick1(QMouseEvent *)
{
    DataContainer* dc =DataContainer::getInstance();
    if(!flag_clicked_disPlay)
    {
        return;
    }
    bool Va=false,Vl=false,Vv=false;
    if(ckb_active->checkState()==Qt::Checked)   Va=true;
    if(ckb_light->checkState()==Qt::Checked)    Vl=true;
    if(ckb_voice->checkState()==Qt::Checked)    Vv=true;
    EnlargePlot * enlagePlot = new EnlargePlot(this);
    enlagePlot->disPlay_data(dc->activeList2880_Day1,0,Va);
    enlagePlot->disPlay_data(dc->lightList2880_Day1,1,Vl);
    enlagePlot->disPlay_data(dc->voiceList2880_Day1,2,Vv);
    enlagePlot->show();
}

void DataAnalysis::slot_doubleClick2(QMouseEvent *)
{
    DataContainer* dc =DataContainer::getInstance();
    if(!flag_clicked_disPlay)
    {
        return;
    }
    bool Va=false,Vl=false,Vv=false;
    if(ckb_active->checkState()==Qt::Checked)   Va=true;
    if(ckb_light->checkState()==Qt::Checked)    Vl=true;
    if(ckb_voice->checkState()==Qt::Checked)    Vv=true;
    EnlargePlot * enlagePlot = new EnlargePlot(this);
    enlagePlot->disPlay_data(dc->activeList2880_Day2,0,Va);
    enlagePlot->disPlay_data(dc->lightList2880_Day2,1,Vl);
    enlagePlot->disPlay_data(dc->voiceList2880_Day2,2,Vv);
    enlagePlot->show();
}

void DataAnalysis::slot_doubleClick3(QMouseEvent *)
{
    DataContainer* dc =DataContainer::getInstance();
    if(!flag_clicked_disPlay)
    {
        return;
    }
    bool Va=false,Vl=false,Vv=false;
    if(ckb_active->checkState()==Qt::Checked)   Va=true;
    if(ckb_light->checkState()==Qt::Checked)    Vl=true;
    if(ckb_voice->checkState()==Qt::Checked)    Vv=true;
    EnlargePlot * enlagePlot = new EnlargePlot(this);
    enlagePlot->disPlay_data(dc->activeList2880_Day3,0,Va);
    enlagePlot->disPlay_data(dc->lightList2880_Day3,1,Vl);
    enlagePlot->disPlay_data(dc->voiceList2880_Day3,2,Vv);
    enlagePlot->show();
}

void DataAnalysis::slot_doubleClick4(QMouseEvent *)
{
    DataContainer* dc =DataContainer::getInstance();
    if(!flag_clicked_disPlay)
    {
        return;
    }
    bool Va=false,Vl=false,Vv=false;
    if(ckb_active->checkState()==Qt::Checked)   Va=true;
    if(ckb_light->checkState()==Qt::Checked)    Vl=true;
    if(ckb_voice->checkState()==Qt::Checked)    Vv=true;
    EnlargePlot * enlagePlot = new EnlargePlot(this);
    enlagePlot->disPlay_data(dc->activeList2880_Day4,0,Va);
    enlagePlot->disPlay_data(dc->lightList2880_Day4,1,Vl);
    enlagePlot->disPlay_data(dc->voiceList2880_Day4,2,Vv);
    enlagePlot->show();
}

void DataAnalysis::slot_doubleClick5(QMouseEvent *)
{
    DataContainer* dc =DataContainer::getInstance();
    if(!flag_clicked_disPlay)
    {
        return;
    }
    bool Va=false,Vl=false,Vv=false;
    if(ckb_active->checkState()==Qt::Checked)   Va=true;
    if(ckb_light->checkState()==Qt::Checked)    Vl=true;
    if(ckb_voice->checkState()==Qt::Checked)    Vv=true;
    EnlargePlot * enlagePlot = new EnlargePlot(this);
    enlagePlot->disPlay_data(dc->activeList2880_Day5,0,Va);
    enlagePlot->disPlay_data(dc->lightList2880_Day5,1,Vl);
    enlagePlot->disPlay_data(dc->voiceList2880_Day5,2,Vv);
    enlagePlot->show();
}

void DataAnalysis::slot_doubleClick6(QMouseEvent *)
{
    DataContainer* dc =DataContainer::getInstance();
    if(!flag_clicked_disPlay)
    {
        return;
    }
    bool Va=false,Vl=false,Vv=false;
    if(ckb_active->checkState()==Qt::Checked)   Va=true;
    if(ckb_light->checkState()==Qt::Checked)    Vl=true;
    if(ckb_voice->checkState()==Qt::Checked)    Vv=true;
    EnlargePlot * enlagePlot = new EnlargePlot(this);
    enlagePlot->disPlay_data(dc->activeList2880_Day6,0,Va);
    enlagePlot->disPlay_data(dc->lightList2880_Day6,1,Vl);
    enlagePlot->disPlay_data(dc->voiceList2880_Day6,2,Vv);
    enlagePlot->show();
}
/**
 * @brief DataAnalysis::初始化顶层信息窗口
 */
void DataAnalysis::init_PageTopInfo()
{
    pageName =new  QLabel(this);
    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor(92,123,137));
    pageName->setPalette(pa);
    pageName->setFont(QFont("楷体",22,75));
    pageName->setText("数据分析");

    btn_createUser = new QPushButton(this);
    btn_createUser->setIcon(QIcon(":/picture/xiefeipic/新建用户.jpg"));
    btn_createUser->setIconSize(QSize(30,30));
    btn_createUser->setText(" 新建用户");
    btn_createUser->setStyleSheet(""
                                  "QPushButton{font:18px;font-weight:bold;color:rgb(96 , 125 , 139);background-color:transparent;}"
                                  "QPushButton:hover {font:20px;color:rgb(136 , 136 , 136);}"
                                  "QPushButton:pressed { color:rgb(0 , 0 , 0);}"
                                  "");
    connect(btn_createUser,SIGNAL(clicked(bool)),this,SLOT(slot_createUsr()));

    btn_switchUser = new QPushButton(this);
    btn_switchUser->setIcon(QIcon(":/picture/xiefeipic/切换用户.jpg"));
    btn_switchUser->setIconSize(QSize(30,30));
    btn_switchUser->setText(" 切换用户");
    btn_switchUser->setStyleSheet(""
                                  "QPushButton{font:18px;font-weight:bold;color:rgb(96 , 125 , 139);background-color:transparent;}"
                                  "QPushButton:hover {font:20px;color:rgb(136 , 136 , 136);}"
                                  "QPushButton:pressed { color:rgb(0 , 0 , 0);}"
                                  "");
    connect(btn_switchUser,SIGNAL(clicked(bool)),this,SLOT(slot_switchUsr()));
    ly_H_Top = new QHBoxLayout();
    ly_H_Top->addSpacing(10);
    ly_H_Top->addWidget(pageName);
    ly_H_Top->addStretch();
    ly_H_Top->addWidget(btn_createUser);
    ly_H_Top->addWidget(btn_switchUser);
    ly_H_Top->addSpacing(10);
    // ly_H_Top->setGeometry(QRect(0,0,thisWidth,40));//单独使用调试
}
/**
 * @brief 初始化分析窗口工具栏
 */
void DataAnalysis::init_PageSecInfo()
{
    ckb_voice   = new QCheckBox(this);
    ckb_voice->setStyleSheet(
                "QCheckBox {spacing: 0px;}"
                "QCheckBox::indicator {width: 30px;height: 30px;}"
                "QCheckBox::indicator:unchecked {image: url(:/picture/huangzhijie/声音-unselect.png);} "
                "QCheckBox::indicator:checked   {image: url(:/picture/huangzhijie/声音-select.png); }"
                );
    ckb_voice->setFont(QFont("楷体",12,75));
    ckb_voice->setText("声音");

    ckb_light   = new QCheckBox(this);
    ckb_light->setFont(QFont("楷体",12,75));
    ckb_light->setText("光强");
    ckb_light->setStyleSheet(
                "QCheckBox {spacing: 0px;}"
                "QCheckBox::indicator {width: 30px;height: 30px;}"
                "QCheckBox::indicator:unchecked {image: url(:/picture/huangzhijie/光强-unselect.png);} "
                "QCheckBox::indicator:checked   {image: url(:/picture/huangzhijie/光强-select.png); }"
                );

    ckb_active  = new QCheckBox(this);
    ckb_active->setFont(QFont("楷体",12,75));
    ckb_active->setText("活动量");
    ckb_active->setStyleSheet(
                "QCheckBox {spacing: 0px;}"
                "QCheckBox::indicator {width: 30px;height: 30px;}"
                "QCheckBox::indicator:unchecked {image: url(:/picture/huangzhijie/活动-unselect.png);} "
                "QCheckBox::indicator:checked   {image: url(:/picture/huangzhijie/活动-select.png); }"
                );

    ckb_all     = new QCheckBox(this);
    ckb_all->setFont(QFont("楷体",12,75));
    ckb_all->setText("综合");
    ckb_all->setStyleSheet(
                "QCheckBox {spacing: 0px;}"
                "QCheckBox::indicator {width: 30px;height: 30px;}"
                "QCheckBox::indicator:unchecked {image: url(:/picture/huangzhijie/综合-unselect.png);} "
                "QCheckBox::indicator:checked   {image: url(:/picture/huangzhijie/综合-select.png);}"
                );
    connect(ckb_all,SIGNAL(stateChanged(int)),this,SLOT(slot_checkboxALL(int)));
    connect(ckb_active,SIGNAL(stateChanged(int)),this,SLOT(slot_checkboxACT(int)));
    connect(ckb_light,SIGNAL(stateChanged(int)),this,SLOT(slot_checkboxLGT(int)));
    connect(ckb_voice,SIGNAL(stateChanged(int)),this,SLOT(slot_checkboxVOC(int)));

    lb_currentUser =new QLabel(this);
    lb_currentUser->setText("当前用户：");
    lb_currentUser->setFont(QFont("楷体",14));

    btn_UPLoadData  =new QPushButton(this);
    //    btn_UPLoadData->setFixedSize(100,30);
    //    btn_UPLoadData->setStyleSheet("QPushButton{border-image: url(:/picture/uploaddata-up.png);}"
    //                                  "QPushButton:hover{border-image: url(:/picture/uploaddata-up.png);}"
    //                                  "QPushButton:pressed{border-image: url(:/picture/uploaddata-down.png);}");
    //    btn_UPLoadData->setFlat(true);

    btn_UPLoadData->setIcon(QIcon(":/picture/xiefeipic/数据上传.jpg"));
    btn_UPLoadData->setIconSize(QSize(30,30));
    btn_UPLoadData->setText("数据上载");
    btn_UPLoadData->setStyleSheet(""
                                  "QPushButton{font:18px;font-weight:bold;color:rgb(96 , 125 , 139);background-color:transparent;}"
                                  "QPushButton:hover {font:20px;color:rgb(136 , 136 , 136);}"
                                  "QPushButton:pressed { color:rgb(0 , 0 , 0);}"
                                  "");

    connect(btn_UPLoadData,SIGNAL(clicked()),this,SLOT(slot_uploaddata()));


    btn_OpenAndChoose   =new QPushButton(this);
    //    btn_OpenAndChoose->setFixedSize(100,30);
    //    btn_OpenAndChoose->setStyleSheet("QPushButton{border-image: url(:/picture/openchoose-up.png);}"
    //                                     "QPushButton:hover{border-image: url(:/picture/openchoose-up.png);}"
    //                                     "QPushButton:pressed{border-image: url(:/picture/openchoose-down.png);}");
    //    btn_OpenAndChoose->setFlat(true);
    btn_OpenAndChoose->setIcon(QIcon(":/picture/xiefeipic/自动分析.jpg"));
    btn_OpenAndChoose->setIconSize(QSize(30,30));
    btn_OpenAndChoose->setText("打开文件");
    btn_OpenAndChoose->setStyleSheet(""
                                     "QPushButton{font:18px;font-weight:bold;color:rgb(96 , 125 , 139);background-color:transparent;}"
                                     "QPushButton:hover {font:20px;color:rgb(136 , 136 , 136);}"
                                     "QPushButton:pressed { color:rgb(0 , 0 , 0);}"
                                     "");
    connect(btn_OpenAndChoose,SIGNAL(clicked(bool)),this,SLOT( slot_openchoose()));

    btn_SaveResult      =new QPushButton(this);
    //    btn_SaveResult->setFixedSize(100,30);
    //    btn_SaveResult->setStyleSheet("QPushButton{border-image: url(:/picture/saveresult-up.png);}"
    //                                  "QPushButton:hover{border-image: url(:/picture/saveresult-up.png);}"
    //                                  "QPushButton:pressed{border-image: url(:/picture/saveresult-down.png);}");
    //    btn_SaveResult->setFlat(true);
    btn_SaveResult->setIcon(QIcon(":/picture/xiefeipic/确认结果.jpg"));
    btn_SaveResult->setIconSize(QSize(30,30));
    btn_SaveResult->setText("确认保存");
    btn_SaveResult->setStyleSheet(""
                                  "QPushButton{font:18px;font-weight:bold;color:rgb(96 , 125 , 139);background-color:transparent;}"
                                  "QPushButton:hover {font:20px;color:rgb(136 , 136 , 136);}"
                                  "QPushButton:pressed { color:rgb(0 , 0 , 0);}"
                                  "");
    connect(btn_SaveResult,SIGNAL(clicked(bool)),this,SLOT( slot_saveresult()));

    btn_currentUser     =new QPushButton(this);
    connect(btn_currentUser,SIGNAL(clicked(bool)),this,SLOT(slot_currentUser()));
    QString patientname = DataContainer::getInstance()->PatientName;
    btn_currentUser->setText(patientname);btn_currentUser->setFont(QFont("楷体",14,75));
    btn_currentUser->setFixedSize(100,30);
    btn_currentUser->setStyleSheet(
                "QPushButton{background-color:rgb(255,255,255);"
                "color: black;   "
                "border-radius: 10px;  "
                "        border: 2px groove gray; "
                "  border-style: outset;}"
                "QPushButton:hover{background-color:white; color: black;}"
                "QPushButton:pressed{background-color:rgb(85, 170, 255); "
                "border-style: inset; }"

                );

    ly_H_Sec = new QHBoxLayout();

    ly_H_Sec->addSpacing(40);
    ly_H_Sec->addWidget(btn_UPLoadData);
    ly_H_Sec->addSpacing(10);
    ly_H_Sec->addWidget(btn_OpenAndChoose);
    ly_H_Sec->addSpacing(10);
    ly_H_Sec->addWidget(btn_SaveResult);
    ly_H_Sec->addStretch();
    ly_H_Sec->addWidget(ckb_voice);
    ly_H_Sec->addSpacing(40);
    ly_H_Sec->addWidget(ckb_light);
    ly_H_Sec->addSpacing(40);
    ly_H_Sec->addWidget(ckb_active);
    ly_H_Sec->addSpacing(40);
    ly_H_Sec->addWidget(ckb_all);

    ly_H_Sec->addSpacing(80);
    ly_H_Sec->addWidget(lb_currentUser);
    ly_H_Sec->addWidget(btn_currentUser);
    ly_H_Sec->addSpacing(10);

}

/**
 * @brief 分析窗口侧边显示
 */
void DataAnalysis::init_PageTitleInfo()
{
    lb_Date = new QLabel(this);
    lb_Date->setText("日期");
    lb_Date->setAlignment(Qt::AlignCenter);
    lb_Date->setStyleSheet("background-color:rgb(212,212,212);");
    lb_Date->setMaximumWidth(LABELDATEMAXWIDTH);

    my_Time_Scale = new MyTimeScale(this);
    ly_H_Title = new QHBoxLayout();
    ly_H_Title->addWidget(lb_Date);
    ly_H_Title->addWidget(my_Time_Scale);

}

void DataAnalysis::init_PagePlotInfo()
{
    lb_Day1 = new QLabel(this);
    lb_Day1->setText("Day1");
    lb_Day1->setAlignment(Qt::AlignCenter);
    lb_Day1->setStyleSheet("background-color:rgb(212,212,212);");
    lb_Day1->setMaximumWidth(LABELDATEMAXWIDTH);

    lb_Day2 = new QLabel(this);
    lb_Day2->setText("Day2");
    lb_Day2->setAlignment(Qt::AlignCenter);
    lb_Day2->setStyleSheet("background-color:rgb(212,212,212);");
    lb_Day2->setMaximumWidth(LABELDATEMAXWIDTH);

    lb_Day3 = new QLabel(this);
    lb_Day3->setText("Day3");
    lb_Day3->setAlignment(Qt::AlignCenter);
    lb_Day3->setStyleSheet("background-color:rgb(212,212,212);");
    lb_Day3->setMaximumWidth(LABELDATEMAXWIDTH);

    lb_Day4 = new QLabel(this);
    lb_Day4->setText("Day4");
    lb_Day4->setAlignment(Qt::AlignCenter);
    lb_Day4->setStyleSheet("background-color:rgb(212,212,212);");
    lb_Day4->setMaximumWidth(LABELDATEMAXWIDTH);

    lb_Day5 = new QLabel(this);
    lb_Day5->setText("Day5");
    lb_Day5->setAlignment(Qt::AlignCenter);
    lb_Day5->setStyleSheet("background-color:rgb(212,212,212);");
    lb_Day5->setMaximumWidth(LABELDATEMAXWIDTH);

    lb_Day6 = new QLabel(this);
    lb_Day6->setText("Day6");
    lb_Day6->setAlignment(Qt::AlignCenter);
    lb_Day6->setStyleSheet("background-color:rgb(212,212,212);");
    lb_Day6->setMaximumWidth(LABELDATEMAXWIDTH);


    myplot1 = new MyPlot(this);
    myplot1->show();


    myplot2 = new MyPlot(this);
    myplot2->show();

    myplot3 = new MyPlot(this);
    myplot3->show();

    myplot4 = new MyPlot(this);
    myplot4->show();

    myplot5 = new MyPlot(this);
    myplot5->show();

    myplot6 = new MyPlot(this);
    myplot6->show();
    /****************************双击波形图，放大*************************************/
    connect(myplot1->temPIC,SIGNAL(mouseDoubleClick(QMouseEvent*)),this,SLOT(slot_doubleClick1(QMouseEvent*)));
    connect(myplot2->temPIC,SIGNAL(mouseDoubleClick(QMouseEvent*)),this,SLOT(slot_doubleClick2(QMouseEvent*)));
    connect(myplot3->temPIC,SIGNAL(mouseDoubleClick(QMouseEvent*)),this,SLOT(slot_doubleClick3(QMouseEvent*)));
    connect(myplot4->temPIC,SIGNAL(mouseDoubleClick(QMouseEvent*)),this,SLOT(slot_doubleClick4(QMouseEvent*)));
    connect(myplot5->temPIC,SIGNAL(mouseDoubleClick(QMouseEvent*)),this,SLOT(slot_doubleClick5(QMouseEvent*)));
    connect(myplot6->temPIC,SIGNAL(mouseDoubleClick(QMouseEvent*)),this,SLOT(slot_doubleClick6(QMouseEvent*)));

    ly_H_plot1 = new QHBoxLayout();ly_H_plot1->addWidget(lb_Day1);ly_H_plot1->addWidget(myplot1);
    ly_H_plot2 = new QHBoxLayout();ly_H_plot2->addWidget(lb_Day2);ly_H_plot2->addWidget(myplot2);
    ly_H_plot3 = new QHBoxLayout();ly_H_plot3->addWidget(lb_Day3);ly_H_plot3->addWidget(myplot3);
    ly_H_plot4 = new QHBoxLayout();ly_H_plot4->addWidget(lb_Day4);ly_H_plot4->addWidget(myplot4);
    ly_H_plot5 = new QHBoxLayout();ly_H_plot5->addWidget(lb_Day5);ly_H_plot5->addWidget(myplot5);
    ly_H_plot6 = new QHBoxLayout();ly_H_plot6->addWidget(lb_Day6);ly_H_plot6->addWidget(myplot6);
}

void DataAnalysis::init_PageAllLayout()
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
    ly_V_all->addLayout(ly_H_Title);

    QLabel *m_label3 = new QLabel(this);
    m_label3->setFrameShape(QFrame::HLine);
    m_label3->setFrameShadow(QFrame::Raised);
    m_label3->setMaximumSize(QSize(6000, 1));
    ly_V_all->addWidget(m_label3);
    ly_V_all->addLayout(ly_H_plot1);

    QLabel *m_label4 = new QLabel(this);
    m_label4->setFrameShape(QFrame::HLine);
    m_label4->setFrameShadow(QFrame::Raised);
    m_label4->setMaximumSize(QSize(6000, 1));
    ly_V_all->addWidget(m_label4);
    ly_V_all->addLayout(ly_H_plot2);

    QLabel *m_label5 = new QLabel(this);
    m_label5->setFrameShape(QFrame::HLine);
    m_label5->setFrameShadow(QFrame::Raised);
    m_label5->setMaximumSize(QSize(6000, 1));
    ly_V_all->addWidget(m_label5);
    ly_V_all->addLayout(ly_H_plot3);

    QLabel *m_label6 = new QLabel(this);
    m_label6->setFrameShape(QFrame::HLine);
    m_label6->setFrameShadow(QFrame::Raised);
    m_label6->setMaximumSize(QSize(6000, 1));
    ly_V_all->addWidget(m_label6);
    ly_V_all->addLayout(ly_H_plot4);

    QLabel *m_label7 = new QLabel(this);
    m_label7->setFrameShape(QFrame::HLine);
    m_label7->setFrameShadow(QFrame::Raised);
    m_label7->setMaximumSize(QSize(6000, 1));
    ly_V_all->addWidget(m_label7);
    ly_V_all->addLayout(ly_H_plot5);

    QLabel *m_label8 = new QLabel(this);
    m_label8->setFrameShape(QFrame::HLine);
    m_label8->setFrameShadow(QFrame::Raised);
    m_label8->setMaximumSize(QSize(6000, 1));
    ly_V_all->addWidget(m_label8);
    ly_V_all->addLayout(ly_H_plot6);

    // ly_V_all->addStretch();

    ly_V_all->setGeometry(QRect(0,0,thisWidth,thisHeight));

}

void DataAnalysis::init_MarkFrame()
{
    Markbar1 = new MarkFrame(this);Markbar1->setMaximumHeight(20);
    Markbar2 = new MarkFrame(this);Markbar2->setMaximumHeight(20);
    Markbar3 = new MarkFrame(this);Markbar3->setMaximumHeight(20);
    Markbar4 = new MarkFrame(this);Markbar4->setMaximumHeight(20);
    Markbar5 = new MarkFrame(this);Markbar5->setMaximumHeight(20);
    Markbar6 = new MarkFrame(this);Markbar6->setMaximumHeight(20);
    //9与18 为了与temPIC对齐，画图位置起始点不是9，实际观察是27
    Markbar1->setGeometry(myplot1->x()+9,myplot1->y()+myplot1->height()-25,myplot1->width()-18,20);
    Markbar2->setGeometry(myplot2->x()+9,myplot2->y()+myplot2->height()-25,myplot2->width()-18,20);
    Markbar3->setGeometry(myplot3->x()+9,myplot3->y()+myplot3->height()-25,myplot3->width()-18,20);
    Markbar4->setGeometry(myplot4->x()+9,myplot4->y()+myplot4->height()-25,myplot4->width()-18,20);
    Markbar5->setGeometry(myplot5->x()+9,myplot5->y()+myplot5->height()-25,myplot5->width()-18,20);
    Markbar6->setGeometry(myplot6->x()+9,myplot6->y()+myplot6->height()-25,myplot6->width()-18,20);
}

void DataAnalysis::init_allStatus()
{
    DataContainer* dc =DataContainer::getInstance();
    if(dc->PatientName!="" && dc->ChooseNUM>= 1 &&dc->activeList2880_Day1.length()==2880)
    {
        slot_Develop_DrawPicture();
    }
}

void DataAnalysis::AutoMark()
{
    //    QVector<struct_Duan> temp;
    //    getMarkData_FromDeepAndShalow(Kshine12,temp);
    //    Markbar1->autoMaticMark(temp);
    //    getMarkData_FromDeepAndShalow(Kshine22,temp);
    //    Markbar2->autoMaticMark(temp);
    //    getMarkData_FromDeepAndShalow(Kshine32,temp);
    //    Markbar3->autoMaticMark(temp);
    //    getMarkData_FromDeepAndShalow(Kshine42,temp);
    //    Markbar4->autoMaticMark(temp);
    //    getMarkData_FromDeepAndShalow(Kshine52,temp);
    //    Markbar5->autoMaticMark(temp);
    //    getMarkData_FromDeepAndShalow(Kshine62,temp);
    //    Markbar6->autoMaticMark(temp);

}
