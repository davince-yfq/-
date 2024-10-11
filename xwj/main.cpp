#include "xwj.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    xwj w;
    w.show();
    return a.exec();
}
