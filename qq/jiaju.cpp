#include "jiaju.h"
#include "ui_jiaju.h"

jiaju::jiaju(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::jiaju)
{
    ui->setupUi(this);
}

jiaju::~jiaju()
{
    delete ui;
}
