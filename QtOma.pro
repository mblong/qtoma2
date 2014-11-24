#-------------------------------------------------
#
# Project created by QtCreator 2014-02-09T09:32:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtOma
TEMPLATE = app


macx:QMAKE_CXXFLAGS += -DQt_UI
win32:QMAKE_CXXFLAGS += -DQt_UI_Win -fpermissive\
    -Wno-unused-parameter -Wno-overflow -Wno-aggressive-loop-optimizations -Wno-array-bounds\
    -Wno-unused-variable -Wno-sign-compare -Wno-write-strings -Wno-unused-but-set-variable\
    -Wno-unknown-pragmas -Wno-strict-overflow -Wno-narrowing -Wno-type-limits\
    -D_WIN32 -DWIN32
unix:!macx: QMAKE_CXXFLAGS += -DQt_UI_Linux -fpermissive -Wno-unused-parameter

SOURCES += main.cpp\
        qtoma2.cpp\
        oma2UiIndependent/comdec.cpp\
        oma2UiIndependent/ImageBitmap.cpp\
        oma2UiIndependent/commands_1.cpp\
        oma2UiIndependent/dcrawx.cpp\
        oma2UiIndependent/gluedCommands.cpp\
        oma2UiIndependent/image.cpp\
        oma2UiIndependent/image_support.cpp \
        oma2UiIndependent/dofft.cpp \
        QtUI.cpp \
        datawindow.cpp \
        preferences.cpp \
        status.cpp \
        drawingwindow.cpp \
    qtimagesupport.cpp \
    oma2commands.cpp


unix:!macx: {
SOURCES += Hardware/andor.cpp \
    Hardware/serial.c
}

HEADERS  += qtoma2.h\
        oma2UiIndependent/UI.h\
        oma2UiIndependent/comdec.h\
        oma2UiIndependent/commands_1.h\
        oma2UiIndependent/dcrawx.h\
        oma2UiIndependent/dofft.h\
        oma2UiIndependent/gluedCommands.h\
        oma2UiIndependent/image.h\
        oma2UiIndependent/image_support.h\
        oma2UiIndependent/ImageBitmap.h \
        oma2UiIndependent/oma2.h \
        datawindow.h \
        drawingwindow.h \
        preferences.h \
        status.h \
    qtimagesupport.h \
    oma2commands.h \
    Hardware/cameraSelector.h


FORMS    += qtoma2.ui \
    datawindow.ui \
    preferences.ui \
    status.ui \
    drawingwindow.ui

#macx: LIBS += -L/opt/local/lib/ -ljpeg
#macx: INCLUDEPATH += /opt/local/lib
#macx: DEPENDPATH += /opt/local/lib
#macx: PRE_TARGETDEPS += /opt/local/lib/libjpeg.a


unix:!macx: LIBS += -landor

#win32:  LIBS += -ljpeg -lws2_32
win32:  LIBS += -lws2_32



RESOURCES += \
    qtoma2.qrc

#for Windows
win32 {
    SFILE = $$_PRO_FILE_PWD_
    SFILE ~= s,/,\\,g
    DFILE = $$OUT_PWD
    DFILE ~= s,/,\\,g
    EXTRA_FILES = $$SFILE\oma2UiIndependent\oma2help.txt
    EXTRA_FILES +=  $$SFILE\Resources\OMApalette.pa1
    EXTRA_FILES +=  $$SFILE\Resources\OMApalette2.pa1
    EXTRA_FILES +=  $$SFILE\Resources\OMApalette3.pa1

    for(FILE,EXTRA_FILES){
        QMAKE_POST_LINK += $$quote(copy $${FILE} $${DFILE}$$escape_expand(\\n\\t))
    }
}

#for Mac
macx {
    MediaFiles.files = "oma2UiIndependent/oma2help.txt"
    MediaFiles.files += "Resources/OMApalette.pa1"
    MediaFiles.files += "Resources/OMApalette2.pa1"
    MediaFiles.files += "Resources/OMApalette3.pa1"
    MediaFiles.path = Contents/Resources
    QMAKE_BUNDLE_DATA += MediaFiles
}

#for linux
unix:!macx:{
    EXTRA_FILES += \
        $$_PRO_FILE_PWD_/oma2UiIndependent/oma2help.txt \
        $$_PRO_FILE_PWD_/Resources/OMApalette.pa1 \
        $$_PRO_FILE_PWD_/Resources/OMApalette2.pa1 \
        $$_PRO_FILE_PWD_/Resources/OMApalette3.pa1
    for(FILE,EXTRA_FILES){
        QMAKE_POST_LINK += $$quote(cp $${FILE} ./$$escape_expand(\\n\\t))
    }
}

# Notes:
# Need to check out from the command line before QtCreator will recognize a subversion repository
# Include username too.
# Also check the username and password in Qt preferences.
#
