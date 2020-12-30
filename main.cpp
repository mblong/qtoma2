#include "qtoma2.h"
#include <QApplication>

QtOma2* wPointer;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDir::setCurrent(QCoreApplication::applicationDirPath());
    qDebug()<<QCoreApplication::applicationDirPath();

    QtOma2 w;
    wPointer = &w;
 //   QDesktopWidget widget;
//    QRect mainScreenSize = QGuiApplication::primaryScreen()->mainScreenSize;

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect mainScreenSize = screen->availableGeometry();


    w.setGeometry(0,mainScreenSize.y()+mainScreenSize.height()-WINDOW_HEIGHT,COMMAND_WIDTH,WINDOW_HEIGHT);



    char text[NEW_PREFIX_CHPERLN];
    strlcpy(text,SETTINGSFILE,NEW_PREFIX_CHPERLN);
    loadprefs(text);

    extern oma2UIData UIData;
    UIData.newwindowflag = 1;   // this default is set initially
    UIData.autoFloatFlag = 1;   // this default is set initially

    strlcpy(text,DCRAW_ARG,NEW_PREFIX_CHPERLN);
    dcrawarg_c(0,text);

    if(argc > 1){
        gmacro_c(0,argv[1]);
        char cmnd[10]={"mac"};
        comdec(cmnd);
    }

    printf("OMA2>");
    w.show();
    update_UI();
    return a.exec();
}
