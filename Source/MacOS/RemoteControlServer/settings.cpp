#include "converter.h"
#include "logger.h"
#include "settings.h"

#include <QDebug>
#include <QDir>
#include <QDomDocument>
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
    // General
    autoStart = false;
    startMinimized = true;
    showTrayNotifications = true;

    showGuide = true;

    // Authentication
    useWhitelist = false;
    usePin = false;
    pin = "0000";
    whitelistedIps = new QStringList();

    // Mouse
    mouseSensitivity = 1.0;
    mouseAcceleration = 1.0;
    motionFilter = 1;
    motionAcceleration = 3;

    // Screen
    screenQuality = 40;
    screenQualityFull = 60;
    screenScale = 0.5;
    screenScaleFull = 1.0;
    screenBlackWhite = false;

    // Slideshow
    clickOnLaserUp = false;
    pointerDesign = 0;
    cropBlackBorder = true;

    // Media
    defaultMediaPlayer = "";

    // Custom
    customActions = new QStringList();

    // Misc
    serialPortName = "auto";
    serialCommands = false;
    updateAmbientColor = false;
}

void Settings::loadSettings()
{
    Logger::Instance()->add("Loading settings");
    readSettingsFromFile();
    emit settingsLoaded();
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
        appendSetting("startMinimized", Converter::Instance()->boolToString(startMinimized), stream);
        appendSetting("showTrayNotifications", Converter::Instance()->boolToString(showTrayNotifications), stream);
        appendSetting("showGuide", Converter::Instance()->boolToString(showGuide), stream);
        stream << endl;

        // Authentication
        appendSetting("useWhitelist", Converter::Instance()->boolToString(useWhitelist), stream);
        appendSetting("usePin", Converter::Instance()->boolToString(usePin), stream);
        appendSetting("pin", pin, stream);
        stream << endl;

        // Mouse and Pointer
        appendSetting("mouseSensitivity", QString::number(mouseSensitivity, 'f', 2), stream);
        appendSetting("mouseAcceleration", QString::number(mouseAcceleration, 'f', 2), stream);
        appendSetting("motionFilter", QString::number(motionFilter), stream);
        appendSetting("motionAcceleration", QString::number(motionAcceleration), stream);

        // Screen
        appendSetting("screenQuality", QString::number(screenQuality), stream);
        appendSetting("screenQualityFull", QString::number(screenQualityFull), stream);
        appendSetting("screenScale", QString::number(screenScale, 'f', 2), stream);
        appendSetting("screenScaleFull", QString::number(screenScaleFull, 'f', 2), stream);
        appendSetting("screenBlackWhite", Converter::Instance()->boolToString(screenBlackWhite), stream);

        // Slideshow
        appendSetting("clickOnLaserUp", Converter::Instance()->boolToString(clickOnLaserUp), stream);
        appendSetting("pointerDesign", QString::number(pointerDesign), stream);
        appendSetting("cropBlackBorder", Converter::Instance()->boolToString(cropBlackBorder), stream);

        // Media
        appendSetting("defaultMediaPlayer", defaultMediaPlayer, stream);

        // Misc
        appendSetting("serialPortName", serialPortName, stream);
        appendSetting("serialCommands", Converter::Instance()->boolToString(serialCommands), stream);
        appendSetting("updateAmbientColor", Converter::Instance()->boolToString(updateAmbientColor), stream);

        stream << "  <customActions>" << endl;
        for (int i = 0; i < customActions->length(); ++i)
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

void Settings::parseSettings(QString &xmlString)
{
    QDomNodeList xml_nodelist = prepareParsing("Parsing config file", xmlString, "setting");
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

void Settings::assignSetting(QString &name, QString &value)
{
    // General
    if (name == "autoStart")
    {
        autoStart = Converter::Instance()->stringToBool(value);
//        setAutostart(autoStart);
    }
    else if (name == "startMinimized")
    {
        startMinimized = Converter::Instance()->stringToBool(value);
    }
    else if (name == "showTrayNotifications")
    {
        showTrayNotifications = Converter::Instance()->stringToBool(value);
    }
    else if (name == "showGuide")
    {
        showGuide = Converter::Instance()->stringToBool(value);
    }

    // Authentication
    else if (name == "useWhitelist")
    {
        useWhitelist = Converter::Instance()->stringToBool(value);
    }
    else if (name == "usePin")
    {
        usePin = Converter::Instance()->stringToBool(value);
    }
    else if (name == "pin")
    {
        pin = value;
    }

    // Mouse and Pointer
    else if (name == "mouseSensitivity")
    {
        mouseSensitivity = value.toFloat();
    }
    else if (name == "mouseAcceleration")
    {
        mouseAcceleration = value.toFloat();
    }
    else if (name == "motionFilter")
    {
        motionFilter = value.toInt();
    }
    else if (name == "motionAcceleration")
    {
        motionAcceleration = value.toInt();
    }

    // Screen
    else if (name == "screenQuality")
    {
        screenQuality = value.toInt();
    }
    else if (name == "screenQualityFull")
    {
        screenQualityFull = value.toInt();
    }
    else if (name == "screenScale")
    {
        screenScale = value.toFloat();
    }
    else if (name == "screenScaleFull")
    {
        screenScaleFull = value.toFloat();
    }
    else if (name == "screenBlackWhite")
    {
        screenBlackWhite = Converter::Instance()->stringToBool(value);
    }

    // Slideshow
    else if (name == "clickOnLaserUp")
    {
        clickOnLaserUp = Converter::Instance()->stringToBool(value);
    }
    else if (name == "pointerDesign")
    {
        pointerDesign = value.toInt();
    }
    else if (name == "cropBlackBorder")
    {
        cropBlackBorder = Converter::Instance()->stringToBool(value);
    }

    // Media
    else if (name == "defaultMediaPlayer")
    {
        defaultMediaPlayer = value;
    }

    // Misc
    else if (name == "serialPortName")
    {
        serialPortName = value;
    }
    else if (name == "serialCommands")
    {
        serialCommands = Converter::Instance()->stringToBool(value);
    }
    else if (name == "updateAmbientColor")
    {
        updateAmbientColor = Converter::Instance()->stringToBool(value);
    }

    else
    {
        Logger::Instance()->add("Unknown config entry: " + name);
    }
}

void Settings::appendSetting(const QString &name, const QString &value, QTextStream &writer)
{
    writer << "  <setting name=\"" << name << "\" value=\"" << value << "\"/>" << endl;
}

void Settings::appendWhitelistIp(QString &ip, QTextStream &writer)
{
    writer << "    <app ip=\"" << ip << "\"/>" << endl;
}

void Settings::appendCustomAction(int i, QTextStream &writer)
{
    writer << "    <custom path=\"" << customActions->at(i) << "\"/>" << endl;
}

void Settings::parseCustomActions(QString &xmlString)
{
    QDomNodeList xml_nodelist = prepareParsing("Parsing custom actions", xmlString, "custom");
    QString path;
    QDomNode xmlnode;

    for (int i = 0; i < xml_nodelist.length(); ++i)
    {
        xmlnode = xml_nodelist.item(i);
        path = xmlnode.attributes().namedItem("path").nodeValue();
        customActions->append(path);
    }
    Logger::Instance()->add("Custom actions restored, " + QString::number(customActions->size()) +  " actions found");

    if (customActions->size() == 0)
    {
        customActions->append("http://remote-control-collection.com/help/custom/");
        customActions->append("https://www.google.com/?q=This+is+a+sample+custom+action");
        customActions->append("/System/Library/CoreServices/Finder.app");
        customActions->append("/Applications/Calculator.app");
    }
}

void Settings::parseWhitelist(QString &xmlString)
{
    QDomNodeList xml_nodelist = prepareParsing("Parsing whitelist", xmlString, "app");
    QString appIp;
    QDomNode xmlnode;

    for (int i = 0; i < xml_nodelist.length(); ++i)
    {
        xmlnode = xml_nodelist.item(i);
        appIp = xmlnode.attributes().namedItem("ip").nodeValue();
        whitelistedIps->append(appIp);
    }
    Logger::Instance()->add("Whitelist restored, " + QString::number(whitelistedIps->size()) +  " IPs restored");

    if (whitelistedIps->size() == 0)
    {
        whitelistedIps->append("127.0.0.1");
    }
}

QDomNodeList Settings::prepareParsing(const QString &initialLogMessage, const QString &xmlString, const QString &tagName)
{
    Logger::Instance()->add(initialLogMessage);

    QDomDocument *xmldoc = new QDomDocument();
    xmldoc->setContent(xmlString);
    return xmldoc->elementsByTagName(tagName);
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

void Settings::setAutostart(bool value)
{
    autoStart = value;
}

void Settings::setClickOnLaserUp(bool value)
{
    clickOnLaserUp = value;
}

void Settings::setCropBlackBorder(bool value)
{
    cropBlackBorder = value;
}

void Settings::setDefaultMediaPlayer(QString value)
{
    defaultMediaPlayer = value;
}

void Settings::setMinimized(bool value)
{
    startMinimized = value;
}

void Settings::setMotionAcceleration(int value)
{
    motionAcceleration = value / 10;
}

void Settings::setMotionFilter(int value)
{
    motionFilter = value;
}

void Settings::setMouseAcceleration(int value)
{
    mouseAcceleration = float(value) / 10;
}

void Settings::setMouseSensitivity(int value)
{
    mouseSensitivity = float(value);
}

void Settings::setPin(QString value)
{
    pin = value;
}

void Settings::setPointerDesign(int value)
{
    pointerDesign = value;
}

void Settings::setScreenBlackWhite(bool value)
{
    screenBlackWhite = value;
}

void Settings::setScreenFullQuality(int value)
{
    screenQualityFull = value;
}

void Settings::setScreenFullScale(int value)
{
    screenScaleFull = (float) value  / 100;
}

void Settings::setScreenQuality(int value)
{
    screenQuality = value;
}

void Settings::setScreenScale(int value)
{
    screenScale = (float) value / 100;
}

void Settings::setSerialPortName(QString value)
{
    serialPortName = value;
}

void Settings::setSerialCommands(bool value)
{
    serialCommands = value;
}

void Settings::setShowTrayNotifications(bool value)
{
    showTrayNotifications = value;
}

void Settings::setUpdateAmbientColor(bool value)
{
    updateAmbientColor = value;
}

void Settings::setUsePin(bool value)
{
    usePin = value;
}

void Settings::setUseWhitelist(bool value)
{
    useWhitelist = value;
}
