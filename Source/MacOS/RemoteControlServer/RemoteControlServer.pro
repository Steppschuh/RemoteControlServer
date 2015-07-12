#-------------------------------------------------
#
# Project created by QtCreator 2015-07-12T15:46:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 0.2

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

TARGET = RemoteControlServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    server.cpp \
    logger.cpp \
    network.cpp \
    tcp.cpp \
    command.cpp

HEADERS  += mainwindow.h \
    server.h \
    logger.h \
    network.h \
    tcp.h \
    command.h

FORMS    += mainwindow.ui
