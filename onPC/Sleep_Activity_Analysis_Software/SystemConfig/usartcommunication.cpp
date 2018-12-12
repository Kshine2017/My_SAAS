#include "usartcommunication.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTime>
#include <QDebug>

#define PacketHead  "A5 10 A5 10"
#define PacketTail  "10 AB 10 AB"
#define CMD_SetTime "00 01"

UsartCommunication::UsartCommunication(QWidget *parent) : QDialog(parent)
{

    this->setFixedSize(800,600);
    this->setWindowModality(Qt::ApplicationModal);
    this->setAttribute(Qt::WA_DeleteOnClose);
    init_UsartPart();
    init_TimePart();
    init_DeviceNumberPart();
    init_ALLlayout();
    show();
    connect(&serial,SIGNAL(readyRead()),this,SLOT(Read_COM_DATA()));//信号-槽
}

void UsartCommunication::init_UsartPart()
{
    QStringList COM_List;       COM_List<<"COM1"<<"COM2"<<"COM3"<<"COM4"<<"COM5"<<"COM6"<<"COM7"<<"COM8"<<"COM9"<<"COM10"<<"COM11"<<"COM12"<<"COM13"<<"COM14"<<"COM15"<<"COM16";
    QStringList BaudRate_List;  BaudRate_List<<"115200"<<"57600"<<"56000"<<"38400"<<"19200"<<"14400"<<"9600"<<"4800";
    QStringList ParityBit_List; ParityBit_List<<"NONE"<<"ODD"<<"EVEN"<<"MARK"<<"SPACE";
    QStringList DataBit_List;   DataBit_List<<"8"<<"7"<<"6"<<"5";
    QStringList StopBit_List;   StopBit_List<<"1"<<"1.5"<<"2";
    lb_titleUsart   = new QLabel(this);     lb_titleUsart   ->setText("串口连接:");lb_titleUsart->setFont(QFont("宋体",16));
    lb_COM          = new QLabel(this);     lb_COM          ->setText("COM口:");lb_COM->setFixedWidth(35);
    cmbx_COM        = new QComboBox(this);  cmbx_COM->addItems(COM_List);
    lb_BaudRate     = new QLabel(this);     lb_BaudRate     ->setText("波特率:");lb_BaudRate->setFixedWidth(35);
    cmbx_BaudRate   = new QComboBox(this);  cmbx_BaudRate->addItems(BaudRate_List);
    lb_ParityBit    = new QLabel(this);     lb_ParityBit    ->setText("校验位:");lb_ParityBit->setFixedWidth(35);
    cmbx_ParityBit  = new QComboBox(this);  cmbx_ParityBit->addItems(ParityBit_List);
    lb_DataBit      = new QLabel(this);     lb_DataBit      ->setText("数据位:");lb_DataBit->setFixedWidth(35);
    cmbx_DataBit    = new QComboBox(this);  cmbx_DataBit->addItems(DataBit_List);
    lb_StopBit      = new QLabel(this);     lb_StopBit      ->setText("停止位:");lb_StopBit->setFixedWidth(35);
    cmbx_StopBit    = new QComboBox(this);  cmbx_StopBit->addItems(StopBit_List);
    btn_Connect     = new QPushButton(this);btn_Connect     ->setText("连接");
    textEdit        = new QTextEdit(this);


    connect(btn_Connect,SIGNAL(clicked(bool)),this,SLOT(slot_btn_connect()));
}

void UsartCommunication::init_TimePart()
{
    lb_titleTime        = new QLabel(this);     lb_titleTime->setText("设备时间：");lb_titleTime->setFont(QFont("宋体",16));
    lcd_time            = new QLCDNumber(this);lcd_time->setMinimumWidth(300);

    btn_SendTimeCommand = new QPushButton(this);btn_SendTimeCommand->setText("生成时间命令");

    lcd_time->setDigitCount(20);
    lcd_time->setMode(QLCDNumber::Dec);
    lcd_time->setSegmentStyle(QLCDNumber::Flat);
    lcd_time->setStyleSheet("border: 1px solid red; color: blue; background: silver;");

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(slot_timer_out()));
    connect(btn_SendTimeCommand,SIGNAL(clicked(bool)),this,SLOT(slot_btn_sendTimeCommand()));
    timer->start();

    timer_ACK = new QTimer(this);
    timer_ACK->setInterval(3000);
    connect(timer_ACK,SIGNAL(timeout()),this,SLOT(slot_timer_out_ACK()));
}

