#include "converter.h"
#include "logger.h"
#include "settings.h"

#include <QDebug>
#include <QDir>
#include <QDomDocument>
#include <QDomNodeList>
#include <QFile>
#include <QStandardPaths>

Settings* Settings::instance = NULL;

Settings* Settings::Instance()
{
    if (!instance)
    {
        instance = new Settings();
    }
    return instance;
}

Settings::Settings() :
    SETTINGS_PATH("config.xml")
{
    autoStart = false;
}

void Settings::loadSettings()
{
    Logger::Instance()->add("Loading settings");
    whitelistedIps = new QStringList();
    readSettingsFromFile();
}

void Settings::saveSettings()
{
    saveSettingsToFile();
}

void Settings::readSettingsFromFile()
{
    QString path = getConfigPath();
    QString xmlString = "";
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Logger::Instance()->add("Error while reading settings file from:/n" + path);
    }
    else
    {
        QTextStream textStream(&file);
        xmlString = textStream.readAll();
        parseSettings(xmlString);
    }
    parseCustomActions(xmlString);
    parseWhitelist(xmlString);

}

void Settings::saveSettingsToFile()
{
    QString path = getConfigPath();
    if (!QDir(getAppDataDirectory()).exists()){
        QDir().mkdir(getAppDataDirectory());
    }
    QFile file(path);
    if (!file.open(QIODevice::ReadWrite))
    {
        Logger::Instance()->add("Error while saving settings file to:/n" + path);
    }
    else
    {
            QTextStream stream(&file);
            stream << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << endl;
            stream << "<settings>" << endl;

            // General
            appendSetting("autoStart", Converter::Instance()->boolToString(autoStart), stream);
            stream << endl;

            stream << "  <customActions>" << endl;
            for (int i = 0; i < customActions.length(); ++i)
            {
                appendCustomAction(i, stream);
            }
            stream << "  </customActions>" << endl;

            stream << "  <whitelist>" << endl;
            foreach (QString appIp, *whitelistedIps)
            {
                appendWhitelistIp(appIp, stream);
            }
            stream << "  </whitelist>" << endl;
            stream << "</settings>";
            stream.flush();
    }
}

void Settings::parseSettings(QString xmlString)
{
    Logger::Instance()->add("Parsing config file");

    QDomDocument *xmldoc = new QDomDocument();
    xmldoc->setContent(xmlString);
    QDomNodeList xml_nodelist = xmldoc->elementsByTagName("setting");
    QString itemName, itemValue;
    QDomNode xmlnode;

    for (int i = 0; i < xml_nodelist.length(); ++i)
    {
        xmlnode = xml_nodelist.item(i);
        itemName = xmlnode.attributes().namedItem("name").nodeValue();
        itemValue = xmlnode.attributes().namedItem("value").nodeValue();
        assignSetting(itemName, itemValue);
    }
    Logger::Instance()->add("Settings restored");
}

void Settings::assignSetting(QString name, QString value)
{
    // General
    if (name == "autoStart")
    {
        autoStart = Converter::Instance()->stringToBool(value);
        setAutostart(autoStart);
    }

    else
    {
        Logger::Instance()->add("Unknown config entry: " + name);
    }
}

void Settings::appendSetting(QString name, QString value, QTextStream &writer)
{
    writer << "  <setting name=\"" << name << "\" value=\"" << value << "\"/>" << endl;
}

void Settings::appendWhitelistIp(QString ip, QTextStream &writer)
{
    writer << "    <app ip=\"" << ip << "\"/>" << endl;
}

void Settings::appendCustomAction(int i, QTextStream &writer)
{
    writer << "    <custom path=\"" << customActions.at(i) << "\"/>" << endl;
}

void Settings::parseCustomActions(QString xmlString)
{
    Logger::Instance()->add("Parsing custom actions");

    QDomDocument *xmldoc = new QDomDocument();
    xmldoc->setContent(xmlString);
    QDomNodeList xml_nodelist = xmldoc->elementsByTagName("custom");
    QString path;
    QDomNode xmlnode;

    for (int i = 0; i < xml_nodelist.length(); ++i)
    {
        xmlnode = xml_nodelist.item(i);
        path = xmlnode.attributes().namedItem("path").nodeValue();
        customActions.append(path);
    }
    Logger::Instance()->add("Custom actions restored, " + QString(customActions.size()) +  " actions found");

    if (customActions.size() == 0)
    {
        customActions.append("http://remote-control-collection.com/help/custom/");
        customActions.append("https://www.google.com/?q=This+is+a+sample+custom+action");
        customActions.append("explorer");
        customActions.append("calc");
    }
}

void Settings::parseWhitelist(QString xmlString)
{
    Logger::Instance()->add("Parsing whitelist");

    QDomDocument *xmldoc = new QDomDocument();
    xmldoc->setContent(xmlString);
    QDomNodeList xml_nodelist = xmldoc->elementsByTagName("custom");
    QString appIp;
    QDomNode xmlnode;

    for (int i = 0; i < xml_nodelist.length(); ++i)
    {
        xmlnode = xml_nodelist.item(i);
        appIp = xmlnode.attributes().namedItem("ip").nodeValue();
        whitelistedIps->append(appIp);
    }
    Logger::Instance()->add("Whitelist restored, " + QString(whitelistedIps->size()) +  " IPs restored");

    if (whitelistedIps->size() == 0)
    {
        whitelistedIps->append("127.0.0.1");
    }
}

QString Settings::getAppDataDirectory()
{
    return QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0];
}

QString Settings::getConfigPath()
{
#ifdef __APPLE__
    QString slash = "/";
#endif
    return getAppDataDirectory() + slash + SETTINGS_PATH;
}

bool Settings::setAutostart(bool value)
{
    return false;
}
