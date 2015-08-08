#include "helper.h"
#include "logger.h"
#include "mainwindow.h"
#include "media.h"
#include "network.h"
#include "serial.h"
#include "settings.h"
#include "ui_mainwindow.h"
#include "updater.h"

#include <QClipboard>
#include <QCloseEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QScrollBar>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Remote Control Server");
    setWindowIcon(QIcon(":/icons/icon_server_256.png"));

    initializeSystemTrayIcon();
    customWindow = new CustomWindow();
    whitelistWindow = new WhitelistWindow();

    closeEventCameFromSystemTrayIcon = false;

    Server::Instance()->userName = Server::Instance()->getServerName();
    ui->updateInstalledVersion->setText(Server::Instance()->getServerVersionName());

    connectUiToServer();
}

void MainWindow::connectUiToServer()
{
    // Signals from the server to the ui
    connect(Logger::Instance(), SIGNAL(loggerUpdated()), this, SLOT(updateLogMessages()));
    connect(Server::Instance(), SIGNAL(newNotification(QString,QString)), trayIcon, SLOT(showNotification(QString,QString)));
    connect(Server::Instance(), SIGNAL(newErrorMessage(QString,QString)), this, SLOT(showNewErrorDialog(QString,QString)));
    connect(Updater::Instance(), SIGNAL(hasUpdatesParsed()), this, SLOT(initUpdateNewestVersion()));

    // Signals from the ui to the server
    // Settings
    // General
    connect(ui->startServerOnLogin, SIGNAL(clicked(bool)), Settings::Instance(), SLOT(setAutostart(bool)));
    connect(ui->startServerMinimized, SIGNAL(clicked(bool)), Settings::Instance(), SLOT(setMinimized(bool)));
    connect(ui->showNotifications, SIGNAL(clicked(bool)), Settings::Instance(), SLOT(setShowTrayNotifications(bool)));

    // Protection
    connect(ui->enableWhitelist, SIGNAL(clicked(bool)), Settings::Instance(), SLOT(setUseWhitelist(bool)));
    connect(ui->enablePin, SIGNAL(clicked(bool)), Settings::Instance(), SLOT(setUsePin(bool)));
    connect(ui->showPin, SIGNAL(clicked(bool)), this, SLOT(setVisibleStateOfPinBox(bool)));
    connect(ui->pinDisplay, SIGNAL(textChanged(QString)), Settings::Instance(), SLOT(setPin(QString)));
    connect(ui->manageWhitelistButton, SIGNAL(clicked(bool)), whitelistWindow, SLOT(customShow()));

    // Mouse
    connect(ui->mouseSensitivity, SIGNAL(sliderMoved(int)), Settings::Instance(), SLOT(setMouseSensitivity(int)));
    connect(ui->mouseAcceleration, SIGNAL(sliderMoved(int)), Settings::Instance(), SLOT(setMouseAcceleration(int)));
    connect(ui->motionFilter, SIGNAL(sliderMoved(int)), Settings::Instance(), SLOT(setMotionFilter(int)));
    connect(ui->motionAcceleration, SIGNAL(sliderMoved(int)), Settings::Instance(), SLOT(setMotionAcceleration(int)));

    // Screenshot
    connect(ui->screenNormalQuality, SIGNAL(sliderMoved(int)), Settings::Instance(), SLOT(setScreenQuality(int)));
    connect(ui->screenNormalScale, SIGNAL(sliderMoved(int)), Settings::Instance(), SLOT(setScreenScale(int)));
    connect(ui->screenFullQuality, SIGNAL(sliderMoved(int)), Settings::Instance(), SLOT(setScreenFullQuality(int)));
    connect(ui->screenFullScale, SIGNAL(sliderMoved(int)), Settings::Instance(), SLOT(setScreenFullScale(int)));
    connect(ui->screenGrayscale, SIGNAL(clicked(bool)), Settings::Instance(), SLOT(setScreenBlackWhite(bool)));

    // Slideshow
    connect(ui->slideShowClickWhenReleased, SIGNAL(clicked(bool)), Settings::Instance(), SLOT(setClickOnLaserUp(bool)));
    connect(ui->slideShowSelectPointer, SIGNAL(currentIndexChanged(int)), Settings::Instance(), SLOT(setPointerDesign(int)));
    connect(ui->slideShowSelectPointer, SIGNAL(currentIndexChanged(int)), this, SLOT(changePointer(int)));
    connect(ui->slideShowCropBlackBorder, SIGNAL(clicked(bool)), Settings::Instance(), SLOT(setCropBlackBorder(bool)));

    // Media
    connect(ui->defaultMediaPlayer, SIGNAL(textChanged(QString)), Settings::Instance(), SLOT(setDefaultMediaPlayer(QString)));
    connect(ui->browseMediaPlayer, SIGNAL(clicked(bool)), this, SLOT(openMediaFileDialog()));

    // Custom
    connect(ui->manageCustomActions, SIGNAL(clicked(bool)), customWindow, SLOT(customShow()));

    // Misc
    connect(ui->enableSerialCommands, SIGNAL(clicked(bool)), Settings::Instance(), SLOT(setSerialCommands(bool)));
    connect(ui->serialPortNameText, SIGNAL(textChanged(QString)), Settings::Instance(), SLOT(setSerialPortName(QString)));
    connect(ui->updateAmbientColor, SIGNAL(clicked(bool)), Settings::Instance(), SLOT(setUpdateAmbientColor(bool)));
    connect(ui->ReopenPortButton, SIGNAL(clicked(bool)), this, SLOT(reopenSerialPort()));
    connect(ui->LEDonButton, SIGNAL(clicked(bool)), this, SLOT(ledOn()));
    connect(ui->LEDoffButton, SIGNAL(clicked(bool)), this, SLOT(ledOff()));

    // Update
    connect(ui->updateInstall, SIGNAL(clicked(bool)), Updater::Instance(), SLOT(startUpdater()));
    connect(ui->updateShowChangelog, SIGNAL(clicked(bool)), this, SLOT(startUpdateChangelogAction()));
    connect(ui->updateHelp, SIGNAL(clicked(bool)), this, SLOT(startUpdateHelpAction()));

    // Help
    connect(ui->helpHelp, SIGNAL(clicked(bool)), this, SLOT(helpHelpAction()));
    connect(ui->helpFAQ, SIGNAL(clicked(bool)), this, SLOT(helpFAQAction()));
    connect(ui->helpContact, SIGNAL(clicked(bool)), this, SLOT(helpContactAction()));
    connect(ui->helptGithubIssue, SIGNAL(clicked(bool)), this, SLOT(helpGithubAction()));

    // Connect
    connect(ui->connectSetupGuide, SIGNAL(clicked(bool)), this, SLOT(connectSetupguideAction()));
    connect(ui->connectTroubleshooting, SIGNAL(clicked(bool)), this, SLOT(connectTroubleshootingAction()));

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

void MainWindow::changePointer(int index)
{
    QString path = "";
    if (index == 0)
    {
        path = ":icons/pointer_white.png";
    }
    else if (index == 1)
    {
        path = ":icons/pointer.png";
    }

    if (path.length() <= 0) return;

    QPixmap *pixmap = new QPixmap(path);
    ui->pointerLabel->setPixmap(*pixmap);
    delete pixmap;
}

void MainWindow::connectSetupguideAction()
{
    Logger::Instance()->trackEvent("Server", "Click", "Connection help page");
    Server::Instance()->startProcess("http://remote-control-collection.com/help/connect/");
}

void MainWindow::connectTroubleshootingAction()
{
    Logger::Instance()->trackEvent("Server", "Click", "Troubleshooting page");
    Server::Instance()->startProcess("http://remote-control-collection.com/help/troubleshooting/");
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

void MainWindow::helpHelpAction()
{
    Logger::Instance()->trackEvent("Server", "Click", "Help page");
    Server::Instance()->startProcess("http://remote-control-collection.com/help/");
}

void MainWindow::helpFAQAction()
{
    Logger::Instance()->trackEvent("Server", "Click", "FAQ page");
    Server::Instance()->startProcess("http://remote-control-collection.com/help/faq/");
}

void MainWindow::helpContactAction()
{
    Logger::Instance()->trackEvent("Server", "Click", "Send support mail");
    QString body = Helper::Instance()->generateHelpMail() + "My problem is ";
    Helper::Instance()->sendMail("support@android-remote.com", "Remote Control Support", body);
}

void MainWindow::helpGithubAction()
{
    Logger::Instance()->trackEvent("Server", "Click", "Github");
    Server::Instance()->startProcess("https://github.com/Steppschuh/RemoteControlServer/issues");
}

void MainWindow::initUiWithSettings()
{
    ui->startServerOnLogin->setChecked(Settings::Instance()->autoStart);
    ui->startServerMinimized->setChecked(Settings::Instance()->startMinimized);
    ui->showNotifications->setChecked(Settings::Instance()->showTrayNotifications);

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
    changePointer(Settings::Instance()->pointerDesign);
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

void MainWindow::initUpdateNewestVersion()
{
    ui->updateLatestVersion->setText(Updater::Instance()->updateVersionName);
}

void MainWindow::ledOn()
{
    Serial::Instance()->sendMessage("<01>");
}

void MainWindow::ledOff()
{
    Serial::Instance()->sendMessage("<02>");
}

void MainWindow::openMediaFileDialog()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Select Media Application"), "/Applications", tr("Applicaionts (*.app)"));

    if (file.length() > 0) ui->defaultMediaPlayer->setText(file);
}

void MainWindow::paintEvent(QPaintEvent * event)
{
    ui->serverIpLabel->setText(Network::Instance()->getServerIp());
}

void MainWindow::reopenSerialPort()
{
    Serial::Instance()->closeSerialPort();
    Serial::Instance()->openSerialPort(Settings::Instance()->serialPortName);
}

void MainWindow::setVisibleStateOfPinBox(bool value)
{
    if (value)  ui->pinDisplay->setEchoMode(QLineEdit::Normal);
    else        ui->pinDisplay->setEchoMode(QLineEdit::Password);
}

void MainWindow::showNewDialog(QString title, QString message)
{
    QMessageBox msgBox(QMessageBox::Information, title, message);

    msgBox.exec();
}

void MainWindow::showNewErrorDialog(QString title, QString message)
{
    QMessageBox msgBox(QMessageBox::Critical, title, message);

    msgBox.exec();
}

void MainWindow::startUpdateChangelogAction()
{
    showNewDialog("Changelog", Updater::Instance()->updateChangeLog);
}

void MainWindow::startUpdateHelpAction()
{
    Server::Instance()->startProcess(Updater::Instance()->URL_UPDATE_HELP);
}

void MainWindow::updateLogMessages()
{
    ui->logTextView->setText(Logger::Instance()->getString());
    QScrollBar *sb = ui->logTextView->verticalScrollBar();
    sb->setValue(sb->maximum());
}
