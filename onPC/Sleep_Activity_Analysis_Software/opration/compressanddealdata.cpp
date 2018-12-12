#include "compressanddealdata.h"
#include <QDebug>

#define DrawTimeIGNORE 30
CompressAndDealData::CompressAndDealData(QObject *parent) : QObject(parent)
{
    dc = DataContainer::getInstance();
    fc = FlagContainer::getInstance();
    CheckNum=dc->ChooseNUM;

}

void CompressAndDealData::start()
{
    emit work_TotalNumber(CheckNum*4); //4代表  活动2880 光强2880 声音2880 活动144
    start_getDrawData();
    start_getAnalyzeData();
    emit work_over();
}

#define OLength     86400
#define PowerOFFDataA    360
#define PowerOFFDataL    360
#define PowerOFFDataV    360
void CompressAndDealData::start_dealAllFlag(int DAYNUM)
{
    int KshineS=DAYNUM*OLength;
    DAYNUM++;
    int KshineE=DAYNUM*OLength;

    memset(tem,0,OLength);
    //处理全部的标志位，得到可操作容器映射位
    //1、开关机
    for(int i=0;i<fc->VPower.length();i++)
    {
        int S=fc->VPower[i].start;
        int E=fc->VPower[i].end;
        if(S>=E)continue;//正常不会出现（前文逻辑已排除，此处二重防护）
        if(S>=KshineE||E<KshineS)continue; //DAYNUM==0时，S和E需要在0~86399中
        else
        {
            if(E>KshineE)E=KshineE;
            if(S<KshineS)S=KshineS;
        }
        for(int j=S;j<E;j++)
        {
            tem[j%OLength]=1;
        }
    }
    //2、睡眠唤醒
    for(int i=0;i<fc->VSleep.length();i++)
    {
        int S=fc->VSleep[i].start;
        int E=fc->VSleep[i].end;
        if(S>=E)continue;//正常不会出现（前文逻辑已排除，此处二重防护）
        if(S>=KshineE||E<KshineS)continue; //DAYNUM==0时，S和E需要在0~86399中
        else
        {
            if(E>KshineE)E=KshineE;
            if(S<KshineS)S=KshineS;
        }
        for(int j=S;j<E;j++)
        {
            tem[j%OLength]=0;
        }
    }
    //3、USB插拔
    //略

}

