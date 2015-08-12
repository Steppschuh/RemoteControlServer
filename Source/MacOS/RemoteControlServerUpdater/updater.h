#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QNetworkReply>

namespace Ui {
class Updater;
}

class Updater : public QObject
{
    Q_OBJECT

public:
    Updater();
    ~Updater();

public slots:
    void run();
    void downloadFinished(QNetworkReply* reply);

signals:
    void finished();

private:
    Ui::Updater *ui;

    const QString URL_UPDATE_SERVER = "https://github.com/Steppschuh/RemoteControlServer/tree/master/Downloads/Mac%20OS/RemoteControlServer.app";
};

#endif // UPDATER_H
