#ifndef UPDATER_H
#define UPDATER_H

#include <QDialog>
#include <QNetworkReply>

namespace Ui {
class Updater;
}

class Updater : public QDialog
{
    Q_OBJECT

public:
    explicit Updater(QWidget *parent = 0);
    ~Updater();

public slots:
    void downloadFinished(QNetworkReply* reply);
    void updateProgressBar(qint64 bytesReceived, qint64 bytesTotal);

private:
    Ui::Updater *ui;

    const QString URL_UPDATE_SERVER = "http://remote-control-collection.com/files/server/RemoteControlServer.app";

    void run();
};

#endif // UPDATER_H
