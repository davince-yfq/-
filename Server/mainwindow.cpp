#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server=new QTcpServer(this);
    server->listen(QHostAddress::Any,9000);
    connect(server,&QTcpServer::newConnection,this,&MainWindow::acceptConneted);
    sqlOP=new sqliteOperator;
    sqlOP->opendb();
//    QString sql="create table User(name varchar(128),password varchar(48));";
//    sqlOP->normalExec(sql);
//    sqlOP->closedb();
}

MainWindow::~MainWindow()
{
    sqlOP->closedb();
    delete ui;
}

void MainWindow::acceptConneted()
{
    client=server->nextPendingConnection();//返回当前链接客户端
    connect(client,&QTcpSocket::readyRead,this,&MainWindow::onReayread);
    connect(client,&QTcpSocket::disconnected,this,&MainWindow::ondisconnected);
}

void MainWindow::onReayread()
{
        qDebug()<<"33"<<endl;
      if(client)
      {
           Qmsg msg;
           int len=client->read((char *)&msg,sizeof(msg));
           qDebug()<<"收到"<<len<<"个字节"<<endl;
           if(msg.type==1)//注册
           {
               rigister_node *node=(rigister_node *)msg.buf;
               //查询有无重复用户名
               QSqlQuery sqlqurey;
               QString sql=QString("select * from User where name='%1'").arg(node->name);
               sqlOP->checkExec(sqlqurey,sql);
               if(sqlqurey.next())
               {
                   msg.type=11;//注册失败
                   client->write((char *)&msg,sizeof(msg));
               }else
               {
                   msg.type=12;//注册成功
                   client->write((char *)&msg,sizeof(msg));
                   qDebug()<<node->name<<" "<<node->pwd<<endl;
                   QString sql1=QString("insert into User values('%1','%2')").arg(node->name).arg(node->pwd);
                   sqlOP->normalExec(sql1);
               }
           }
           else if(msg.type==2)//登录
           {
                rigister_node *node=(rigister_node *)msg.buf;
                QSqlQuery sqlqurey;
                QString sql=QString("select * from User where name='%1' and password='%2'").arg(node->name).arg(node->pwd);
                sqlOP->checkExec(sqlqurey,sql);
                if(sqlqurey.next())
                {
                    qDebug()<<"登入成功！"<<endl;
                    QString name=sqlqurey.value(0).toString();
                    QString pwd=sqlqurey.value(1).toString();
                    qDebug()<<name<<" "<<pwd<<endl;
                    //发送登入成功的消息给客户端
                    msg.type=21;
                    client->write((char *)&msg,sizeof(msg));
                }
                else{
                    msg.type=22;
                    client->write((char *)&msg,sizeof(msg));
                }
           }
      }
}

void MainWindow::ondisconnected()
{
    if(client)
    {
        qDebug()<<"客户端退出了"<<endl;
    }
}

