#include "updater.h"

#include <iostream>

#include <QFile>

Updater::Updater()
{
}

Updater::~Updater()
{
    delete ui;
}

void Updater::downloadFinished(QNetworkReply* reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (statusCode.isValid() && statusCode.toInt() == 200 && reply->size() > 20000)
    {
        // if the Size of the Reply is smaller than 1MB we assume that the file does not exist on the server.
        // if your File should be smaller than 1MB, change the number
        QFile *file = new QFile("/Applications/RemoteControlServerNew.app");
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

        std::cout << "Update finished - Have fun using the new version of the Remote Control Server.\n";
    }
    else
    {
        std::cout << "Update failed - The Download of the new Version of the Remote Control Server failed.\nPlease update the server manually, instructions can be found at http://remote-control-collection.com/help/update/";
    }

    emit finished();
}

void Updater::run()
{
    QNetworkAccessManager *webManager = new QNetworkAccessManager();
    connect(webManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));

    QNetworkRequest request(URL_UPDATE_SERVER);
    webManager->get(request);
}
