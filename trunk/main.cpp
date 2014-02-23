#include "qtoma2.h"
#include <QApplication>

QtOma2* wPointer;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtOma2 w;
    wPointer = &w;
    QDesktopWidget widget;
    QRect mainScreenSize = widget.availableGeometry(widget.primaryScreen());
    w.setGeometry(0,mainScreenSize.y()+mainScreenSize.height()-WINDOW_HEIGHT,COMMAND_WIDTH,WINDOW_HEIGHT);
    update_UI();
    w.show();

    return a.exec();
}
