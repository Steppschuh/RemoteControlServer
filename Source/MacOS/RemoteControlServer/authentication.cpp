#include "authentication.h"
#include "settings.h"

Authentication* Authentication::instance = NULL;

Authentication* Authentication::Instance()
{
    if (!instance)
    {
        instance = new Authentication();
    }
    return instance;
}

Authentication::Authentication()
{
}

bool Authentication::isProtected()
{
    return (Settings::Instance()->usePin || Settings::Instance()->useWhitelist) ? true : false;
}

bool Authentication::isAuthenticated(const QString &ip, const QString &pin)
{
    if (!isProtected())
    {
        return true;
    }
    else
    {
        return ((Settings::Instance()->useWhitelist && isWhitelisted(ip)) || (Settings::Instance()->usePin && Settings::Instance()->pin == pin))
                ? true : false;

    }
}

bool Authentication::isWhitelisted(const QString &ip)
{
    return (Settings::Instance()->whitelistedIps->contains(ip)) ? true : false;
}

bool Authentication::isWhitelisted(App *app)
{
    return isWhitelisted(app->getIp());
}

void Authentication::addToWhiteList(QString ip)
{
    if (!isWhitelisted(ip))
    {
        Settings::Instance()->whitelistedIps->append(ip);
    }
}

void Authentication::removeFromWhiteList(QString ip)
{
    if (isWhitelisted(ip))
    {
        Settings::Instance()->whitelistedIps->removeOne(ip);
    }
}
