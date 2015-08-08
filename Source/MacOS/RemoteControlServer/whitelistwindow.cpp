#include "network.h"
#include "settings.h"
#include "whitelistwindow.h"
#include "ui_whitelistwindow.h"

#include <QDebug>

WhitelistWindow::WhitelistWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WhitelistWindow)
{
    ui->setupUi(this);

    connect(ui->ipTextEdit, SIGNAL(textChanged()), this, SLOT(validateInput()));
    connect(ui->applyButton, SIGNAL(pressed()), this, SLOT(customHide()));
}

WhitelistWindow::~WhitelistWindow()
{
    delete ui;
}

void WhitelistWindow::customHide()
{
    Settings::Instance()->whitelistedIps = new QStringList();
    QString whitelist = ui->ipTextEdit->toPlainText();
    foreach(QString ip, whitelist.split("\n"))
    {
        QString trimmedIp = ip.trimmed();
        if (Network::Instance()->isValidIp(trimmedIp))
        {
            if (!Settings::Instance()->whitelistedIps->contains(QString(ip.trimmed())))
            {
                Settings::Instance()->whitelistedIps->append(QString(ip.trimmed()));
            }
        }
    }
    hide();
}

void WhitelistWindow::customShow()
{
    QString text = "";
    for (int i = 0; i < Settings::Instance()->whitelistedIps->length(); ++i)
    {
        text += Settings::Instance()->whitelistedIps->at(i) + "\n";
    }
    text = text.left(text.length() - 1);
    ui->ipTextEdit->setText(text);

    validateInput();

    show();
}

void WhitelistWindow::validateInput()
{
    int countValid = 0;
    int countInvalid = 0;
    QString whitelist = ui->ipTextEdit->toPlainText();

    foreach (QString ip, whitelist.split("\n"))
    {
        QString trimmedIp = ip.trimmed();
        if (Network::Instance()->isValidIp(trimmedIp)) countValid += 1;
        else countInvalid += 1;
    }

    QString status;
    if (countValid == 1) status = QString::number(countValid) + " valid IP address found";
    else status = QString::number(countValid) + " valid IP addresses found";

    if (countInvalid > 0)
    {
        status = status + ", " + QString::number(countInvalid) + " invalid";
        ui->applyButton->setEnabled(false);
    }
    else
    {
        ui->applyButton->setEnabled(true);
    }
    ui->validIPsFound->setText(status);
}
