#include "media.h"
#include "server.h"
#include "settings.h"

#include <QFileInfo>

#ifdef Q_OS_MAC
    #include <ApplicationServices/ApplicationServices.h>
    #include <IOKit/hidsystem/ev_keymap.h>
    #include "macx.h"
#endif

#include <QDebug>

Media* Media::instance = NULL;

Media* Media::Instance()
{
    if (!instance)
    {
        instance = new Media();
    }
    return instance;
}

Media::Media()
{

}

void Media::playMedia()
{
#ifdef Q_OS_MAC
    HIDPostAuxKey (NX_KEYTYPE_PLAY);
#endif
}

void Media::stopMedia()
{
#ifdef Q_OS_MAC
    HIDPostAuxKey (NX_KEYTYPE_PLAY);
#endif
}

void Media::nextMedia()
{
#ifdef Q_OS_MAC
    HIDPostAuxKey (NX_KEYTYPE_NEXT);
#endif
}

void Media::previousMedia()
{
#ifdef Q_OS_MAC
    HIDPostAuxKey (NX_KEYTYPE_PREVIOUS);
#endif
}

void Media::volumeUp()
{
#ifdef Q_OS_MAC
    HIDPostAuxKey (NX_KEYTYPE_SOUND_UP);
#endif
}

void Media::volumeDown()
{
#ifdef Q_OS_MAC
    HIDPostAuxKey (NX_KEYTYPE_SOUND_DOWN);
#endif
}

void Media::volumeMute()
{
#ifdef Q_OS_MAC
    HIDPostAuxKey (NX_KEYTYPE_MUTE);
#endif
}

void Media::launchPlayer()
{
    QString path = Settings::Instance()->defaultMediaPlayer;
    QFileInfo checkFile(path);
    if (!checkFile.exists() || !checkFile.isDir())
    {
        path = getDefaultMediaPlayerPath();
    }
    Server::Instance()->startProcess(path);
}

QString Media::getDefaultMediaPlayerPath()
{
    QString pathFound = "";
    QString path;
    path = "/Applications/iTunes.app";
    QFileInfo *checkFile = new QFileInfo(path);
    if (checkFile->exists() && checkFile->isDir())
    {
        pathFound = path;
    }

    path = "/Applications/Spotify.app";
    checkFile = new QFileInfo(path);
    if (checkFile->exists() && checkFile->isDir())
    {
        pathFound = path;
    }
    return pathFound;
}
