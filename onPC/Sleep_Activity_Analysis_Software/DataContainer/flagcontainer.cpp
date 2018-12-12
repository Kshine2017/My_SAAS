#include "flagcontainer.h"


FlagContainer* FlagContainer::instance=0;
FlagContainer *FlagContainer::getInstance()
{
    if(instance==0)
    {
        instance= new FlagContainer();
    }
    return instance;

}

FlagContainer::~FlagContainer()
{
    //delete instance;
}

FlagContainer::FlagContainer(QObject *parent) : QObject(parent)
{

}
