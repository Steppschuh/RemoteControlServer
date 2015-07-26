#ifndef MEDIA_H
#define MEDIA_H


class Media
{
public:
    Media *Instance();

private:
    Media();
    Media *instance;

    void playMedia();
    void stopMedia();
    void nextMedia();
    void previousMedia();
    void volumeUp();
    void volumeDown();
    void volumeMute();
//    void launchPlayer();
//    QString getDefaultMediaPlayerPath(); //Really necessary on mac???
};

#endif // MEDIA_H
