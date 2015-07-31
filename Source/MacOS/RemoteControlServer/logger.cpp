#include "logger.h"

Logger* Logger::instance = NULL;

Logger* Logger::Instance()
{
    if (!instance)
    {
        instance = new Logger();
    }
    return instance;
}

Logger::Logger() :
  URL_TRACKER("http://remote-control-collection.com/api/tracker/"),
  trackEvents(true),
//  showDebug(true),
  maxItems(100)
{
//    dispatcherActive = false;
    lastEntry = "";

    lastUpdate = QDateTime().currentDateTime();
    logMessages = new QStringList();
}

void Logger::add(const QString &message)
{
    QString timestamp = QTime().currentTime().toString();
    lastEntry = message;
    logMessages->append(timestamp + ": " + message);
    Logger::trim();
    emit loggerUpdated();
}

void Logger::trim()
{
    if (logMessages->size() > maxItems) logMessages->removeAt(0);
}

QString Logger::getString()
{
    QString result = "";
    foreach (QString message, *logMessages) {
        result += " " + message + "\n";
    }
    return result;
}

QString Logger::getLastEntry()
{
    const int maxLength = 30;
    QString entry = lastEntry;

    while (entry.length() > maxLength)
    {
        if (entry.contains(" "))
        {
            entry.truncate(entry.lastIndexOf(" "));
        }
        else
        {
            entry.truncate(maxLength - 3);
        }
        entry.append("...");
    }
    return entry;
}

void Logger::trackLaunchEvent()
{
    Logger::trackEvent("Server", "Launch", APP_VERSION);
}

void Logger::trackEvent(const QString &category, const QString &action, const QString &label)
{
    if (trackEvents)
    {
       QString url = URL_TRACKER
                    + "?category=" + category
                    + "&action=" + action
                    + "&label=" + label;
       //Network::Instance()->loadInBrowser(url);
    }
}

