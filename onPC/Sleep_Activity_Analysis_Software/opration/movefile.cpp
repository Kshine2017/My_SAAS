#include "movefile.h"
#include <QDebug>
#include <QDir>
#include <QFileInfoList>
#include <QMessageBox>
#include <QThread>
MoveFile::MoveFile(QObject *parent) : QObject(parent)
{

}

void MoveFile::movePDFtoDir()
{
    if(filename.isEmpty()||toWhere.isEmpty())
    {
        emit work_message("未指定文件或者目的地");
        qDebug()<<"未指定文件或者目的地";
        return;
    }
    QFile sfile(filename);
    if(!sfile.exists())
    {
        emit work_message("pdf文件不存在，无法复制");
        qDebug()<<"pdf文件不存在，无法复制"<<filename;
        return;
    }
    QDir ARDir(toWhere);
    if(!ARDir.exists())
    {
        if(!ARDir.mkdir(ARDir.absolutePath()))
        {
            emit work_message("avtive_report文件夹创建失败");
            qDebug()<<"avtive_report文件夹创建失败";
            return;
        }
    }
    QString target=toWhere+"/"+name;
    QDir TDIR(target);

    if(!TDIR.exists())
    {
        if(!TDIR.mkdir(TDIR.absolutePath()))
        {
            emit work_message("avtive_report/用户文件夹创建失败");
            qDebug()<<"avtive_report/用户文件夹创建失败"<<target;
            return;
        }
    }
    QString tfile=target+"/"+targetFilename;
    if(QFile::copy(filename,tfile))
    {
        return ;
    }
    else
    {
        emit work_message("复制失败");
        qDebug()<<"复制失败"<<filename<<tfile;
        return;
    }
}


bool MoveFile::PDFisExist()
{
    if(filename.isEmpty()||toWhere.isEmpty()||name.isEmpty()||targetFilename.isEmpty())
    {
        return false;
    }
    QString tfile=toWhere+"/"+name+"/"+targetFilename;
    QFile ttt(tfile);
    if(!ttt.exists())
    {
        return false;
    }
    return true;
}

bool MoveFile::PDFisExist_X()
{
    if(filename.isEmpty()||toWhere.isEmpty())
    {
        return false;
    }
    QString tfile=toWhere;
    QFile ttt(tfile);
    if(!ttt.exists())
    {
        return false;
    }
    return true;
}

bool MoveFile::deletePDF()
{
    QString tfile=toWhere+"/"+name+"/"+targetFilename;
    QFile ttt(tfile);
    if(ttt.exists())
    {
        ttt.remove();
    }
    return true;
}

bool MoveFile::deletePDF_X()
{
    QString tfile=toWhere;
     QFile ttt(tfile);
     if(ttt.exists())
     {
         ttt.remove();
     }
     return true;

}

void MoveFile::outputPDF()
{
    if(QFile::copy(filename,toWhere))
    {
        return ;
    }
    else
    {
        emit work_message("复制失败");
        qDebug()<<"复制失败"<<filename<<toWhere;
        return;
    }
}

