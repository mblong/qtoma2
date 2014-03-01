#-------------------------------------------------
#
# Project created by QtCreator 2014-02-09T09:32:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtOma
TEMPLATE = app

CONFIG(release, debug|release) {
    DESTDIR = release
} else {
    DESTDIR = debug
}


#macx:
QMAKE_CXXFLAGS += -DQt_UI
#win:
#QMAKE_CXXFLAGS += -DQt_UI_Win
#unix:!macx:
#QMAKE_CXXFLAGS += -DQt_UI_Linux -fpermissive

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
        oma2UiIndependent/jpegIO.cpp \
        QtUI.cpp \
        datawindow.cpp \
    preferences.cpp \
    status.cpp \
    drawingwindow.cpp


HEADERS  += qtoma2.h\
        oma2UiIndependent/UI.h\
        oma2UiIndependent/comdec.h\
        oma2UiIndependent/commands_1.h\
        oma2UiIndependent/dcrawx.h\
        oma2UiIndependent/dofft.h\
        oma2UiIndependent/gluedCommands.h\
        oma2UiIndependent/image.h\
        oma2UiIndependent/image_support.h\
        oma2UiIndependent/jconfig.h\
        oma2UiIndependent/jmorecfg.h\
        oma2UiIndependent/jpeglib.h\
        oma2UiIndependent/ImageBitmap.h \
        oma2UiIndependent/oma2.h \
        datawindow.h \
        preferences.h \
        oma2UiIndependent/tiff.h\
        oma2UiIndependent/tiffio.h\
        oma2UiIndependent/tiffvers.h \
    status.h \
    drawingwindow.h


FORMS    += qtoma2.ui \
    datawindow.ui \
    preferences.ui \
    status.ui \
    drawingwindow.ui

macx: LIBS += -L/opt/local/lib/ -ljpeg

INCLUDEPATH += /opt/local/lib
DEPENDPATH += /opt/local/lib

macx: PRE_TARGETDEPS += /opt/local/lib/libjpeg.a

unix:!macx: LIBS += -ljpeg
unix:!macx: LIBS += -lbsd


RESOURCES += \
    qtoma2.qrc

#for Windows
win32 {
    #in Windows, you can use & to separate commands
    #copyfiles.commands += @echo NOW COPYING ADDITIONAL FILE(S) &
    #copyfiles.commands += @call copy ..\\$${TARGET}\\ffmpeg.exe $${DESTDIR}\\ffmpeg.exe
}
#QMAKE_EXTRA_TARGETS += copyfiles
#POST_TARGETDEPS += copyfiles

#for Mac
macx {
    MediaFiles.files = "oma help.txt"
    MediaFiles.files += "Resources/OMA palette.pa1"
    MediaFiles.files += "Resources/OMA palette2.pa1"
    MediaFiles.files += "Resources/OMA palette3.pa1"
    MediaFiles.path = Contents/Resources
    QMAKE_BUNDLE_DATA += MediaFiles
}


