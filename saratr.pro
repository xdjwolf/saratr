#-------------------------------------------------
#
# Project created by QtCreator 2016-03-15T14:32:42
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
qtHaveModule(printsupport): QT += printsupport


TARGET = saratr
TEMPLATE = app

HEADERS += \
    mainwindow.h

SOURCES += main.cpp \
    mainwindow.cpp \
    test.cpp

RESOURCES += mainwindow.qrc


INCLUDEPATH+=D:/workspaces/qt5/opencv_env/include/opencv \
             D:/workspaces/qt5/opencv_env/include/opencv2 \
             D:/workspaces/qt5/opencv_env/include

LIBS += D:/workspaces/qt5/opencv_env/lib/opencv_world310.lib \
        D:/workspaces/qt5/opencv_env/lib/opencv_world310d.lib

FORMS += \
    mainwindow.ui
