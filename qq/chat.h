#ifndef CHAT_H
#define CHAT_H

#include <QMainWindow>
#include "weather.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
namespace Ui {
class Chat;
}

class Chat : public QMainWindow
{
    Q_OBJECT

public:
    explicit Chat(QWidget *parent = nullptr);
    ~Chat();


    void slotOpenSerialPort();
    void slotLoadSerialPort();
    void slotCloseSerialPort();
    void slotSend(QString data);
private slots:
    void on_pushButton_clicked();
    void slotReadData();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::Chat *ui;
    Weather *myw;
    QSerialPort * m_serialPort;
};

#endif // CHAT_H
