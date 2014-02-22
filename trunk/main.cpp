#include "qtoma2.h"
#include <QApplication>

QtOma2* wPointer;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtOma2 w;
    wPointer = &w;
    w.show();

    return a.exec();
}
