#-------------------------------------------------
#
# Project created by QtCreator 2015-08-12T12:12:11
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RemoteControlServerUpdater
TEMPLATE = app

ICON = applicationIcon.icns

SOURCES += main.cpp\
        updater.cpp

HEADERS  += updater.h

FORMS    += updater.ui
