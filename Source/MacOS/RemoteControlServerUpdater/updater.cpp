#include "updater.h"
#include "ui_updater.h"

#include <QFile>
#include <QMessageBox>

Updater::Updater(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Updater)
{
    ui->setupUi(this);
    run();
}

Updater::~Updater()
{
    delete ui;
}

void Updater::downloadFinished(QNetworkReply* reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (statusCode.isValid() && statusCode.toInt() == 200 && reply->size() > 1000000)
    {
        // if the Size of the Reply is smaller than 1MB we assume that the file does not exist on the server.
        // if your File should be smaller than 1MB, change the number
        QFile *file = new QFile("/Applications/RemoteControlServer2.app");
        if (file->open(QIODevice::WriteOnly))
        {
            file->write(reply->readAll());
        }
        file->setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner
                             | QFileDevice::ReadGroup | QFileDevice::ExeGroup
                             | QFileDevice::ReadOther | QFileDevice::ExeOther);

        QString appPath = "/Applications/RemoteControlServer.app";
        QFile::remove(appPath);
        file->rename(appPath);
        file->close();
    }
    else
    {
        QMessageBox msgBox(QMessageBox::Critical, "Update failed",
                           "The Download of the new Version of the Remote Control Server failed. The Updater is finishing now");
        msgBox.exec();
    }

    close();
}

void Updater::run()
{
    QNetworkAccessManager *webManager = new QNetworkAccessManager();
    connect(webManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));

    QNetworkRequest request(URL_UPDATE_SERVER);
    QNetworkReply *reply = webManager->get(request);

    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateProgressBar(qint64,qint64)));
}

void Updater::updateProgressBar(qint64 bytesReceived, qint64 bytesTotal)
{
    ui->progressBar->setMaximum(bytesTotal);
    ui->progressBar->setValue(bytesReceived);
}
