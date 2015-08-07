#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "server.h"
#include "trayicon.h"

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
    void copyLogTextToClipboard();
    void customClose();
    void customShow();
    void initUiWithSettings();
    void setVisibleStateOfPinBox(bool value);
    void showNewErrorDialog(QString title, QString message);
    void updateLogMessages();

private:
    Ui::MainWindow *ui;
    bool closeEventCameFromSystemTrayIcon;

    TrayIcon *trayIcon;

    void connectUiToServer();
    void initializeSystemTrayIcon();
    void setLog(QString message);
};

#endif // MAINWINDOW_H