void UsartCommunication::init_DeviceNumberPart()
{
    lb_titleDeviceNumber        = new QLabel(this);lb_titleDeviceNumber->setText("设备号信息:");lb_titleDeviceNumber->setFont(QFont("宋体",16));
    lb_DeviceNumberRegulation   = new QLabel(this);lb_DeviceNumberRegulation->setText("设备号命名规则：如20180807003");
    le_DeviceNumber             = new QLineEdit(this) ;le_DeviceNumber->setPlaceholderText("设备号");le_DeviceNumber->setMaximumWidth(200);
    btn_SendDeviceCommand       = new QPushButton(this);btn_SendDeviceCommand->setText("生成设备号命令");
    //--------------------------------------------
    le_Command      = new QLineEdit(this);  le_Command->setPlaceholderText("命令");le_Command->setMinimumWidth(600);
    btn_Send                    = new QPushButton(this);btn_Send->setText("发送");
    connect(btn_Send,SIGNAL(clicked(bool)),this,SLOT(slot_btn_send()));
    btn_Clear                    = new QPushButton(this);btn_Clear->setText("清空");
}

void UsartCommunication::init_ALLlayout()
{
    QHBoxLayout* ly_H_Usart1;
    QHBoxLayout* ly_H_Usart2;
    QHBoxLayout* ly_H_Usart3;
    QHBoxLayout* ly_H_Time1;
    //QHBoxLayout* ly_H_Time2;
    QHBoxLayout* ly_H_Time3;
    QHBoxLayout* ly_H_DN1;
    QHBoxLayout* ly_H_DN2;
    QHBoxLayout* ly_H_DN3;

    QVBoxLayout* ly_V_ALL;
    QHBoxLayout* ly_H_quit;
    ly_H_Usart1 = new QHBoxLayout;
    ly_H_Usart1->addWidget(lb_titleUsart);ly_H_Usart1->addStretch();
    ly_H_Usart2 = new QHBoxLayout;
    ly_H_Usart2->addWidget(lb_COM);ly_H_Usart2->addWidget(cmbx_COM);
    ly_H_Usart2->addWidget(lb_BaudRate);ly_H_Usart2->addWidget(cmbx_BaudRate);
    ly_H_Usart2->addWidget(lb_ParityBit);ly_H_Usart2->addWidget(cmbx_ParityBit);
    ly_H_Usart2->addWidget(lb_DataBit);ly_H_Usart2->addWidget(cmbx_DataBit);
    ly_H_Usart2->addWidget(lb_StopBit);ly_H_Usart2->addWidget(cmbx_StopBit);
    ly_H_Usart2->addWidget(btn_Connect);//ly_H_Usart2->addStretch();
    ly_H_Usart3 = new QHBoxLayout;
    ly_H_Usart3->addWidget(textEdit);

    ly_H_Time1  = new QHBoxLayout;
    ly_H_Time1->addWidget(lb_titleTime);ly_H_Time1->addStretch();
//    ly_H_Time2  = new QHBoxLayout;
//    ly_H_Time2->addSpacing();
    ly_H_Time3  = new QHBoxLayout;
    ly_H_Time3->addWidget(lcd_time);ly_H_Time3->addWidget(btn_SendTimeCommand);ly_H_Time3->addStretch();


    ly_H_DN1    = new QHBoxLayout;
    ly_H_DN1->addWidget(lb_titleDeviceNumber);ly_H_DN1->addStretch();
    ly_H_DN2    = new QHBoxLayout;
    ly_H_DN2->addWidget(lb_DeviceNumberRegulation);ly_H_DN2->addStretch();
    ly_H_DN3    = new QHBoxLayout;
    ly_H_DN3->addWidget(le_DeviceNumber);ly_H_DN3->addWidget(btn_SendDeviceCommand);ly_H_DN3->addStretch();


    ly_H_quit = new QHBoxLayout;
    ly_H_quit->addWidget(le_Command);ly_H_quit->addWidget(btn_Send); ly_H_quit->addWidget(btn_Clear);ly_H_quit->addStretch();

    ly_V_ALL = new QVBoxLayout;
    ly_V_ALL->addLayout(ly_H_Usart1);
    ly_V_ALL->addLayout(ly_H_Usart2);
    ly_V_ALL->addLayout(ly_H_Usart3);

    ly_V_ALL->addLayout(ly_H_Time1);
    //ly_V_ALL->addLayout(ly_H_Time2);
    ly_V_ALL->addLayout(ly_H_Time3);

    ly_V_ALL->addLayout(ly_H_DN1);
    ly_V_ALL->addLayout(ly_H_DN2);
    ly_V_ALL->addLayout(ly_H_DN3);
    ly_V_ALL->addSpacing(80);
    ly_V_ALL->addLayout(ly_H_quit);

    this->setLayout(ly_V_ALL);
}


