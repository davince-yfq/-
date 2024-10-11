#include "chat.h"
#include "ui_chat.h"
Chat::Chat(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
    myw = new Weather;
    slotOpenSerialPort();

}

Chat::~Chat()
{
    delete ui;
}

void Chat::slotLoadSerialPort()
{
    QStringList m_serialPortName;
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()) //获取有效的串口列表
    {
        m_serialPortName << info.portName();
        qWarning()<<"serialPortName:"<<info.portName();
    }
}

void Chat::slotCloseSerialPort()
{
    if (m_serialPort)
        m_serialPort->close();
}

void Chat::slotSend(QString data)
{
    if (m_serialPort)
       {
           if (!m_serialPort->isOpen())
               qWarning()<<"请打开一个串口";

           //QString info = "hello";
           QByteArray sendBuf;
           //把字符串转为字节
           sendBuf.append(data);// = info.toLocal8Bit();
           m_serialPort->write(sendBuf);
       }
}

void Chat::slotOpenSerialPort()
{
    m_serialPort = new QSerialPort(this);//新建一个窗口对象
    if(m_serialPort->isOpen())//判断对象是否已经打开，如果是就关闭
    {
        m_serialPort->clear();
        m_serialPort->close();
    }
    //设置串口名字
    m_serialPort->setPortName("COM1");
    //打开串口
    if(!m_serialPort->open(QIODevice::ReadWrite))
    {
        return;
    }
    m_serialPort->setBaudRate(9600);//波特率
    m_serialPort->setDataBits(QSerialPort::Data8);//数据为
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    m_serialPort->setParity(QSerialPort::NoParity);//奇偶校验位
    m_serialPort->setStopBits(QSerialPort::OneStop);//停止位

    //把串口的可读信号和槽函数链接
    connect(m_serialPort,SIGNAL(readyRead()),this,SLOT(slotReadData()));
}

void Chat::on_pushButton_clicked()
{
    myw->show();
}

void Chat::slotReadData()
{
    QString data = m_serialPort->readAll();
    qWarning()<<"接收数据："<<data;
}

void Chat::on_pushButton_2_clicked()
{
    slotSend("打开灯光");
}

void Chat::on_pushButton_4_clicked()
{
    slotSend("关闭灯光");
}

void Chat::on_pushButton_5_clicked()
{
    slotSend("打开风扇");
}

void Chat::on_pushButton_6_clicked()
{
    slotSend("关闭风扇");
}

void Chat::on_pushButton_3_clicked()
{
    slotSend("打开空调");
}

void Chat::on_pushButton_7_clicked()
{
    slotSend("关闭空调");
}
