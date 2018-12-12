#include "restoredevicedefaultdata.h"
#include <QDebug>

#include <QDir>
#include <QFileInfoList>
#include <QMessageBox>
#include <QThread>
RestoreDeviceDefaultData::RestoreDeviceDefaultData(QWidget *parent):QObject(parent)
{
    thisparent=parent;
    SourceDir ="./DefaultDataDir";
    //TODir=".\/DefaultDataDir";
    //QDir dir(".");
    //qDebug()<<"创建默认初始数据文件夹"<<dir.mkdir(SourceDir);
}

void RestoreDeviceDefaultData::start()
{
    //打开线程
    //执行删除
    // startDeleteDeviceData();
    //执行复制粘贴
    //startWriteInEmptyFiles();

    //
    startCOPYFileinSecondDir();

}

void RestoreDeviceDefaultData::startDeleteDeviceData()
{
    qDebug()<<"开始删除设备数据";
    //获取设备地址"D:\\Active_Data"
    //QString delPath = "D:\\Active_Data";
    QString delPath = "./Active_Data";
    //删除
    deleteDirExceptDelPath(delPath,0);
    //复制粘贴
}

void RestoreDeviceDefaultData::startWriteInEmptyFiles()
{
    qDebug()<<"开始写入空数据文件";
    qDebug()<<"复制文件夹："<<copyDirectoryFiles(SourceDir,TODir,true);
}

void RestoreDeviceDefaultData::startCOPYFileinSecondDir()
{
    //得到所有的子文件夹 2018-0101 2018-0102.。。。
    //设备地址ToDir  U:/Active_Data
    qDebug()<<"设备一级地址"<<TODir;
    if (TODir.isEmpty())
    {
        qDebug()<<"未插入设备";
        emit work_message("未插入设备!");//QMessageBox::information(thisparent,"提示","未插入设备!",QMessageBox::Ok);
        emit work_over();//结束工作
        return ;
    }
    QDir dir(TODir);//设备地址
    if(!dir.exists())
    {
        qDebug()<<"设备中无指定的文件夹";

        if(!dir.mkdir(dir.absolutePath()))
        {
            qDebug()<<"创建失败";
            emit work_message("设备中无指定的文件夹!");//QMessageBox::information(thisparent,"提示","设备中无指定的文件夹!",QMessageBox::Ok);
            emit work_over();//结束工作
            return ;
        }
        qDebug()<<"创建完成";
    }
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
    QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
    //如果没有子文件夹手动创建文件夹
    for(int i=fileList.length();i<6;i++)
    {

        QDir datedir(TODir+"\\2000-00-0"+QString::number(i));//设备地址
        if(!datedir.mkdir(datedir.absolutePath()))
        {
                qDebug()<<"创建子文件失败";
                emit work_message("创建子文件失败!");//QMessageBox::information(thisparent,"提示","创建子文件失败!",QMessageBox::Ok);
                emit work_over();//结束工作
                return ;
        }

    }
    fileList = dir.entryInfoList(); // 获取所有的文件信息
    qDebug()<<"子文件夹个数："<<fileList.length();

    QDir SDir(SourceDir);
    SDir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList SList = SDir.entryInfoList();
    emit work_Progress(0);//0%
    foreach (QFileInfo file, fileList)//遍历文件信息
    {
        if (file.isFile())      // 是文件
        {
            qDebug()<<"File:"<<file.absoluteFilePath();
        }
        else if(file.isDir())   //是文件夹
        {

                qDebug()<<"Diretory:"<<file.absoluteFilePath();
                //将文件复制到 file.absoluteFilePath()
                 foreach (QFileInfo Sfile, SList)//遍历源文件（程序内的文件）
                 {
                     bool ret= copyFileToPath(Sfile.absoluteFilePath(), file.absoluteFilePath()+"/"+Sfile.fileName(), true);
                     qDebug()<<"---->file:"<<Sfile.fileName()<<ret;
                     emit work_Progress(1);//成功复制一个文件
                     if(ret)
                        emit work_message(QString("---->file:")+Sfile.fileName()+QString("OK"));
                     else
                        emit work_message(QString("---->file:")+Sfile.fileName()+QString("Failed"));
                 }
        }
        else                    //其他类型
        {
            qDebug()<<"OtherType:"<<file.absoluteFilePath();
        }


    }






    emit work_Progress(100);//发送一百 表示完成
    emit work_message("还原设备数据文件成功!");//QMessageBox::information(thisparent,"成功","还原设备数据文件成功!",QMessageBox::Ok);
    emit work_over();//结束工作
}

//复制文件到指定位置
bool RestoreDeviceDefaultData::copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist)
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

//复制文件夹
bool RestoreDeviceDefaultData::copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);

    if(!targetDir.exists())     /**< 如果目标目录不存在，则进行创建 */
    {
        if(!targetDir.mkdir(targetDir.absolutePath()))
            return false;
        qDebug()<<"创建完成";
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir()){    /**< 当为目录时，递归的进行copy */
            if(!copyDirectoryFiles(fileInfo.filePath(),
                                   targetDir.filePath(fileInfo.fileName()),
                                   coverFileIfExist))
                return false;
        }
        else{            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
            if(coverFileIfExist && targetDir.exists(fileInfo.fileName())){
                //targetDir.remove(fileInfo.fileName());
            }
            /// 进行文件copy
            if(!QFile::copy(fileInfo.filePath(),
                            targetDir.filePath(fileInfo.fileName()))){
                return false;
            }
        }
    }
    return true;

}

bool RestoreDeviceDefaultData::deleteDir(const QString &delPath)
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

bool RestoreDeviceDefaultData::deleteDirExceptDelPath(const QString &delPath,int Zero)
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
