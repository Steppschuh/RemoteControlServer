#-------------------------------------------------
#
# Project created by QtCreator 2015-07-12T15:46:43
#
#-------------------------------------------------

QT       += core gui
QT       += xml
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RemoteControlServer
TEMPLATE = app

SOURCES += main.cpp

include(RemoteControlServerSrc.pri)
