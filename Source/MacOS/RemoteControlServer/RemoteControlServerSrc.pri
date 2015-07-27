VERSION = 0.5

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

INCLUDEPATH += ../RemoteControlServer

RESOURCES     = ../RemoteControlServer/resources.qrc

HEADERS  += ../RemoteControlServer/mainwindow.h \
    ../RemoteControlServer/apiv1.h \
    ../RemoteControlServer/apiv2.h \
    ../RemoteControlServer/apiv3.h \
    ../RemoteControlServer/app.h \
    ../RemoteControlServer/authentication.h \
    ../RemoteControlServer/command.h \
    ../RemoteControlServer/converter.h \
    ../RemoteControlServer/helper.h \
    ../RemoteControlServer/keyboardmac.h \
    ../RemoteControlServer/license.h \
    ../RemoteControlServer/logger.h \
    ../RemoteControlServer/media.h \
    ../RemoteControlServer/mousev3mac.h \
    ../RemoteControlServer/network.h \
    ../RemoteControlServer/remote.h \
    ../RemoteControlServer/screenshot.h \
    ../RemoteControlServer/serial.h \
    ../RemoteControlServer/server.h \
    ../RemoteControlServer/settings.h \
    ../RemoteControlServer/tcp.h \
    ../RemoteControlServer/touchpoint.h \
    ../RemoteControlServer/trayicon.h \
    ../RemoteControlServer/udp.h

SOURCES += ../RemoteControlServer/mainwindow.cpp \
    ../RemoteControlServer/apiv1.cpp \
    ../RemoteControlServer/apiv2.cpp \
    ../RemoteControlServer/apiv3.cpp \
    ../RemoteControlServer/app.cpp \
    ../RemoteControlServer/authentication.cpp \
    ../RemoteControlServer/command.cpp \
    ../RemoteControlServer/converter.cpp \
    ../RemoteControlServer/helper.cpp \
    ../RemoteControlServer/keyboardmac.cpp \
    ../RemoteControlServer/license.cpp \
    ../RemoteControlServer/logger.cpp \
    ../RemoteControlServer/media.cpp \
    ../RemoteControlServer/mousev3mac.cpp \
    ../RemoteControlServer/network.cpp \
    ../RemoteControlServer/remote.cpp \
    ../RemoteControlServer/screenshot.cpp \
    ../RemoteControlServer/serial.cpp \
    ../RemoteControlServer/server.cpp \
    ../RemoteControlServer/settings.cpp \
    ../RemoteControlServer/tcp.cpp \
    ../RemoteControlServer/touchpoint.cpp \
    ../RemoteControlServer/trayicon.cpp \
    ../RemoteControlServer/udp.cpp

FORMS    += ../RemoteControlServer/mainwindow.ui
