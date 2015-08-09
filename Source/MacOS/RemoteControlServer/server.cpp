#include "network.h"
#include "remote.h"
#include "screenshot.h"
#include "serial.h"
#include "server.h"
#include "settings.h"
#include "tcp.h"
#include "updater.h"

#include <QtConcurrent>
#include <QDesktopServices>
#include <QProcess>
#include <QSysInfo>

#include <QDebug>

Server* Server::instance = NULL;

Server* Server::Instance()
{
    if (!instance)
    {
        instance = new Server();
    }
    return instance;
}

Server::Server()
{
    Logger::Instance()->add("Initializing server");
    status = "Initializing";
    userName = "";

    if (isLatestServerRunning())
    {
        Settings::Instance()->loadSettings();
        Network::Instance();                            // In order to initialize the network
        Updater::Instance()->checkForUpdates(30);
        Remote::Instance()->initializeLastCommand();    // In order to initialize the remote
        Screenshot::Instance()->startUpdateColorTimer();
        QtConcurrent::run(this, &Server::initializeAsync);
    }
}

void Server::initializeAsync()
{
    apps = new QList<App*>();
    Logger::Instance()->trackLaunchEvent();
    Logger::Instance()->add("Server ready");
    status = "Ready";
    showNotification("Server ready", "The Remote Control Server has been started and is waiting for a connection");
}

void Server::finish()
{
    Settings::Instance()->saveSettings();
    TCP::Instance()->stopListener();
    Serial::Instance()->closeSerialPort();
}

bool Server::isLatestServerRunning()
{
    // ToDo: find out whether latest server is running
    return true;
}

App* Server::getApp(QString &ip)
{
    for (int i = 0; i < this->apps->length(); ++i)
    {
        if (apps->at(i)->ip == ip)
        {
            return apps->at(i);
        }
    }
    App *app = new App();
    app->ip = ip;
    this->apps->append(app);
    return app;
}

App* Server::getCurrentApp()
{
    return getApp(Remote::Instance()->lastCommand->source);
}

QString Server::getServerVersionName()
{
    return APP_VERSION;
}

QString Server::getServerOs()
{
    return QSysInfo::prettyProductName();
}

QString Server::getServerName(){
    QString name = "Unknown";
#ifdef __APPLE__
    name = getenv("USER");
#endif
    return name;
}

void Server::startProcess(QString path)
{
    if (path.startsWith("http")) QDesktopServices::openUrl(path);   // includes paths with https
    else QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void Server::showErrorDialog(QString title, QString text){
    emit newErrorMessage(title, text);
}

void Server::showNotification(QString title, QString text)
{
    emit newNotification(title, text);
}

