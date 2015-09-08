#ifndef CUSTOMWINDOW_H
#define CUSTOMWINDOW_H

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class CustomWindow;
}

class CustomWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CustomWindow(QWidget *parent = 0);
    ~CustomWindow();

    void closeEvent(QCloseEvent *event);

public slots:
    void customHide();
    void customShow();

private:
    Ui::CustomWindow *ui;
};

#endif // CUSTOMWINDOW_H
