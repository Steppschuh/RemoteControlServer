#include "updater.h"

#include <QObject>

Updater *Updater::instance = NULL;

Updater *Updater::Instance()
{
    if (!instance)
    {
        instance = new Updater();
    }
    return instance;
}

Updater::Updater()
{

}

void Updater::checkForUpdates(int delayInSeconds)
{

}

