#-------------------------------------------------
#
# Project created by QtCreator 2015-05-25T01:14:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GrizzlyPanel
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    grizzlydata.cpp

HEADERS  += mainwindow.h \
    grizzlydata.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/include

LIBS += -L"$$PWD/lib" -lgrizzlypp

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libusb-1.0
