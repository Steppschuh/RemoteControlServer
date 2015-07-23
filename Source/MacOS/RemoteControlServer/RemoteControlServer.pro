#-------------------------------------------------
#
# Project created by QtCreator 2015-07-12T15:46:43
#
#-------------------------------------------------

CONFIG +=c++11

QT       += core gui
QT       += xml
QT       += network
QT       += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RemoteControlServer
TEMPLATE = app

include(RemoteControlServerSrc.pri)

SOURCES += main.cpp
