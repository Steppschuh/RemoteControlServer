#ifndef LOGGER_H
#define LOGGER_H

#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QTimer>

class Logger : public QObject
{
    Q_OBJECT

public:
    static Logger* Instance();

    QStringList *logMessages;

    void add(const QString &message);
    void trackLaunchEvent();
    void trackEvent(const QString &category, const QString &action, const QString &label);
    QString getLastEntry();
    QString getString();

signals:
    void loggerUpdated();

private:
    Logger();
    static Logger* instance;

    const QString URL_TRACKER;

    const bool trackEvents;
    const int maxItems;

    QString lastEntry;

    void trim();
};

#endif // LOGGER_H
