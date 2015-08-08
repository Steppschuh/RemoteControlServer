#include "customwindow.h"
#include "settings.h"
#include "ui_customwindow.h"

CustomWindow::CustomWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomWindow)
{
    ui->setupUi(this);

    connect(ui->applyButton, SIGNAL(pressed()), this, SLOT(customHide()));
}

CustomWindow::~CustomWindow()
{
    delete ui;
}

void CustomWindow::customHide()
{
    Settings::Instance()->customActions = new QStringList();
    QString actions = ui->customTextEdit->toPlainText();
    foreach (QString value, actions.split("\n"))
    {
        QString trimmed = value.trimmed();
        if (!Settings::Instance()->customActions->contains(trimmed)) Settings::Instance()->customActions->append(trimmed);
    }

    hide();
}

void CustomWindow::customShow()
{
    QString text = "";
    for (int i = 0; i < Settings::Instance()->customActions->length(); ++i)
    {
        text = text + Settings::Instance()->customActions->at(i) + "\n";
    }
    if (text.length() > 0) text = text.left(text.length() - 1);

    ui->customTextEdit->setText(text);

    show();
}
