#include "readfiledata.h"
#include <QDebug>
#include <QDir>

#define flag_When_readytosleep          -2
#define flag_When_wake_up_now			-3
#define flag_When_power_on				-13
#define flag_When_power_off				-14
#define flag_When_usb_connect			-15
#define flag_When_usb_disconnect        -16


ReadFileData::ReadFileData(QObject *parent): QObject(parent)
{
    dc = DataContainer::getInstance();
    fc = FlagContainer::getInstance();
}

int ReadFileData::getTotalFileNum(const QFileInfoList&  fileinfolist)
{
    int sizzze=0;
    for(int i=0;i<fileinfolist.length();i++)
    {
        QDir dir(fileinfolist[i].filePath());//获取日期文件夹对象
        //开始遍历某一文件夹里的所有csv文件
        QFileInfoList files = dir.entryInfoList();
        foreach (QFileInfo file, files)//在文件夹中，遍历每一个CSV文件
            if(file.fileName() != "." && file.fileName() != "..")
                sizzze++;//记文件的个数
    }
    return sizzze;//返回文件的个数
}

#if 1

const char CSV[24][10]={
    "\\112.csv","\\113.csv","\\114.csv","\\115.csv","\\116.csv","\\117.csv","\\118.csv","\\119.csv","\\120.csv","\\121.csv","\\122.csv","\\123.csv",
    "\\200.csv","\\201.csv","\\202.csv","\\203.csv","\\204.csv","\\205.csv","\\206.csv","\\207.csv","\\208.csv","\\209.csv","\\210.csv","\\211.csv"
                     };
