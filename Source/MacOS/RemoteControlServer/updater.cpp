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

Updater::Updater() :
URL_UPDATE_HELP("http://remote-control-collection.com/help/update/")
{
}

void Updater::checkForUpdates(int delayInSeconds)
{

}

void Updater::startUpdater()
{

}

