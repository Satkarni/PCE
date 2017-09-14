#-------------------------------------------------
#
# Project created by QtCreator 2016-06-02T14:18:53
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CILOverspeed
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logwindow.cpp \
    packethandler.cpp \
    configurationhandler.cpp

HEADERS  += mainwindow.h \
    logwindow.h \
    packethandler.h \
    configurationhandler.h

FORMS    += mainwindow.ui \
    logwindow.ui

DISTFILES +=

RESOURCES += \
    resources.qrc