void UsartCommunication::slot_timer_out()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    lcd_time->display(dateTime.toString("yyyy-MM-dd HH:mm:ss"));//yyyy-MM-dd HH:mm:ss.zzz
}

void UsartCommunication::slot_timer_out_ACK()
{
    textEdit->append("未接收到设备反馈信息！");
    timer_ACK->stop();
}
QString ArrayTo_Hex_ToQstring(unsigned int array[],int len)
{
    QString tem;
    for(int i=0;i<len;i++)
    {
        char str[10]="";
        memset(str,0,10);
      //sprintf(str,"%X",array[i]&0x000000ff);
        sprintf(str,"%02X",array[i]&0x000000ff);
        QString qstr=QString(str);
        tem+=qstr;
        tem+=" ";
    }
    tem.chop(1);//删除最后一个空格
    return tem;
}
QString ArrayTo_Dec_ToQstring(unsigned int array[],int len)
{
    QString tem;
    for(int i=0;i<len;i++)
    {
        char str[10]="";
        memset(str,0,10);
        sprintf(str,"%03d",array[i]);
        QString qstr=QString(str);
        tem+=qstr;
        tem+=" ";
    }
    tem.chop(1);//删除最后一个空格
    return tem;
}
int QString2_to_Hex(QString tem)
{
   int ret=0;
   //zuo
   if(tem.length()<1)return 0;
   int a=tem.at(0).toLatin1();
   if(a>='A'&&a<='Z') //A~Z
   {                  //10 11 12 13 14 15
       ret+=(a-64+9)*16;    //1   2  3  4  5  6   +9
   }
   if(a>='a'&&a<='z') //a~z
   {
       ret+=(a-96+9)*16;
   }
   if(a>='0'&&a<='9')//0~9
   {
       ret+=(a-48)*16;
   }

   //you
   if(tem.length()<2)return 0;
   int b=tem.at(1).toLatin1();
   if(b>='A'&&a<='Z') //A~Z
   {                  //10 11 12 13 14 15
       ret+=(b-64+9)*1;    //1   2  3  4  5  6   +9
   }
   if(b>='a'&&b<='z') //a~z
   {
       ret+=(b-96+9)*1;
   }
   if(b>='0'&&b<='9')//0~9
   {
       ret+=(b-48)*1;
   }
   qDebug()<<tem<<"转10进制："<<ret;
   return ret;
}
void UsartCommunication::slot_btn_connect()
{
    if(cmbx_COM->isEnabled())
    {
        btn_Connect->setText("断开");                                         //按下“OpenPort”后，按键显示为“ClosePort”

        cmbx_COM->setDisabled(true);                                 //按下“OpenPort”后，禁止再修改COM口


        serial.setPortName(cmbx_COM->currentText());                 //设置COM口

        serial.setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);  //设置波特率和读写方向

        serial.setDataBits(QSerialPort::Data8);                                  //数据位为8位

        serial.setFlowControl(QSerialPort::NoFlowControl);                       //无流控制

        serial.setParity(QSerialPort::NoParity);                                 //无校验位

        serial.setStopBits(QSerialPort::OneStop);                                //一位停止位

        if(serial.open(QIODevice::ReadWrite))                                    //以可读写的方式打开串口
        {
            qDebug()<<"已经打开串口设备";
            textEdit->append("<span style=' color:#0000ff;'>已经打开串口设备</span>");
        }
        else
        {
            qDebug()<<"串口设备打开失败";
            textEdit->append("<span style=' color:#ff0000;'>串口设备打开失败</span>");
        }
    }
    else
    {
        btn_Connect->setText("连接");                                         //按下“ClosePort”后，按键显示为“OpenPort”
        cmbx_COM->setEnabled(true);                                  //按下“ClosePort”后，COM口可被修改
        serial.close();//关串口
        qDebug()<<"已经关闭串口设备";
        //textEdit->setHtml("<span style=' color:#ff0000;'>已经关闭串口设备</span>");
        textEdit->append("<span style=' color:#0000ff;'>已经关闭串口设备</span>");
        textEdit->append("--------------------------------------------");
    }
}

