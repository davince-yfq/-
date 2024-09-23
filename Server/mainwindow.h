#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include "comment.h"
#include "sqliteoperator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTcpServer *server;
    QTcpSocket *client;
    void acceptConneted();
    void onReayread();
    void ondisconnected();
    sqliteOperator *sqlOP;
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
