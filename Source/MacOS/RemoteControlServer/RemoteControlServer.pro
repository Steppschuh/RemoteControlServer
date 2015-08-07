#-------------------------------------------------
#
# Project created by QtCreator 2015-07-12T15:46:43
#
#-------------------------------------------------

CONFIG +=c++11
LIBS += -framework ApplicationServices

VERSION = 0.5

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

QT       += core gui
QT       += xml
QT       += network
QT       += concurrent
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RemoteControlServer
TEMPLATE = app

SOURCES += main.cpp \
    mainwindow.cpp \
    apiv1.cpp \
    apiv2.cpp \
    apiv3.cpp \
    app.cpp \
    authentication.cpp \
    command.cpp \
    converter.cpp \
    helper.cpp \
    keyboardmac.cpp \
    logger.cpp \
    media.cpp \
    mousemac.cpp \
    mousev2.cpp \
    mousev3.cpp \
    network.cpp \
    remote.cpp \
    screenshot.cpp \
    serial.cpp \
    server.cpp \
    settings.cpp \
    tcp.cpp \
    touchpoint.cpp \
    trayicon.cpp \
    udp.cpp \
    updater.cpp \
    whitelistwindow.cpp

LIBS += -framework IOKit

OBJECTIVE_SOURCES += macx.mm

HEADERS += macx.h \
    mainwindow.h \
    apiv1.h \
    apiv2.h \
    apiv3.h \
    app.h \
    authentication.h \
    command.h \
    converter.h \
    helper.h \
    keyboardmac.h \
    logger.h \
    media.h \
    mousemac.h \
    mousev2.h \
    mousev3.h \
    network.h \
    remote.h \
    screenshot.h \
    serial.h \
    server.h \
    settings.h \
    tcp.h \
    touchpoint.h \
    trayicon.h \
    udp.h \
    updater.h \
    whitelistwindow.h

RESOURCES += \
    resources.qrc

FORMS += \
    mainwindow.ui \
    whitelistwindow.ui
