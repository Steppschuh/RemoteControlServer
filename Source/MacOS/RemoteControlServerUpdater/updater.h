#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QNetworkReply>
#include <QProcess>

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

    const QString URL_UPDATE_SERVER = "http://remote-control-collection.com/files/server/RemoteControlServer.app.zip";

    void displayErrorMessage();
};

#endif // UPDATER_H
