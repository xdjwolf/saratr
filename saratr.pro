#-------------------------------------------------
#
# Project created by QtCreator 2016-03-15T14:32:42
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
qtHaveModule(printsupport): QT += printsupport

win32 {contains(QMAKE_TARGET.arch, x86_64) {PLATFORM = x64} else {PLATFORM = Win32}}

TARGET = saratr
TEMPLATE = app

HEADERS += \
    mainwindow.h \
    ddqlistwidget.h \
    imageviewer3.h \
    IPL_global.h \
    IPLColor.h \
    IPLComplexImage.h \
    IPLData.h \
    IPLImage.h \
    IPLImagePlane.h \
    IPLMatrix.h \
    IPLPoint.h \
    IPLProcess.h \
    IPLProcessProperty.h \
    IPLProgressEventHandler.h \
    IPLPropertyChangedEventHandler.h \
    IPLOutputsChangedEventHandler.h \
    pugg/Driver.h

SOURCES += main.cpp \
    mainwindow.cpp \
    ddqlistwidget.cpp \
    imageviewer3.cpp \
    IPL_global.cpp \
    IPLComplexImage.cpp \
    IPLData.cpp \
    IPLImage.cpp \
    IPLImagePlane.cpp \
    IPLMatrix.cpp \
    IPLPoint.cpp \
    IPLProcess.cpp \
    IPLProcessProperty.cpp

RESOURCES += mainwindow.qrc


INCLUDEPATH+=D:/workspaces/qt5/opencv_env/include/opencv \
             D:/workspaces/qt5/opencv_env/include/opencv2 \
             D:/workspaces/qt5/opencv_env/include

LIBS += D:/workspaces/qt5/opencv_env/lib/opencv_world310.lib \
        D:/workspaces/qt5/opencv_env/lib/opencv_world310d.lib

FORMS += \
    mainwindow.ui

DISTFILES += \
    README.md
