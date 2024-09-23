#ifndef ENROLL_H
#define ENROLL_H

#include <QMainWindow>
#include "mainwindow.h"
#include "comment.h"

namespace Ui {
class enroll;
}

class enroll : public QMainWindow
{
    Q_OBJECT

public:
    explicit enroll(QWidget *parent = nullptr);
    ~enroll();
    QMessageBox msgbox;
private slots:
    void on_pushButton_clicked();

private:
    Ui::enroll *ui;
};

#endif // ENROLL_H