void CompressAndDealData::start_getDrawData()
{
    if(CheckNum==0)
    {
        return;//无被选中的，则不需要压缩
    }
    QVector<double>* V_originalData=NULL;
    QVector<double>* V_Result=NULL;

    qDebug()<<"-->>活动数据 30s压缩";
    //活动数据 30s压缩
    for(int i=0;i<CheckNum;i++)
    {
        start_dealAllFlag(i);

        //指定操作的容器
        if(i==0){V_originalData=&dc->activeList86400_Day1;V_Result=&dc->activeList2880_Day1;}
        else if(i==1){V_originalData=&dc->activeList86400_Day2;V_Result=&dc->activeList2880_Day2;}
            else if(i==2){V_originalData=&dc->activeList86400_Day3;V_Result=&dc->activeList2880_Day3;}
                else if(i==3){V_originalData=&dc->activeList86400_Day4;V_Result=&dc->activeList2880_Day4;}
                    else if(i==4){V_originalData=&dc->activeList86400_Day5;V_Result=&dc->activeList2880_Day5;}
                        else if(i==5){V_originalData=&dc->activeList86400_Day6;V_Result=&dc->activeList2880_Day6;}

        //开始操作----------------------
        if(!V_Result->isEmpty())V_Result->clear();
        for (int i = 0; i < V_originalData->length(); i++)
        {
            if (i % DrawTimeIGNORE == 0)//0(0~29) 30(30~59)
            {
                double temp = 0;
                for (int j = 0; j < DrawTimeIGNORE; j++)
                {
                    if(tem[i+j])
                        temp += V_originalData->at(i+j);
                    else
                        temp += PowerOFFDataA;
                }
                temp = temp / DrawTimeIGNORE;
                V_Result->push_back(temp);
            }
        }
        qDebug()<<"i=="<<i<<V_originalData<<V_Result<<V_Result->length();
        emit work_Progress_int(1);

    }

    qDebug()<<"-->>光强数据30s压缩";
    //光强数据30s压缩
    for(int i=0;i<CheckNum;i++)
    {
        //指定操作的容器
        if(i==0){V_originalData=&dc->lightList86400_Day1;V_Result=&dc->lightList2880_Day1;}
        else if(i==1){V_originalData=&dc->lightList86400_Day2;V_Result=&dc->lightList2880_Day2;}
            else if(i==2){V_originalData=&dc->lightList86400_Day3;V_Result=&dc->lightList2880_Day3;}
                else if(i==3){V_originalData=&dc->lightList86400_Day4;V_Result=&dc->lightList2880_Day4;}
                    else if(i==4){V_originalData=&dc->lightList86400_Day5;V_Result=&dc->lightList2880_Day5;}
                        else if(i==5){V_originalData=&dc->lightList86400_Day6;V_Result=&dc->lightList2880_Day6;}

        //开始操作----------------------
        if(!V_Result->isEmpty())V_Result->clear();
        for (int i = 0; i < V_originalData->length(); i++)
        {
            if (i % DrawTimeIGNORE == 0)//0(0~29) 30(30~59)
            {
                double temp = 0;
                for (int j = 0; j < DrawTimeIGNORE; j++)
                {
                    if(tem[i+j])
                        temp += V_originalData->at(i+j);
                    else
                        temp += PowerOFFDataL;
                }
                temp = temp / DrawTimeIGNORE;
                V_Result->push_back(temp);
            }
        }
        qDebug()<<"i=="<<i<<V_originalData<<V_Result<<V_Result->length();
        emit work_Progress_int(1);
    }

    qDebug()<<"-->>声音数据30s压缩";
    //声音数据30s压缩
    for(int i=0;i<CheckNum;i++)
    {
        //指定操作的容器
        if(i==0){V_originalData=&dc->voiceList86400_Day1;V_Result=&dc->voiceList2880_Day1;}
        else if(i==1){V_originalData=&dc->voiceList86400_Day2;V_Result=&dc->voiceList2880_Day2;}
            else if(i==2){V_originalData=&dc->voiceList86400_Day3;V_Result=&dc->voiceList2880_Day3;}
                else if(i==3){V_originalData=&dc->voiceList86400_Day4;V_Result=&dc->voiceList2880_Day4;}
                    else if(i==4){V_originalData=&dc->voiceList86400_Day5;V_Result=&dc->voiceList2880_Day5;}
                        else if(i==5){V_originalData=&dc->voiceList86400_Day6;V_Result=&dc->voiceList2880_Day6;}


        //开始操作----------------------
        if(!V_Result->isEmpty())V_Result->clear();
        for (int i = 0; i < V_originalData->length(); i++)
        {
            if (i % DrawTimeIGNORE == 0)//0(0~29) 30(30~59)
            {
                double temp = 0;
                for (int j = 0; j < DrawTimeIGNORE; j++)
                {
                    if(tem[i+j])
                        temp += V_originalData->at(i+j);
                    else
                        temp += PowerOFFDataV;
                }
                temp = temp / DrawTimeIGNORE;
                V_Result->push_back(temp);
            }
        }
        qDebug()<<"i=="<<i<<V_originalData<<V_Result<<V_Result->length();
        emit work_Progress_int(1);
    }




}

void CompressAndDealData::start_getAnalyzeData()
{
    if(CheckNum==0)return;//无被选中的，则不需要压缩
    QVector<double> *vec_2880=NULL;
    QVector<double> *result144=NULL;//存放144个最大值

    qDebug()<<"-->>活动数据 再压缩";
//活动数据 再压缩（用于分析）
    for(int i=0;i<CheckNum;i++)
    {
        double datamax=0;
        //指定操作的容器
        if(i==0){result144=&dc->activeList144_Day1;vec_2880=&dc->activeList2880_Day1;}
        else if(i==1){result144=&dc->activeList144_Day2;vec_2880=&dc->activeList2880_Day2;}
            else if(i==2){result144=&dc->activeList144_Day3;vec_2880=&dc->activeList2880_Day3;}
                else if(i==3){result144=&dc->activeList144_Day4;vec_2880=&dc->activeList2880_Day4;}
                    else if(i==4){result144=&dc->activeList144_Day5;vec_2880=&dc->activeList2880_Day5;}
                        else if(i==5){result144=&dc->activeList144_Day6;vec_2880=&dc->activeList2880_Day6;}

        //开始操作----------------------
        if(!result144->isEmpty())result144->clear();
        for(int i=0; i< vec_2880->length() ;i++)
        {
            if(vec_2880->at(i)>datamax)datamax=vec_2880->at(i);
            if((i+1)%20 ==0)//20,40,60,80.....
            {
                result144->append(datamax);
                datamax=0;
            }
        }

        qDebug()<<"i=="<<i<<vec_2880<<result144<<result144->length();
        emit work_Progress_int(1);

    }

}