void UsartCommunication::Read_COM_DATA()
{
    QByteArray tem;
    QString sss;
    tem=serial.readAll();
    sss+=QString::fromLocal8Bit(tem.toStdString().c_str());

    textEdit->append(sss);
    qDebug()<<sss;
    if(sss.contains("Tell_ME_TIME")||sss.contains("Tell")||sss.contains("ME_TIME"))
    {
        textEdit->setText("设备寻求RTC时间");

        //自动发送数据
        QDateTime dateTime = QDateTime::currentDateTime();
        QString time=dateTime.toString("yyyy-MM-dd-hh-mm-ss");
        serial.write(time.toStdString().c_str(),(qint64)19);
    }
    //----------------------------------------
    timer_ACK->stop();





}
//生成命令
void UsartCommunication::slot_btn_sendTimeCommand()
{
    //得到时间
    QDateTime dateTime = QDateTime::currentDateTime();
    unsigned int* array = new unsigned int[7];
    array[0]=dateTime.toString("yyyy").toInt()/256;
    array[1]=dateTime.toString("yyyy").toInt()%256;
    array[2]=dateTime.toString("MM").toInt();
    array[3]=dateTime.toString("dd").toInt();
    array[4]=dateTime.toString("hh").toInt();
    array[5]=dateTime.toString("mm").toInt();
    array[6]=dateTime.toString("ss").toInt();
    QString sss=   ArrayTo_Hex_ToQstring(array,7);
    qDebug()<<sss;
    delete array;
    //--------------------------
    QString CRC = "00 00";


    //---------------------------
    QString text = QString(PacketHead)+" "+QString(CMD_SetTime)+" 00 07 "+sss+" "+CRC+" "+QString(PacketTail);

    le_Command->setText(text);

}

void UsartCommunication::slot_btn_send()
{

//    if(!serial.isOpen())
//    {
//        textEdit->append("串口未连接！");
//        return;
//    }
    if(le_Command->text().isEmpty())
    {
        textEdit->append("无命令！");
        return;
    }
    QStringList strlist;
    if(le_Command->text().contains(" "))
        strlist = le_Command->text().split(" ");
    else
        {
            textEdit->append("命令格式不正确！");
            return;
        }
    //qDebug()<<strlist;
    QByteArray BA ;
    for(int i=0;i<strlist.length();i++)
    {
        char c = (char)QString2_to_Hex(strlist[i]);
        BA.append(c);
    }
    //qDebug()<<BA;
    serial.write(BA);
    timer_ACK->start();

}
