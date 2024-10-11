#include "yfq.h"
#include "ui_yfq.h"
#include "mainwindow.h"
#include <QTcpSocket>
yfq::yfq(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::yfq)
{
    ui->setupUi(this);

    // 假设窗口宽度为800像素，设置初始比例
    int initialWidth = 800;
    int part1Width = initialWidth * 20 / 100;
    int part2Width = initialWidth - part1Width; // 或者 initialWidth * 40 / 100，但这里用减法更直观

    // 设置QSplitter的初始大小比例
    QList<int> sizes;
    sizes << part1Width << part2Width;
    ui->splitter->setSizes(sizes);

    ui->listWidget->addItems(QStringList()<<"聊天"<<"天气精灵"<<"智能家居");
    //设定显示窗口
    chat = new Chat(ui->frame);
    chat->hide();
}

yfq::~yfq()
{
    delete ui;
}

void yfq::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if(item->text()=="聊天")
    {
        chat->show();
    }
}
