#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "customwindow.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "server.h"
#include "trayicon.h"
#include "whitelistwindow.h"

#include <QMainWindow>
#include <QString>
#include <QSystemTrayIcon>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString userName;

    void closeEvent(QCloseEvent *event);

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void changePointer(int index);
    void connectSetupguideAction();
    void connectTroubleshootingAction();
    void copyLogTextToClipboard();
    void customClose();
    void customShow();
    void helpHelpAction();
    void helpFAQAction();
    void helpContactAction();
    void helpGithubAction();
    void initUiWithSettings();
    void initUpdateNewestVersion();
    void ledOn();
    void ledOff();
    void openMediaFileDialog();
    void reopenSerialPort();
    void setVisibleStateOfPinBox(bool value);
    void showNewDialog(QString title, QString message);
    void showNewErrorDialog(QString title, QString message);
    void startUpdateHelpAction();
    void startUpdateChangelogAction();
    void updateLogMessages();

private:
    Ui::MainWindow *ui;
    bool closeEventCameFromSystemTrayIcon;

    CustomWindow *customWindow;
    TrayIcon *trayIcon;
    WhitelistWindow *whitelistWindow;

    void connectUiToServer();
    void initializeSystemTrayIcon();
    void setLog(QString message);
};

#endif // MAINWINDOW_H
