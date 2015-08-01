#include "logger.h"
#include "mainwindow.h"
#include "settings.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QScrollBar>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initializeSystemTrayIcon();
    connectUiToServer();

    closeEventCameFromSystemTrayIcon = false;

    Server::Instance()->userName = Server::Instance()->getServerName();
}

void MainWindow::connectUiToServer()
{
    // Signals from the server to the ui
    connect(Logger::Instance(), SIGNAL(loggerUpdated()), this, SLOT(updateLogMessages()));

    // Signals from the ui to the server
    connect(ui->startServerOnLogin, SIGNAL(clicked(bool)), Settings::Instance(), SLOT(setAutostart(bool)));
    connect(ui->startServerMinimized, SIGNAL(clicked(bool)), Settings::Instance(), SLOT(setMinimized(bool)));
    connect(ui->enableWhitelist, SIGNAL(clicked(bool)), Settings::Instance(), SLOT(setUseWhitelist(bool)));
    connect(ui->enablePin, SIGNAL(clicked(bool)), Settings::Instance(), SLOT(setUsePin(bool)));
    connect(ui->showPin, SIGNAL(clicked(bool)), this, SLOT(setVisibleStateOfPinBox(bool)));
    connect(ui->pinDisplay, SIGNAL(textChanged(QString)), Settings::Instance(), SLOT(setPin(QString)));
}

void MainWindow::initializeSystemTrayIcon()
{
    this->clearFocus();
    if (!QSystemTrayIcon::isSystemTrayAvailable())
                QMessageBox::warning(this, tr("System tray is unavailable"),
                                          tr("System tray unavailable"));
    trayIcon = new TrayIcon();
    connect(trayIcon->openSettingsWindow, SIGNAL(triggered()), this, SLOT(customShow()));
    connect(trayIcon->quit, SIGNAL(triggered()), this, SLOT(customClose()));
    connect(Settings::Instance(), SIGNAL(settingsLoaded()), this, SLOT(initUiWithSettings()));

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
    this->raise();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (closeEventCameFromSystemTrayIcon)
    {
        Server::Instance()->finish();
        qDebug() << "stopping server";
        event->accept();
    }
    else
    {
        if (event->spontaneous())
        {
            this->hide();
        }
        event->ignore();
    }
}

void MainWindow::initUiWithSettings()
{
    ui->enableWhitelist->setChecked(Settings::Instance()->useWhitelist);
    ui->enablePin->setChecked(Settings::Instance()->usePin);
    ui->pinDisplay->setText(Settings::Instance()->pin);
    ui->startServerOnLogin->setChecked(Settings::Instance()->autoStart);
    ui->startServerMinimized->setChecked(Settings::Instance()->startMinimized);

    if (!Settings::Instance()->startMinimized) customShow();
}

void MainWindow::paintEvent(QPaintEvent * event)
{
    ui->logTextView->setText(Logger::Instance()->getString());
    QScrollBar *sb = ui->logTextView->verticalScrollBar();
    sb->setValue(sb->maximum());
}

void MainWindow::setVisibleStateOfPinBox(bool value)
{
    if (value)  ui->pinDisplay->setEchoMode(QLineEdit::Normal);
    else        ui->pinDisplay->setEchoMode(QLineEdit::Password);
}

void MainWindow::updateLogMessages()
{
    ui->logTextView->setText(Logger::Instance()->getString());
    QScrollBar *sb = ui->logTextView->verticalScrollBar();
    sb->setValue(sb->maximum());
}
