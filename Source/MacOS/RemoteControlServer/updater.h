#ifndef UPDATER_H
#define UPDATER_H


class Updater
{
public:
    static Updater *Instance();

    char currentVersionCode = 0;

    void checkForUpdates(int delayInSeconds);

private:
    static Updater *instance;
    Updater();
};

#endif // UPDATER_H
