#include "dataanalyze.h"
#include "QDebug"
#define FT6 400
#define FT5 320
#define FT4 240
#define FT3 160
#define FT2 80
#define FT1 0
DataAnalyze::DataAnalyze(QObject *parent) : QObject(parent)
{
        dc = DataContainer::getInstance();
        chknum=dc->ChooseNUM;
//        Markbarwidth=Markbar_width;
}

//阶梯化（6梯度）
void DataAnalyze::getData_FuzzificationTable()
{
    QVector<double> *vec=NULL;
    QVector<double> *V_Result=NULL;

    for(int pi=0;pi<chknum;pi++)
    {
             if(pi==0){vec=&dc->activeList144_Day1;V_Result=&dc->AnalysisResult144_Day1;}
        else if(pi==1){vec=&dc->activeList144_Day2;V_Result=&dc->AnalysisResult144_Day2;}
        else if(pi==2){vec=&dc->activeList144_Day3;V_Result=&dc->AnalysisResult144_Day3;}
        else if(pi==3){vec=&dc->activeList144_Day4;V_Result=&dc->AnalysisResult144_Day4;}
        else if(pi==4){vec=&dc->activeList144_Day5;V_Result=&dc->AnalysisResult144_Day5;}
        else if(pi==5){vec=&dc->activeList144_Day6;V_Result=&dc->AnalysisResult144_Day6;}

        if(!V_Result->isEmpty())V_Result->clear();
        for(int i=0;i<vec->length();i++)
        {
            if(vec->at(i) >= FT5)V_Result->push_back(FT6);
            else if(vec->at(i) >= FT4)V_Result->push_back(FT5);
            else if(vec->at(i) >= FT3)V_Result->push_back(FT4);
            else if(vec->at(i) > FT2)V_Result->push_back(FT3);
            else if(vec->at(i) > FT1)V_Result->push_back(FT2);
            else V_Result->push_back(FT1);
        }
    }
}

//20分钟除杂(凹变平)
void DataAnalyze::getData_Remove_Noise_AfterFT()
{
    QVector<double> *vec = NULL;

    for(int pi=0;pi<chknum;pi++)
    {
             if(pi==0){vec=&dc->AnalysisResult144_Day1;}
        else if(pi==1){vec=&dc->AnalysisResult144_Day2;}
        else if(pi==2){vec=&dc->AnalysisResult144_Day3;}
        else if(pi==3){vec=&dc->AnalysisResult144_Day4;}
        else if(pi==4){vec=&dc->AnalysisResult144_Day5;}
        else if(pi==5){vec=&dc->AnalysisResult144_Day6;}

        for(int i=1;i<vec->length()-1;i++)
        {
            int XX=vec->at(i-1)-vec->at(i+1);

            if(XX==FT1 && vec->at(i)<vec->at(i-1))//0
                (*vec)[i]=vec->at(i-1);

            if(XX==FT2 && vec->at(i)<vec->at(i+1))//80(一级)
                (*vec)[i]=vec->at(i+1);

            if(XX==-1*FT2 && vec->at(i)<vec->at(i-1))//-80(一级)
                (*vec)[i]=vec->at(i-1);
        }
    }
}


//活动拔高
void DataAnalyze::getData_Divide_Status_AfterRN()
{
    QVector<double> *vec = NULL;

    for(int pi=0;pi<chknum;pi++)
    {
             if(pi==0){vec=&dc->AnalysisResult144_Day1;}
        else if(pi==1){vec=&dc->AnalysisResult144_Day2;}
        else if(pi==2){vec=&dc->AnalysisResult144_Day3;}
        else if(pi==3){vec=&dc->AnalysisResult144_Day4;}
        else if(pi==4){vec=&dc->AnalysisResult144_Day5;}
        else if(pi==5){vec=&dc->AnalysisResult144_Day6;}

        for(int i=0;i<vec->length();i++)
        {
            if(vec->at(i)>FT2)(*vec)[i]=FT6;
        }
    }

}


