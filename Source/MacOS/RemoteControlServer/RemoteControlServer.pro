#-------------------------------------------------
#
# Project created by QtCreator 2015-07-12T15:46:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 0.1

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

TARGET = RemoteControlServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    server.cpp \
    logger.cpp

HEADERS  += mainwindow.h \
    server.h \
    logger.h

FORMS    += mainwindow.ui
