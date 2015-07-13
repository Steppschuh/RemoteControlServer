#include "server.h"
#include "settings.h"

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

    if (isLatestServerRunning())
    {
        Settings::Instance()->loadSettings();
    }
}

void Server::finish()
{
    Settings::Instance()->saveSettings();
}

bool Server::isLatestServerRunning()
{
    // ToDo: find out whether latest server is running
    return true;
}

QString Server::getServerName(){
    QString name = "Unknown";
#ifdef __APPLE__
    name = getenv("USER");
#endif
    return name;
}

