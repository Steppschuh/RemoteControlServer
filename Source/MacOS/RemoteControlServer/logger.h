#ifndef LOGGER_H
#define LOGGER_H

#include <QDateTime>
#include <QString>
#include <QStringList>

class Logger
{
public:
    static Logger* Instance();
    void add(QString message);
    void add(QString message, bool isDebug);
    void clearLog();
    QString getString();
    QString getLastEntry();

private:
    Logger();
    static Logger* instance;

    const QString URL_TRACKER;

    const bool trackEvents;
    const bool showDebug;
    const int maxItems;

    QStringList *logMessages;

    QDateTime lastUpdate;
    bool dispatcherActive;

    QString lastEntry;

    void trim();
    void trackLaunchEvent();
    void trackEvent(QString category, QString action, QString label);
};

#endif // LOGGER_H
