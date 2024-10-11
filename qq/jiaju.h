#ifndef JIAJU_H
#define JIAJU_H

#include <QWidget>

namespace Ui {
class jiaju;
}

class jiaju : public QWidget
{
    Q_OBJECT

public:
    explicit jiaju(QWidget *parent = nullptr);
    ~jiaju();

private:
    Ui::jiaju *ui;
};

#endif // JIAJU_H
