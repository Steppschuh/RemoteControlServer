#include "whitelistedipswindow.h"
#include "ui_whitelistedipswindow.h"

WhitelistedIpsWindow::WhitelistedIpsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WhitelistedIpsWindow)
{
    ui->setupUi(this);
}

WhitelistedIpsWindow::~WhitelistedIpsWindow()
{
    delete ui;
}
