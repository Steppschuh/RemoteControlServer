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
    if (statusCode.isValid() && statusCode.toInt() == 200 && reply->size() > 10000)
    {
        std::cout << "Download finished.\nExtracting application...\n";

        QString appPath = "/Applications/RemoteControlServer.app";

        QFile *file = new QFile(appPath + ".zip");
        if (file->open(QIODevice::WriteOnly))
        {
            file->write(reply->readAll());
        }
        file->setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner
                             | QFileDevice::ReadGroup | QFileDevice::ExeGroup
                             | QFileDevice::ReadOther | QFileDevice::ExeOther);

        QProcess unzipProcess;
        unzipProcess.setProcessChannelMode(QProcess::ForwardedChannels);
        unzipProcess.start("unzip -o -q " + file->fileName() + " -d /Applications/");
        unzipProcess.waitForFinished();
        unzipProcess.close();

        // no need to rename, unzip has overwritten the existing server
        //QFile::remove(appPath);
        //file->rename(appPath);

        file->close();
        std::cout << "Extraction done.\nUpdated application has been stored to:\n   " << appPath.toUtf8().constData() << "\n";
        std::cout << "Starting the new Remote Control Server.\n\n";

        QProcess serverProcess;
        serverProcess.startDetached("open " + appPath);
    }
    else
    {
        std::cout << "Update failed.\nPlease update the server manually, instructions can be found at http://remote-control-collection.com/help/update/\n\n";
    }

    emit finished();
}

void Updater::run()
{
    std::cout << "\n\nDownloading the latest version of the Remote Control Server...\n";


    QNetworkAccessManager *webManager = new QNetworkAccessManager();
    connect(webManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));

    QNetworkRequest request(URL_UPDATE_SERVER);
    webManager->get(request);
}