void ReadFileData::startReadFileData(const QStringList &strlist )
{
    if(strlist.length()==0){emit work_over();return;}
    if(dc->PatientName=="")
    {
        emit work_message("代码未指定用户名！");
        emit work_over();
        return;
    }
    int num = dc->ChooseNUM*24;
    emit work_TotalNumber(num);
    //清空原始数据
    if(!dc->activeList86400_Day1.isEmpty())dc->activeList86400_Day1.clear();if(!dc->lightList86400_Day1.isEmpty())dc->lightList86400_Day1.clear();if(!dc->voiceList86400_Day1.isEmpty())dc->voiceList86400_Day1.clear();
    if(!dc->activeList86400_Day2.isEmpty())dc->activeList86400_Day2.clear();if(!dc->lightList86400_Day2.isEmpty())dc->lightList86400_Day2.clear();if(!dc->voiceList86400_Day2.isEmpty())dc->voiceList86400_Day2.clear();
    if(!dc->activeList86400_Day3.isEmpty())dc->activeList86400_Day3.clear();if(!dc->lightList86400_Day3.isEmpty())dc->lightList86400_Day3.clear();if(!dc->voiceList86400_Day3.isEmpty())dc->voiceList86400_Day3.clear();
    if(!dc->activeList86400_Day4.isEmpty())dc->activeList86400_Day4.clear();if(!dc->lightList86400_Day4.isEmpty())dc->lightList86400_Day4.clear();if(!dc->voiceList86400_Day4.isEmpty())dc->voiceList86400_Day4.clear();
    if(!dc->activeList86400_Day5.isEmpty())dc->activeList86400_Day5.clear();if(!dc->lightList86400_Day5.isEmpty())dc->lightList86400_Day5.clear();if(!dc->voiceList86400_Day5.isEmpty())dc->voiceList86400_Day5.clear();
    if(!dc->activeList86400_Day6.isEmpty())dc->activeList86400_Day6.clear();if(!dc->lightList86400_Day6.isEmpty())dc->lightList86400_Day6.clear();if(!dc->voiceList86400_Day6.isEmpty())dc->voiceList86400_Day6.clear();

    //得到18个容器的原始数据
    QVector<double> * VA,*VL,*VV;
    //QString pathStr = "d:\\Active_Data\\"+dc->PatientName+"\\";
    QString pathStr = "./Active_Data/"+dc->PatientName+"/";
    int flag=0;
    for(int i=0;i<strlist.length();i++)
    {
             if(i==0){VA=&dc->activeList86400_Day1;VL=&dc->lightList86400_Day1;VV=&dc->voiceList86400_Day1;}
        else if(i==1){VA=&dc->activeList86400_Day2;VL=&dc->lightList86400_Day2;VV=&dc->voiceList86400_Day2;}
        else if(i==2){VA=&dc->activeList86400_Day3;VL=&dc->lightList86400_Day3;VV=&dc->voiceList86400_Day3;}
        else if(i==3){VA=&dc->activeList86400_Day4;VL=&dc->lightList86400_Day4;VV=&dc->voiceList86400_Day4;}
        else if(i==4){VA=&dc->activeList86400_Day5;VL=&dc->lightList86400_Day5;VV=&dc->voiceList86400_Day5;}
        else if(i==5){VA=&dc->activeList86400_Day6;VL=&dc->lightList86400_Day6;VV=&dc->voiceList86400_Day6;}
        //开始遍历某一文件夹里的所有csv文件
        int csv_counter=0;
        QString sdate=strlist[i];
        int Year=sdate.section('-', 0, 0).toInt();//2018-03-03
        int Month=sdate.section('-', 1, 1).toInt();
        int Day=sdate.section('-', 2, 2).toInt();
        for(int kk=0;kk<24;kk++)
        {
            QFileInfo file(pathStr+strlist[i]+CSV[kk]);
            qDebug()<<"------ now open"<<file.fileName();

                //qDebug()<<"SIZE"<<file.size();
                if(file.exists()==false)
                {
                    qDebug()<<"\033[44m[Error] "<<file.fileName()<<"不存在！\033[0m";
                    //填方不存在波动
                    for(int i=1;i<=3600;i++)
                    {
                        int tem =0;
                        if(i<=600||(i>1200&&i<=1800)||(i>2400&&i<=3000))tem=360;
                        VA->append(tem);
                        VL->append(tem);
                        VV->append(tem);
                    }
                    continue;
                }

                if(file.size()!=104434)
                {
                    qDebug()<<"\033[43m[Error] "<<file.fileName()<<"size错误！\033[0m";
                    //填方错误波动
                    for(int i=1;i<=3600;i++)
                    {
                        int tem =0;
                        int ii=i%600;
                        if((ii%600)<=300)tem=1.2*ii;
                        else tem = 720-1.2*ii;

                        VA->append(tem);
                        VL->append(tem);
                        VV->append(tem);
                    }
                    continue;
                }
                QString path = file.filePath();
                QFile file1(path);
                //打开文件
                file1.open(QIODevice::ReadOnly | QIODevice::Text);
                //QString s = ",";//分隔符为空格
                QTextStream in(&file1);
                in.seek(34);    //文件偏移，去掉表头
                int row =0;
                while(!in.atEnd())
                {
                    row++;
                    QStringList list    = in.readLine(28).split(",");     //一行内的数据
                    QString s1          = list.at(1);//活动（标志位）
                    QString s2          = list.at(2);//光强（year）
                    QString s3          = list.at(3);//温度（month）
                    QString s4          = list.at(4);//声音（Day）
                    //转换-------------------------------------------
                    int n1=s1.toInt();
                    int n2=s2.toInt();
                    int n3=s3.toInt();
                    int n4=s4.toInt();
                    int NUM=VA->length()+i*86400;//当前坐标0~6*86400-1
                    //检测是否为标志
                    if(n2==Year&&n3==Month&&n4==Day)//数据日期与文件夹符合
                    {
                        if(n1==flag_When_power_on)//开机
                        {
                            //qDebug()<<"************开机标志*************";
                            if(!fc->VPower.isEmpty()&&fc->VPower.last().end==KERRO_Flag)
                            {
                                //如果上一次的开机没有结束：
                                //保留上一次至今的所有数据，需要手动标记。
                                fc->VPower.last().end=NUM;//封口
                            }
                            FPower fp;
                            fp.start=NUM;fp.end=KERRO_Flag;
                            fp.year =Year;
                            fp.month=Month;
                            fp.day  =Day;
                            fp.row  =row;
                            fc->VPower.append(fp);
                        }
                        else if(n1==flag_When_power_off)//关机
                        {
                            //qDebug()<<"************关机标志*************";
                            if(!fc->VPower.isEmpty()
                                  //  &&fc->VPower.last().end==KERRO_Flag
                                    &&Year>=fc->VPower.last().year
                                    &&Month>=fc->VPower.last().month
                                    &&Day>=fc->VPower.last().day
                                    &&row>=fc->VPower.last().row)//时间条件符合
                                fc->VPower.last().end=NUM;
                        }
                        else if(n1==flag_When_readytosleep)//睡眠
                        {
                            //qDebug()<<"************睡眠标志*************";
                            if(!fc->VSleep.isEmpty()&&fc->VSleep.last().end==KERRO_Flag)
                            {
                                //如果上一次的睡眠标志还没结束
                                //封口
                                fc->VSleep.last().end=NUM;
                            }
                            FSleep fs;
                            fs.start=NUM;fs.end=KERRO_Flag;
                            fs.year =Year;
                            fs.month=Month;
                            fs.day  =Day;
                            fs.row  =row;
                            fc->VSleep.append(fs);
                        }
                        else if(n1==flag_When_wake_up_now)//唤醒
                        {
                            //qDebug()<<"************唤醒标志*************";
                            if(!fc->VSleep.isEmpty())
                                fc->VSleep.last().end=NUM;

                        }
                        else if(n1==flag_When_usb_connect)//USB插入
                        {
                            qDebug()<<"************USB插入*************";

                        }
                        else if(n1==flag_When_usb_disconnect)//USB拔出
                        {
                            qDebug()<<"************USB拔出*************";
                        }
                    }

                    VA->append(n1);
                    VL->append(n2);
                    VV->append(n4);
                }
                //qDebug()<<file.fileName();
                emit work_Progress_int(1);//完成一个文件
                csv_counter++;
        }
        if(VA->length()<86400)
        {
             qDebug()<<"\033[31m[Error] "<<strlist[i]<<"----> csv文件数："<<csv_counter<<"数据数："<<VA->length()<<"\033[0m";
             for(int i=VA->length();i<86400;i++)
             {

                 VA->append(-100);
                 VL->append(-100);
                 VV->append(-100);
             }
             qDebug()<<"\033[34m ----------<<<< 代码填补 <<<<\033[0m";

        }
        if( csv_counter <24)flag=1;

        //完成一个文件夹的读取
    }
    if(flag==1)
        emit work_message("发生文件数据丢失！\n(1)尖波表示该时刻文件数据错误;\n(2)方波表示该时刻文件丢失;\n");

    emit work_over();

}
#else
void ReadFileData::startReadFileData(const QStringList &strlist )
{
    if(strlist.length()==0){emit work_over();return;}
    if(dc->PatientName=="")
    {
        emit work_message("代码未指定用户名！");
        emit work_over();
        return;
    }

    //将stringlist 装换成 fileinfolist
    QFileInfoList  fileinfolist;
    QString pathStr = "d:\\Active_Data\\"+dc->PatientName+"\\";
    for(int i=0;i<strlist.length();i++)
    {
        QFileInfo tem(pathStr+strlist[i]);
        fileinfolist.append(tem);
    }
    int num =getTotalFileNum(fileinfolist);
    qDebug()<<"startReadFileData"<<pathStr<<"目标文件个数："<<num;
    //潜在的BUG：如果num为0（文件夹下文件的个数不正常），会报段错误。暂时不想处理。
    emit work_TotalNumber(num);

    //清空原始数据
    if(!dc->activeList86400_Day1.isEmpty())dc->activeList86400_Day1.clear();if(!dc->lightList86400_Day1.isEmpty())dc->lightList86400_Day1.clear();if(!dc->voiceList86400_Day1.isEmpty())dc->voiceList86400_Day1.clear();
    if(!dc->activeList86400_Day2.isEmpty())dc->activeList86400_Day2.clear();if(!dc->lightList86400_Day2.isEmpty())dc->lightList86400_Day2.clear();if(!dc->voiceList86400_Day2.isEmpty())dc->voiceList86400_Day2.clear();
    if(!dc->activeList86400_Day3.isEmpty())dc->activeList86400_Day3.clear();if(!dc->lightList86400_Day3.isEmpty())dc->lightList86400_Day3.clear();if(!dc->voiceList86400_Day3.isEmpty())dc->voiceList86400_Day3.clear();
    if(!dc->activeList86400_Day4.isEmpty())dc->activeList86400_Day4.clear();if(!dc->lightList86400_Day4.isEmpty())dc->lightList86400_Day4.clear();if(!dc->voiceList86400_Day4.isEmpty())dc->voiceList86400_Day4.clear();
    if(!dc->activeList86400_Day5.isEmpty())dc->activeList86400_Day5.clear();if(!dc->lightList86400_Day5.isEmpty())dc->lightList86400_Day5.clear();if(!dc->voiceList86400_Day5.isEmpty())dc->voiceList86400_Day5.clear();
    if(!dc->activeList86400_Day6.isEmpty())dc->activeList86400_Day6.clear();if(!dc->lightList86400_Day6.isEmpty())dc->lightList86400_Day6.clear();if(!dc->voiceList86400_Day6.isEmpty())dc->voiceList86400_Day6.clear();
    //得到18个容器的原始数据

    QVector<double> * VA,*VL,*VV;
    for(int i=0;i<strlist.length();i++)
    {


             if(i==0){VA=&dc->activeList86400_Day1;VL=&dc->lightList86400_Day1;VV=&dc->voiceList86400_Day1;}
        else if(i==1){VA=&dc->activeList86400_Day2;VL=&dc->lightList86400_Day2;VV=&dc->voiceList86400_Day2;}
        else if(i==2){VA=&dc->activeList86400_Day3;VL=&dc->lightList86400_Day3;VV=&dc->voiceList86400_Day3;}
        else if(i==3){VA=&dc->activeList86400_Day4;VL=&dc->lightList86400_Day4;VV=&dc->voiceList86400_Day4;}
        else if(i==4){VA=&dc->activeList86400_Day5;VL=&dc->lightList86400_Day5;VV=&dc->voiceList86400_Day5;}
        else if(i==5){VA=&dc->activeList86400_Day6;VL=&dc->lightList86400_Day6;VV=&dc->voiceList86400_Day6;}



        qDebug()<<"打开："<<fileinfolist[i].fileName()<<"----"<<fileinfolist[i].filePath();
        QDir dir(fileinfolist[i].filePath());//获取日期文件夹对象

        //开始遍历某一文件夹里的所有csv文件
        int csv_counter=0;
        QFileInfoList files = dir.entryInfoList();
        foreach (QFileInfo file, files)//在文件夹中，遍历每一个CSV文件
        {
            if(file.fileName() != "." && file.fileName() != "..")
            {
                //qDebug()<<"SIZE"<<file.size();
                if(file.size()!=104434)
                {
                    qDebug()<<"\033[43m[Error] "<<file.fileName()<<"size错误！\033[0m";
                    //填方错误波动



                    continue;
                }
                QString path = file.filePath();
                QFile file1(path);
                //打开文件
                file1.open(QIODevice::ReadOnly | QIODevice::Text);
                //QString s = ",";//分隔符为空格
                QTextStream in(&file1);
                in.seek(34);    //文件偏移，去掉表头
                //int i =0;
                while(!in.atEnd())
                {
                    QStringList list    = in.readLine(28).split(",");     //一行内的数据
                    QString s1          = list.at(1);
                    QString s2          = list.at(2);
                    QString s4          = list.at(4);
                    VA->append(s1.toInt());
                    VL->append(s2.toInt());
                    VV->append(s4.toInt());
                }
                //完成一个csv文件的读取
                csv_counter++;
                //qDebug()<<file.fileName();
                emit work_Progress_int(1);//完成一个文件
            }
        }
        if(csv_counter<24 || VA->length()!=86400)
        {
             qDebug()<<"\033[31m[Error] "<<fileinfolist[i].fileName()<<"----> csv文件数："<<csv_counter<<"数据数："<<VA->length()<<"\033[0m";
             for(int i=VA->length();i<86400;i++)
             {
                 VA->append(-100);
                 VL->append(-100);
                 VV->append(-100);
             }
             qDebug()<<"\033[34m ----------<<<< 代码填补 <<<<\033[0m";
        }
        //完成一个文件夹的读取
    }


    emit work_message("已读取所有数据！");
    emit work_over();
}
#endif
