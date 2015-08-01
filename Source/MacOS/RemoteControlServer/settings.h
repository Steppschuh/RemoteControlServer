#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDomNodeList>
#include <QString>
#include <QStringList>
#include <QTextStream>

class Settings : public QObject
{
    Q_OBJECT

public:
    static Settings *Instance();

    // General
    bool autoStart;
    bool startMinimized;

    bool showGuide;

    // Authentication
    bool useWhitelist;
    bool usePin;
    QString pin;
    QStringList *whitelistedIps;

    // Mouse
    float mouseSensitivity;
    float mouseAcceleration;
    int motionFilter;
    int motionAcceleration;

    // Screen
    int screenQualityFull;
    bool screenBlackWhite;

    // Misc
    bool serialCommands;

    QStringList *customActions;

    void loadSettings();
    void saveSettings();

signals:
    void settingsLoaded();

public slots:
    void setAutostart(bool value);
    void setMinimized(bool value);
    void setMotionFilter(int value);
    void setMotionAcceleration(int value);
    void setMouseAcceleration(int value);
    void setMouseSensitivity(int value);
    void setPin(QString value);
    void setUsePin(bool value);
    void setUseWhitelist(bool value);

private:
    Settings();
    static Settings *instance;

    const QString SETTINGS_PATH;
//    const float OS_AUTO;
//    const float OS_ANDROID;
//    const float OS_BLACKBERRY;
//    const float OS_IOS;
//    const float OS_GENERAL;

//    bool minimizeToTray;
//    bool showTrayNotifications;

//    bool showGuide;
//    char backDesign;
//    char lastBackDesign;

//    bool useWhiteList;
//    bool usePin;
//    QString pin;

    void readSettingsFromFile();
    void saveSettingsToFile();
    void parseSettings(QString &xmlString);
    void assignSetting(QString &name, QString &value);
    void appendSetting(const QString &name, const QString &value, QTextStream &writer);
    void appendWhitelistIp(QString &ip, QTextStream &writer);
    void appendCustomAction(int i, QTextStream &writer);
    void parseCustomActions(QString &xmlString);
    void parseWhitelist(QString &xmlString);
    QDomNodeList prepareParsing(const QString &initialLogMessage, const QString &xmlString, const QString &tagName);
    QString getAppDataDirectory();
    QString getConfigPath();
};

#endif // SETTINGS_H
