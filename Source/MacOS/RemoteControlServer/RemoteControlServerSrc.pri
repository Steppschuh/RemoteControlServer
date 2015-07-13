VERSION = 0.2

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

INCLUDEPATH += ../RemoteControlServer

SOURCES += ../RemoteControlServer/mainwindow.cpp \
    ../RemoteControlServer/server.cpp \
    ../RemoteControlServer/logger.cpp \
    ../RemoteControlServer/network.cpp \
    ../RemoteControlServer/tcp.cpp \
    ../RemoteControlServer/command.cpp \
    ../RemoteControlServer/remote.cpp

HEADERS  += ../RemoteControlServer/mainwindow.h \
    ../RemoteControlServer/server.h \
    ../RemoteControlServer/logger.h \
    ../RemoteControlServer/network.h \
    ../RemoteControlServer/tcp.h \
    ../RemoteControlServer/command.h \
    ../RemoteControlServer/remote.h

FORMS    += ../RemoteControlServer/mainwindow.ui
