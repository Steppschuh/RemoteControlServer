#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QString>

class Updater : public QObject
{
    Q_OBJECT

public:
    static Updater *Instance();

    const QString URL_UPDATE_HELP;

    char currentVersionCode = 0;
    QString updateChangeLog = "Unknown";
    QString updateVersionName = "Unknown";

    void checkForUpdates(int delayInSeconds);

signals:
    void hasUpdatesParsed();

public slots:
    void startUpdater();

private:
    static Updater *instance;
    Updater();
};

#endif // UPDATER_H
