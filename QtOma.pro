#-------------------------------------------------
#
# Project created by QtCreator 2014-02-09T09:32:32
#
#-------------------------------------------------

QT       += core gui sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtOma
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


macx:QMAKE_CXXFLAGS += -DQt_UI_Mac\
    -Wno-c++11-narrowing -Wno-sign-conversion

win32:QMAKE_CXXFLAGS += -DQt_UI_Win -fpermissive\
    -Wno-unused-parameter -Wno-overflow -Wno-aggressive-loop-optimizations -Wno-array-bounds\
    -Wno-unused-variable -Wno-sign-compare -Wno-write-strings -Wno-unused-but-set-variable\
    -Wno-unknown-pragmas -Wno-strict-overflow -Wno-narrowing -Wno-type-limits

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
    oma2commands.cpp \
    variableswindow.cpp \
    Hardware/gige.cpp \
    oma2UiIndependent/dmmalvar.cpp \
    oma2UiIndependent/dmbilinear.cpp \
    sqlRoutines.cpp \
    videoRoutines.cpp


unix:!macx: {
SOURCES += Hardware/andor.cpp \
    Hardware/serial.c \
    Hardware/gphotoFunctions.c \
    Hardware/gphotoCommands.cpp
}

macx: {
SOURCES += Hardware/serial.c
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
    Hardware/cameraSelector.h \
    variableswindow.h \
    Hardware/gphotoFunctions.h \
    Hardware/ImageLib.h \
    Hardware/PvApi.h \
    oma2UiIndependent/dmbilinear.h \
    oma2UiIndependent/dmmalvar.h \
    videoRoutines.h


FORMS    += qtoma2.ui \
    datawindow.ui \
    preferences.ui \
    status.ui \
    drawingwindow.ui \
    variableswindow.ui


RESOURCES += \
    qtoma2.qrc

#for Windows
win32 {
    LIBS += -lws2_32
    SFILE = $$_PRO_FILE_PWD_
    SFILE ~= s,/,\\,g
    DFILE = $$OUT_PWD
    DFILE ~= s,/,\\,g
    EXTRA_FILES = $$SFILE\oma2UiIndependent\oma2help.txt
    EXTRA_FILES +=  $$SFILE\Resources\OMApalette.pa1
    EXTRA_FILES +=  $$SFILE\Resources\OMApalette2.pa1
    EXTRA_FILES +=  $$SFILE\Resources\OMApalette3.pa1
    EXTRA_FILES +=  $$SFILE\Resources\OMASettings

    for(FILE,EXTRA_FILES){
        #message($$quote(copy \"$${FILE}\" \"$${DFILE}\"$$escape_expand(\\n\\t)))
        QMAKE_POST_LINK += $$quote(copy \"$${FILE}\" \"$${DFILE}\\\"$$escape_expand(\\n\\t))
    }
}

#for Mac
macx {
    MediaFiles.files = "oma2UiIndependent/oma2help.txt"
    MediaFiles.files += "Resources/OMApalette.pa1"
    MediaFiles.files += "Resources/OMApalette2.pa1"
    MediaFiles.files += "Resources/OMApalette3.pa1"
    MediaFiles.files += "Resources/OMASettings"
    MediaFiles.path = Contents/Resources
    QMAKE_BUNDLE_DATA += MediaFiles

    ICON = qtOma2Icon.icns



    # for video commands
    # suggest making symbolic link in /usr/local/lib as follows:
    #   cd /usr/local/lib
    #   ln -s ../Cellar/opencv/4.4.0/lib opencv
    # Update to whatever is the latest version of opencv (4.4.0 in this case)

    INCLUDEPATH += /usr/local/include/opencv4
    DEPENDPATH += /usr/local/lib

    LIBS += -L/usr/local/lib/opencv \
        -lopencv_core \
        -lopencv_imgcodecs \
        -lopencv_video \
        -lopencv_videoio \

}

#for linux
unix:!macx:{

    LIBS += -landor -lgphoto2

    EXTRA_FILES += \
        $$_PRO_FILE_PWD_/oma2UiIndependent/oma2help.txt \
        $$_PRO_FILE_PWD_/Resources/OMApalette.pa1 \
        $$_PRO_FILE_PWD_/Resources/OMApalette2.pa1 \
        $$_PRO_FILE_PWD_/Resources/OMApalette3.pa1
        $$_PRO_FILE_PWD_/Resources/OMASettings
    for(FILE,EXTRA_FILES){
        QMAKE_POST_LINK += $$quote(cp $${FILE} ./$$escape_expand(\\n\\t))
    }
}

# Notes:
# Need to check out from the command line before QtCreator will recognize a subversion repository
# Include username too.
# Also check the username and password in Qt preferences.
#

OBJECTIVE_SOURCES +=

DISTFILES +=
