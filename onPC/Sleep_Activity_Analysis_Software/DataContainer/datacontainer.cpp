#include "datacontainer.h"
#include <QDebug>
DataContainer* DataContainer::instance=0;

DataContainer::DataContainer(QObject *parent):QObject(parent)
{
    ChooseNUM=0;
    MarkBarWidth=0;
    KFlag_PDF=0;
}

DataContainer *DataContainer::getInstance()
{
    if(instance==0)
        instance=new DataContainer();
    return instance;
}

void DataContainer::releaseContainers()
{
    delete instance;
    instance=0;
}

DataContainer::~DataContainer()
{
    qDebug()<<"释放数据容器存储空间";
}
