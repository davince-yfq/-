#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QTcpSocket>
#include <QPoint>
#include <QMouseEvent>
#include <QMessageBox>
#include "weather.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTcpSocket *clientSocket;
    void onConnected();
    void onreadyRead();

    QPoint dragPos;
      void mouseMoveEvent(QMouseEvent *event) override;
      void mousePressEvent(QMouseEvent *event) override;
      QMessageBox msgbox;
private slots:
    void on_btn_close_clicked();

    void on_btn_zxh_clicked();

    void on_btn_login_clicked();

    void on_btn_enroll_clicked();

private:
    Weather *wea;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
