#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QMessageBox>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initializeSystemTrayIcon();

    closeEventCameFromSystemTrayIcon = false;

    Server::Instance()->userName = Server::Instance()->getServerName();
}

void MainWindow::initializeSystemTrayIcon()
{
    if (!QSystemTrayIcon::isSystemTrayAvailable())
                QMessageBox::warning(this, tr("System tray is unavailable"),
                                          tr("System tray unavailable"));
    trayIcon = new TrayIcon();
    connect(trayIcon->openSettingsWindow, SIGNAL(triggered()), this, SLOT(customShow()));
    connect(trayIcon->quit, SIGNAL(triggered()), this, SLOT(customClose()));

    this->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::CustomizeWindowHint); // disables the minimize and maximize buttons
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::customClose()
{
    closeEventCameFromSystemTrayIcon = true;
    this->close();
}

void MainWindow::customShow()
{
    this->show();
    this->activateWindow();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (closeEventCameFromSystemTrayIcon)
    {
        Server::Instance()->finish();
    }
    else
    {
        event->ignore();
        this->hide();
    }
}
