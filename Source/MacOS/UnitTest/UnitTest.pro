#-------------------------------------------------
#
# Project created by QtCreator 2015-07-13T10:49:04
#
#-------------------------------------------------

QT += testlib core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = Test
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

SOURCES += main.cpp \
    testlogger.cpp

HEADERS += AutoTest.h \
    AutoTest.h \
    testlogger.h

include(../RemoteControlServer/RemoteControlServerSrc.pri)
