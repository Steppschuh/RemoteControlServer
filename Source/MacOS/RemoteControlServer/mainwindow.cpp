#include "logger.h"
#include "mainwindow.h"
#include "media.h"
#include "network.h"
#include "settings.h"
#include "ui_mainwindow.h"

#include <QClipboard>
#include <QCloseEvent>
#include <QFileInfo>
#include <QMessageBox>
#include <QScrollBar>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->slideShowSelectPointer->addItem("Red point");

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

    connect(ui->mouseSensitivity, SIGNAL(sliderMoved(int)), Settings::Instance(), SLOT(setMouseSensitivity(int)));
    connect(ui->mouseAcceleration, SIGNAL(sliderMoved(int)), Settings::Instance(), SLOT(setMouseAcceleration(int)));
    connect(ui->motionFilter, SIGNAL(sliderMoved(int)), Settings::Instance(), SLOT(setMotionFilter(int)));
    connect(ui->motionAcceleration, SIGNAL(sliderMoved(int)), Settings::Instance(), SLOT(setMotionAcceleration(int)));

    connect(ui->screenNormalQuality, SIGNAL(sliderMoved(int)), Settings::Instance(), SLOT(setScreenQuality(int)));
    connect(ui->screenNormalScale, SIGNAL(sliderMoved(int)), Settings::Instance(), SLOT(setScreenScale(int)));
    connect(ui->screenFullQuality, SIGNAL(sliderMoved(int)), Settings::Instance(), SLOT(setScreenFullQuality(int)));
    connect(ui->screenFullScale, SIGNAL(sliderMoved(int)), Settings::Instance(), SLOT(setScreenFullScale(int)));
    connect(ui->screenGrayscale, SIGNAL(clicked(bool)), Settings::Instance(), SLOT(setScreenBlackWhite(bool)));

    connect(ui->slideShowClickWhenReleased, SIGNAL(clicked(bool)), Settings::Instance(), SLOT(setClickOnLaserUp(bool)));
    connect(ui->slideShowSelectPointer, SIGNAL(currentIndexChanged(int)), Settings::Instance(), SLOT(setPointerDesign(int)));
    connect(ui->slideShowCropBlackBorder, SIGNAL(clicked(bool)), Settings::Instance(), SLOT(setCropBlackBorder(bool)));

    connect(ui->defaultMediaPlayer, SIGNAL(textChanged(QString)), Settings::Instance(), SLOT(setDefaultMediaPlayer(QString)));

    connect(ui->enableSerialCommands, SIGNAL(clicked(bool)), Settings::Instance(), SLOT(setSerialCommands(bool)));
    connect(ui->serialPortNameText, SIGNAL(textChanged(QString)), Settings::Instance(), SLOT(setSerialPortName(QString)));
    connect(ui->updateAmbientColor, SIGNAL(clicked(bool)), Settings::Instance(), SLOT(setUpdateAmbientColor(bool)));

    // Signals from the UI to the UI
    connect(ui->logCopyToClipboard, SIGNAL(released()), this, SLOT(copyLogTextToClipboard()));
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

void MainWindow::copyLogTextToClipboard()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->logTextView->toPlainText());
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
    ui->startServerOnLogin->setChecked(Settings::Instance()->autoStart);
    ui->startServerMinimized->setChecked(Settings::Instance()->startMinimized);

    ui->enableWhitelist->setChecked(Settings::Instance()->useWhitelist);
    ui->enablePin->setChecked(Settings::Instance()->usePin);
    ui->pinDisplay->setText(Settings::Instance()->pin);

    ui->motionAcceleration->setValue(Settings::Instance()->motionAcceleration * 10);
    ui->motionFilter->setValue(Settings::Instance()->motionFilter);
    ui->mouseAcceleration->setValue(Settings::Instance()->mouseAcceleration * 10);
    ui->mouseSensitivity->setValue(Settings::Instance()->mouseSensitivity);

    ui->screenNormalQuality->setValue(Settings::Instance()->screenQuality);
    ui->screenNormalScale->setValue(Settings::Instance()->screenScale * 100);
    ui->screenFullQuality->setValue(Settings::Instance()->screenQualityFull);
    ui->screenFullScale->setValue(Settings::Instance()->screenScaleFull * 100);
    ui->screenGrayscale->setChecked(Settings::Instance()->screenBlackWhite);

    ui->slideShowClickWhenReleased->setChecked(Settings::Instance()->clickOnLaserUp);
    ui->slideShowSelectPointer->setCurrentIndex(Settings::Instance()->pointerDesign);
    ui->slideShowCropBlackBorder->setChecked(Settings::Instance()->cropBlackBorder);

    QFileInfo checkFile(Settings::Instance()->defaultMediaPlayer);
    if (!checkFile.exists() || !checkFile.isFile())
    {
        ui->defaultMediaPlayer->setText(Media::Instance()->getDefaultMediaPlayerPath());
    }
    else
    {
        ui->defaultMediaPlayer->setText(Settings::Instance()->defaultMediaPlayer);
    }

    ui->enableSerialCommands->setChecked(Settings::Instance()->serialCommands);
    ui->serialPortNameText->setText(Settings::Instance()->serialPortName);
    ui->updateAmbientColor->setChecked(Settings::Instance()->updateAmbientColor);

    if (!Settings::Instance()->startMinimized) customShow();
}

void MainWindow::paintEvent(QPaintEvent * event)
{
    ui->serverIpLabel->setText(Network::Instance()->getServerIp());
}

void MainWindow::setVisibleStateOfPinBox(bool value)
{
    if (value)  ui->pinDisplay->setEchoMode(QLineEdit::Normal);
    else        ui->pinDisplay->setEchoMode(QLineEdit::Password);
}

void MainWindow::showNotification(QString title, QString text)
{
    // todo
}

void MainWindow::updateLogMessages()
{
    ui->logTextView->setText(Logger::Instance()->getString());
    QScrollBar *sb = ui->logTextView->verticalScrollBar();
    sb->setValue(sb->maximum());
}
