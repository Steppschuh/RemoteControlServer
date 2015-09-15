#ifndef WHITELISTWINDOW_H
#define WHITELISTWINDOW_H

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class WhitelistWindow;
}

class WhitelistWindow : public QDialog
{
    Q_OBJECT

public:
    explicit WhitelistWindow(QWidget *parent = 0);
    ~WhitelistWindow();

    void closeEvent(QCloseEvent *event);

public slots:
    void customHide();
    void customShow();
    void validateInput();

private:
    Ui::WhitelistWindow *ui;
};

#endif // WHITELISTWINDOW_H
