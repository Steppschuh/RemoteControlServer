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

public slots:
    void customClose();
    void customShow();

private:
    Ui::MainWindow *ui;
    bool closeEventCameFromSystemTrayIcon;

    TrayIcon *trayIcon;

    void initializeSystemTrayIcon();
};

#endif // MAINWINDOW_H
