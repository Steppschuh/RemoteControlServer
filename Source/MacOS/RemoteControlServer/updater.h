#ifndef UPDATER_H
#define UPDATER_H

#include <QNetworkReply>
#include <QObject>
#include <QString>

class Updater : public QObject
{
    Q_OBJECT

public:
    static Updater *Instance();

    const QString URL_UPDATE_INFO = "http://remote-control-collection.com/files/server/update_mac.xml";
    const QString URL_UPDATE_HELP = "http://remote-control-collection.com/help/update/";

    const char currentVersionCode = 1;

    char updateVersionCode = 0;
    QString updateVersionName = "";
    QString updateReleaseDate = "";
    QString updateChangeLog = "";

    void checkForUpdates(int delayInSeconds);

signals:
    void hasUpdatesParsed();
    void hasUpdatesStarted();
    void updatesAvailable();

public slots:
    void checkForUpdates();
    void parseUpdateResult(QNetworkReply *reply);

public slots:
    void startUpdater();

private:
    static Updater *instance;
    Updater();

    bool isUpdateAvailable = false;

    QString getValue(QString tag, QString source);
};

#endif // UPDATER_H
