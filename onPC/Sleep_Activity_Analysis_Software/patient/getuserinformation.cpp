#include "getuserinformation.h"

#include <QDebug>
GetUserInformation::GetUserInformation(QWidget *parent):QDialog(parent)
{



}

GetUserInformation::~GetUserInformation()
{
    qDebug()<<"析构-getuserinfo";
}
