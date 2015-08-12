#include "logger.h"
#include "server.h"
#include "settings.h"
#include "updater.h"

#include <QFileInfo>
#include <QtConcurrent>
#include <QObject>
#include <QTimer>

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
    QTimer::singleShot(delayInSeconds * 1000, this, SLOT(checkForUpdates()));
}

void Updater::checkForUpdates()
{
    Logger::Instance()->add("Checking for updates");
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseUpdateResult(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(URL_UPDATE_INFO)));
}

void Updater::parseUpdateResult(QNetworkReply *reply)
{
    QByteArray bytes = reply->readAll();
    QString result = QString::fromUtf8(bytes.data(), bytes.size());

    updateVersionCode = getValue("versioncode", result).toInt();
    updateVersionName = getValue("versionname", result);
    updateReleaseDate = getValue("releasedate", result);
    updateChangeLog = getValue("changelog", result).replace("\t", " ");

    if (updateVersionCode > currentVersionCode)
    {
        Logger::Instance()->add("Update available: " + updateVersionName);
        isUpdateAvailable = true;
        Server::Instance()->showNotification("Update available", "There is a new version of the Remote Control Server available");
    }
    else
    {
        Logger::Instance()->add("No update available");
        isUpdateAvailable = false;
    }
    emit hasUpdatesParsed();

    if (isUpdateAvailable) emit updatesAvailable();
}

QString Updater::getValue(QString tag, QString source)
{
    int index_start = source.indexOf(tag) + tag.length() + 1;
    QString tmp = source.right(source.length() - index_start);

    int index_end = tmp.indexOf("</");
    tmp = tmp.left(index_end);
    return tmp;
}

void Updater::startUpdater()
{
    if (!isUpdateAvailable) return;
#ifdef Q_OS_MAC
    QString pathUpdater = Settings::Instance()->getAppDataDirectory() + "/RemoteControlServerUpdater";
#endif

    QFileInfo fileInfo(pathUpdater);
    if (!fileInfo.exists() || !fileInfo.isDir())
    {
        QString appDataDir = Settings::Instance()->getAppDataDirectory();
        if (!QDir(appDataDir).exists()){
            QDir().mkpath(appDataDir);
        }
#ifdef Q_OS_MAC

        QFile::copy(":/Resources/RemoteControlServerUpdater", pathUpdater);

        QFile file(pathUpdater);
        file.setPermissions(QFile::ReadOwner|QFile::WriteOwner|QFile::ExeOwner);

#endif
    }

    Logger::Instance()->add("Starting update tool");
    Server::Instance()->startProcess(pathUpdater);
    //    emit hasUpdatesStarted();
}

