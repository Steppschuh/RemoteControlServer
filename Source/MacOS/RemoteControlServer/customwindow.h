#ifndef CUSTOMWINDOW_H
#define CUSTOMWINDOW_H

#include <QDialog>

namespace Ui {
class CustomWindow;
}

class CustomWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CustomWindow(QWidget *parent = 0);
    ~CustomWindow();

public slots:
    void customHide();
    void customShow();

private:
    Ui::CustomWindow *ui;
};

#endif // CUSTOMWINDOW_H