//30分钟 削平，高低低高
void DataAnalyze::getData_Remove_Noise_AfterDS()
{

    QVector<double> *vec = NULL;

    for(int pi=0;pi<chknum;pi++)
    {
             if(pi==0){vec=&dc->AnalysisResult144_Day1;}
        else if(pi==1){vec=&dc->AnalysisResult144_Day2;}
        else if(pi==2){vec=&dc->AnalysisResult144_Day3;}
        else if(pi==3){vec=&dc->AnalysisResult144_Day4;}
        else if(pi==4){vec=&dc->AnalysisResult144_Day5;}
        else if(pi==5){vec=&dc->AnalysisResult144_Day6;}

        for(int i=0;i<vec->length()-3;i++)
        {
            if(vec->at(i)==FT6)
            {
                //下两个是睡状态
                if(vec->at(i+1) <FT6 && vec->at(i+2)  < FT6 && vec->at(i+3)  == FT6)
                {
                    (*vec)[i+1]=FT6;
                    (*vec)[i+2]=FT6;
                }
            }
        }

    }
    //跨文件夹处理

}

void DataAnalyze::getData_Remove_Noise_AfterDSRN()
{
    QVector<double> *vec = NULL;

    for(int pi=0;pi<chknum;pi++)
    {
        if(pi==0){vec=&dc->AnalysisResult144_Day1;}
   else if(pi==1){vec=&dc->AnalysisResult144_Day2;}
   else if(pi==2){vec=&dc->AnalysisResult144_Day3;}
   else if(pi==3){vec=&dc->AnalysisResult144_Day4;}
   else if(pi==4){vec=&dc->AnalysisResult144_Day5;}
   else if(pi==5){vec=&dc->AnalysisResult144_Day6;}

        for(int i=0;i<vec->length()-4;i++)
        {
            if(vec->at(i)==FT6)
            {
                //下三个是睡状态
                if(vec->at(i+1) <FT6 && vec->at(i+2)  < FT6 && vec->at(i+3) < FT6 && vec->at(i+4) == FT6)
                {
                    (*vec)[i+1]=FT6;
                    (*vec)[i+2]=FT6;
                    (*vec)[i+3]=FT6;
                }
            }
        }
    }
}

void DataAnalyze::getData_Divide_SleepActive_AfterDS()
{

    QVector<double> *vec=NULL;
    QVector<double>* V_Deep=NULL;
    QVector<double>* V_shadow=NULL;

    for(int pi=0;pi<chknum;pi++)
    {
             if(pi==0){vec=&dc->AnalysisResult144_Day1;V_Deep=&dc->deepSleep144_Day1;V_shadow=&dc->shallowSleep144_Day1;}
        else if(pi==1){vec=&dc->AnalysisResult144_Day2;V_Deep=&dc->deepSleep144_Day2;V_shadow=&dc->shallowSleep144_Day2;}
        else if(pi==2){vec=&dc->AnalysisResult144_Day3;V_Deep=&dc->deepSleep144_Day3;V_shadow=&dc->shallowSleep144_Day3;}
        else if(pi==3){vec=&dc->AnalysisResult144_Day4;V_Deep=&dc->deepSleep144_Day4;V_shadow=&dc->shallowSleep144_Day4;}
        else if(pi==4){vec=&dc->AnalysisResult144_Day5;V_Deep=&dc->deepSleep144_Day5;V_shadow=&dc->shallowSleep144_Day5;}
        else if(pi==5){vec=&dc->AnalysisResult144_Day6;V_Deep=&dc->deepSleep144_Day6;V_shadow=&dc->shallowSleep144_Day6;}

        if(!V_Deep->isEmpty())V_Deep->clear();
        if(!V_shadow->isEmpty())V_Deep->clear();
        //vec 144
        for(int i=0;i<vec->length();i++)
        {
            if(vec->at(i)==FT6)//运动（两边的深睡眠受影响）
            {
                V_Deep->append(0);
                V_shadow->append(0);

            }
            if(vec->at(i)==FT2)//浅睡眠（两边的深睡眠受影响）
            {
                V_Deep->append(0);
                V_shadow->append(500);

            }
            if(vec->at(i)==FT1)//深睡眠
            {
                if(i>=1&&i<vec->length()-1)
                {
                    if(vec->at(i-1) > FT1 || vec->at(i+1)>FT1)
                    {
                        V_Deep->append(0);
                        V_shadow->append(500);
                    }
                    else
                    {
                        V_Deep->append(500);
                        V_shadow->append(0);

                    }
                }
                else
                {
                    V_Deep->append(500);
                    V_shadow->append(0);

                }

            }


        }
    }

}

