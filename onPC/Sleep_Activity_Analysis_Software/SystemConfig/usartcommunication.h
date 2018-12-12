#ifndef USARTCOMMUNICATION_H
#define USARTCOMMUNICATION_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QLCDNumber>
#include <QLineEdit>
#include <QTimer>
#include <QTextEdit>
#include <QtSerialPort/QSerialPort>
class UsartCommunication : public QDialog
{
    Q_OBJECT
public:
    explicit UsartCommunication(QWidget *parent = 0);
    QLabel * lb_background;

    QSerialPort serial;			//声明串口类
    //串口连接----------------------
    QLabel* lb_titleUsart;
    QLabel* lb_COM;         QComboBox* cmbx_COM;
    QLabel* lb_BaudRate;    QComboBox* cmbx_BaudRate;
    QLabel* lb_ParityBit;   QComboBox* cmbx_ParityBit;
    QLabel* lb_DataBit;     QComboBox* cmbx_DataBit;
    QLabel* lb_StopBit;     QComboBox* cmbx_StopBit;
    QPushButton* btn_Connect;
    QTextEdit* textEdit;
    void init_UsartPart();

    //时间矫正-----------------------
    QLabel * lb_titleTime;
    QLCDNumber* lcd_time;    
    QPushButton* btn_SendTimeCommand;
    QTimer* timer;
    QTimer* timer_ACK;
    void init_TimePart();


    //设备号-----------------------
    QLabel * lb_titleDeviceNumber;
    QLabel* lb_DeviceNumberRegulation;//提示性文字，说明版本号的命名规则
    QLineEdit* le_DeviceNumber;
    QPushButton* btn_SendDeviceCommand;
    void init_DeviceNumberPart();

    //----------------------------
    QLineEdit* le_Command;
    QPushButton* btn_Send;
    QPushButton* btn_Clear;

    void init_ALLlayout();

signals:

public slots:
    void slot_timer_out();
    void slot_timer_out_ACK();
    void slot_btn_connect();
    void Read_COM_DATA();
    void slot_btn_sendTimeCommand();
    void slot_btn_send();
};

#endif // USARTCOMMUNICATION_H
