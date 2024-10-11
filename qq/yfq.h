#ifndef YFQ_H
#define YFQ_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "chat.h"
namespace Ui {
class yfq;
}

class yfq : public QMainWindow
{
    Q_OBJECT

public:
    explicit yfq(QWidget *parent = nullptr);
    ~yfq();

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::yfq *ui;
    Chat *chat;
};

#endif // YFQ_H
