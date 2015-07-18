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

//public slots:
//    void InvalidateTimerTick();

private:
    Logger();
    static Logger* instance;

    const QString URL_TRACKER;

    const bool trackEvents;
//    const bool showDebug;
    const int maxItems;

    QDateTime lastUpdate;
//    QTimer *dispatcherTimer;
//    bool dispatcherActive;

    QString lastEntry;

//    void add(QString message, bool isDebug);
//    void invalidateLog();
//    void startInvalidateTimer();
//    void clearLog();
    void trim();
//    QString getString();
//    QString getLastEntry();
//    bool eventTracked(QString result);
};

#endif // LOGGER_H
