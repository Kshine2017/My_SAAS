#include "obtaindevicemeasuredata.h"
#include <QDebug>
#include <QDir>
#include <QFileInfoList>
#include <QMessageBox>
#include <QThread>

ObtainDeviceMeasureData::ObtainDeviceMeasureData(const QString& UserName, QWidget *parent):QObject(parent)
{
    thisparent=parent;

    //TODir = "d:\\Active_Data\\"+UserName;
    TODir = "./Active_Data/"+UserName;
}

ObtainDeviceMeasureData::~ObtainDeviceMeasureData()
{
    qDebug()<<"析构---复制文件";
}

void ObtainDeviceMeasureData::start()
{

}

void ObtainDeviceMeasureData::startCOPYFileFromDevice()
{
    if(SourceDir == "")
    {
        emit work_message("请先插入设备!");//QMessageBox::information(this,"提示","请先插入设备!",QMessageBox::Ok);
        emit work_over();
        return;
    }
    qDebug()<<"目标大小："<<getDirSize(SourceDir)/1024.0/1024.0;
    emit work_TotalSize(getDirSize(SourceDir));
    if(copyDirectoryFiles(SourceDir,TODir,false))
        emit work_message("设备数据上传完成!");//QMessageBox::information(this,"提示","设备数据上传完成!",QMessageBox::Ok);
    else
        emit work_message("设备数据上传失败!");

    emit work_over();
}

qint64 ObtainDeviceMeasureData::getDirSize(const QString &targetDir)
{
    QDir sourceDir(targetDir);
    qint64 sizzzze=0;

    if(!sourceDir.exists())     /**< 如果目标目录不存在，则进行创建 */
    {
            return sizzzze;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir())
        {
            sizzzze += getDirSize(fileInfo.filePath());
        }
        else
        {
            sizzzze+=fileInfo.size();
           // qDebug()<<fileInfo.fileName()<<fileInfo.size();
            //emit work_Progress(fileInfo.size());
        }
    }
    return sizzzze;

}

bool ObtainDeviceMeasureData::deleteDir(const QString &delPath)
{
    if (delPath.isEmpty())
    {
        return false;
    }
    QDir dir(delPath);
    if(!dir.exists()){
        return true;
    }
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤

    QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
    foreach (QFileInfo file, fileList)//遍历文件信息
    {
        if (file.isFile()){ // 是文件，删除
            file.dir().remove(file.fileName());
        }else{ // 递归删除
            deleteDir(file.absoluteFilePath());
        }
    }

    return dir.rmpath(dir.absolutePath()); // 删除文件夹
}

bool ObtainDeviceMeasureData::deleteDirExceptDelPath(const QString &delPath, int Zero)
{
    Zero++;
    if (delPath.isEmpty())
    {

        return false;
    }
    QDir dir(delPath);
    if(!dir.exists()){
        qDebug()<<"#未找到目标文件夹";
        return true;
    }
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot ); //设置过滤

    QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
    foreach (QFileInfo file, fileList)//遍历文件信息
    {
        if (file.isFile()){ // 是文件，删除
            file.dir().remove(file.fileName());
        }else{ // 递归删除
            deleteDirExceptDelPath(file.absoluteFilePath(),Zero);
        }
    }

    if(dir.exists())qDebug()<<"目前---目录存在："<<dir.absolutePath()<<"---------";

    QString tem = delPath;tem.replace("\\","/");
    //        qDebug()<<"#文件夹---第"<<Zero<<"层---|";
    //        qDebug()<<"dir.absolutePath()"<<dir.absolutePath();
    //        qDebug()<<"tem"<<tem;
    //if(tem != dir.absolutePath())//不是：ActiveData文件夹

    if(Zero>1)
    {
        qDebug()<<"删除目录"<<dir.absolutePath()<<endl;
        return dir.rmpath(dir.absolutePath()); // 删除文件夹
    }
    qDebug()<<"保留目录"<<dir.absolutePath();
    //检测是否保留成功（实践发现，会被删除）
    if(!dir.exists())//如果当前层 的文件夹被删除，则创建
    {
        qDebug()<<"目录丢失"<<dir.absolutePath()<<endl;
        dir.mkdir(dir.absolutePath());
    }
    else
    {
        qDebug()<<"目录存在"<<dir.absolutePath()<<endl;
    }
    return true;//保留ActiveData文件夹
}

bool ObtainDeviceMeasureData::copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist)
{
    //sourceDir.replace("/","\\");
    toDir.replace("\\","/");
    if (sourceDir == toDir){
        return true;
    }
    if (!QFile::exists(sourceDir)){
        return false;
    }
    QDir *createfile     = new QDir;
    bool exist = createfile->exists(toDir);
    if (exist){
        if(coverFileIfExist){
            createfile->remove(toDir);
        }
    }//end if

    if(!QFile::copy(sourceDir, toDir))
    {
        return false;
    }
    return true;

}

bool ObtainDeviceMeasureData::copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);

    if(!targetDir.exists())     /**< 如果目标目录不存在，则进行创建 */
    {
        if(!targetDir.mkdir(targetDir.absolutePath()))
        {
            emit work_message("目标对象："+toDir+" 文件夹创建失败！");
            return false;
        }
        qDebug()<<"创建完成";
    }
    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir()){    /**< 当为目录时，递归的进行copy */
            if(!copyDirectoryFiles(fileInfo.filePath(),
                                   targetDir.filePath(fileInfo.fileName()),
                                   coverFileIfExist))
            {

                return false;
            }
        }
        else
        {            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
            if(coverFileIfExist && targetDir.exists(fileInfo.fileName()))
            {
                targetDir.remove(fileInfo.fileName());
            }
            /// 进行文件copy
            QString ttarget =targetDir.filePath(fileInfo.fileName());
            if(!QFile::exists(ttarget))//如果文件不存在，才进行复制
            if(!QFile::copy(fileInfo.filePath(),ttarget))
            {
                work_message("文件："+fileInfo.fileName()+"拷贝失败");
                qDebug()<<"文件拷贝失败"<<fileInfo.filePath()<<targetDir.filePath(fileInfo.fileName());
                return false;
            }
           // qDebug()<<fileInfo.fileName()<<fileInfo.size();
            emit work_Progress(fileInfo.size());
        }
    }
    return true;
}
