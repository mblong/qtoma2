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

    char text[NEW_PREFIX_CHPERLN];
    strlcpy(text,SETTINGSFILE,NEW_PREFIX_CHPERLN);
    loadprefs(text);

    strlcpy(text,DCRAW_ARG,NEW_PREFIX_CHPERLN);
    dcrawarg_c(0,text);

    printf("OMA2>");
    w.show();
    update_UI();
    return a.exec();
}