#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QString>

#include "app.h"

class Authentication
{
public:
    static Authentication *Instance();

    bool isAuthenticated(const QString &ip, const QString &pin);

private:
    Authentication();
    static Authentication *instance;

    bool isProtected();
    bool isWhitelisted(const QString &ip);
    bool isWhitelisted(App *app);
    void addToWhiteList(QString ip);
    void removeFromWhiteList(QString ip);
};

#endif // AUTHENTICATION_H
