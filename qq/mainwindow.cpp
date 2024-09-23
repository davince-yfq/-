#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "comment.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QPixmap pixmap(":/toux.jpg");
    ui->label->setStyleSheet("background-image:url(:/toux.png);border:0px solid black;border-radius:60px;");

    QFile file(":/style.qss");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString str=in.readAll();
    file.close();

    this->setStyleSheet(str);
    this->setWindowFlag(Qt::FramelessWindowHint);//去掉主窗口边框
    this->setAttribute(Qt::WA_TranslucentBackground);//把窗口设置为透明

    //建立服务端客户端链接
    clientSocket=new QTcpSocket(this);
    clientSocket->connectToHost("192.168.100.1",9000);
    connect(clientSocket,&QTcpSocket::connected,this,&MainWindow::onConnected);
    connect(clientSocket,&QTcpSocket::readyRead,this,&MainWindow::onreadyRead);

    //天气窗口初始化
    wea=new Weather;
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_close_clicked()
{
    this->close();
}

void MainWindow::on_btn_zxh_clicked()
{
    this->showMinimized();
}

void MainWindow::on_btn_login_clicked()
{
    Qmsg msg;
    msg.type=2;
    QString name=ui->line_name->text();
    std::string sname=name.toStdString();

    QString pwd=ui->line_pwd->text();
    std::string spwd=pwd.toStdString();

    rigister_node node(sname.c_str(),spwd.c_str());
    memcpy(msg.buf,&node,sizeof(node));
    clientSocket->write((char *)&msg,sizeof(msg));
    qDebug()<<"enpressed"<<endl;
}
void MainWindow::onConnected()
{
    QString data=ui->line_name->text();
    clientSocket->write(data.toStdString().c_str());
}

void MainWindow::onreadyRead()
{
    Qmsg msg;
    clientSocket->read((char *)&msg,sizeof(msg));
    if(msg.type==11)
    {
        msgbox.setText("注册失败！用户名已存在。");
        msgbox.show();
    }else if(msg.type==12)
    {
        msgbox.setText("注册成功！");
        msgbox.show();
    }
    else if(msg.type==21)
    {
        wea->show();
        this->hide();
    }
    else if(msg.type==22)
    {
        msgbox.setText("用户名或密码错误！");
        msgbox.show();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    // 如果按下了左键，则移动窗口
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPos);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    // 当左键按下时记录当前位置
    if (event->button() == Qt::LeftButton) {
        dragPos = event->globalPos() - frameGeometry().topLeft();
    }
}

void MainWindow::on_btn_enroll_clicked()
{
    Qmsg msg;
    msg.type=1;
    QString name=ui->line_name->text();
    std::string sname=name.toStdString();

    QString pwd=ui->line_pwd->text();
    std::string spwd=pwd.toStdString();

    if(name==NULL || pwd==NULL)
    {
        msgbox.setText("还有未填写用户名或密码。");
        msgbox.show();
        return ;
    }

    rigister_node node(sname.c_str(),spwd.c_str());
    memcpy(msg.buf,&node,sizeof(node));
    clientSocket->write((char *)&msg,sizeof(msg));
}

