#ifndef MEDIA_H
#define MEDIA_H

#include <QString>

class Media
{
public:
    static Media *Instance();

    void playMedia();
    void stopMedia();
    void nextMedia();
    void previousMedia();
    void volumeUp();
    void volumeDown();
    void volumeMute();
    void launchPlayer();
    QString getDefaultMediaPlayerPath();

private:
    Media();
    static Media *instance;


//    void launchPlayer();
};

#endif // MEDIA_H