void DataAnalyze::getMarkData_FromDeepAndShalow()
{
    struct_Duan tem;
    int duanFlag=0;
    int len =2880;              //传来的数据是144 *20
    //int border=DUAN_BORDER;//27-9
    double rate = 1.0*(dc->MarkBarWidth-DUAN_BORDER*2)/len;

    QVector<double>*V_144 =NULL;
    QVector<struct_Duan>*V_duan =NULL;
    for(int pi=0;pi<chknum;pi++)
    {
            if(pi==0){V_144=&dc->AnalysisResult144_Day1;V_duan=&dc->DUAN1;}
       else if(pi==1){V_144=&dc->AnalysisResult144_Day2;V_duan=&dc->DUAN2;}
       else if(pi==2){V_144=&dc->AnalysisResult144_Day3;V_duan=&dc->DUAN3;}
       else if(pi==3){V_144=&dc->AnalysisResult144_Day4;V_duan=&dc->DUAN4;}
       else if(pi==4){V_144=&dc->AnalysisResult144_Day5;V_duan=&dc->DUAN5;}
       else if(pi==5){V_144=&dc->AnalysisResult144_Day6;V_duan=&dc->DUAN6;}


        qDebug()<<"-----"<<V_144->length();
        //---------位置对齐------------
        for(int i=0;i<len;i++)//0~143
        {
            if((*V_144)[i/20]==FT6 )//活动
            {
                if(duanFlag==0)//一段未开始
                {
                    qDebug()<<">>>>>>>>"<<i/20<<"<<<<<<<找到活动起始点400";
                    duanFlag=1;//一段现在开始
                    tem.startX=i*rate+DUAN_BORDER;
                }
                else //一段已经开始
                {
                    if(i==len-1)//走到结尾 //仍然是活动状态
                    {
                        qDebug()<<">>>>>>>>"<<i/20<<"<<<<<<<结尾400";
                        duanFlag=0;//结束一段
                        tem.endX=(i)*rate+DUAN_BORDER;
                        V_duan->push_back(tem);
                    }
                }

            }
            else //睡
            {
                if(duanFlag==1)//一段已经开始
                {
                    qDebug()<<">>>>>>>>"<<i/20<<"<<<<<<<找到活动结束点";
                    duanFlag=0;//结束一段
                    tem.endX=(i-1)*rate+DUAN_BORDER;//取前一次
                    V_duan->push_back(tem);
                }

            }

        }
        tem.startX=0;
        tem.endX=0;
        V_duan->push_back(tem);//这里需要挤一下，否者最后一个不会被放进去
      }

}
void DataAnalyze::getData_DUANs()
{
    //先清空所有
    qDebug()<<"test 先清空所有";
    if(!dc->DUAN1.isEmpty())dc->DUAN1.clear();
    if(!dc->DUAN2.isEmpty())dc->DUAN2.clear();
    if(!dc->DUAN3.isEmpty())dc->DUAN3.clear();
    if(!dc->DUAN4.isEmpty())dc->DUAN4.clear();
    if(!dc->DUAN5.isEmpty())dc->DUAN5.clear();
    if(!dc->DUAN6.isEmpty())dc->DUAN6.clear();

    //再部分赋值
    qDebug()<<"test 再部分赋值";
    getMarkData_FromDeepAndShalow();


}

void DataAnalyze::startAnalyze()
{
    emit work_TotalNumber(7);

    qDebug()<<"--->>模糊6梯度";
    getData_FuzzificationTable();
    emit work_Progress_int(1);
    qDebug()<<"--->>凹变平";
    getData_Remove_Noise_AfterFT();//20分钟去杂，标准化
    emit work_Progress_int(1);
    qDebug()<<"--->>活动拔高";
    getData_Divide_Status_AfterRN();//三值化
    emit work_Progress_int(1);
    qDebug()<<"--->>除两低";
    getData_Remove_Noise_AfterDS();//30分钟去杂，标准化
    emit work_Progress_int(1);
    qDebug()<<"--->>除三低";
    getData_Remove_Noise_AfterDSRN();//40分钟去杂
    emit work_Progress_int(1);
    qDebug()<<"--->>得到深浅睡眠";
    getData_Divide_SleepActive_AfterDS();//区别睡觉和活动的时间域
    emit work_Progress_int(1);
    qDebug()<<"--->>得到标记数据";
    getData_DUANs();
    emit work_Progress_int(1);

    emit work_over();
}
