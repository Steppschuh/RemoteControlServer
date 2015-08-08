#ifndef WHITELISTWINDOW_H
#define WHITELISTWINDOW_H

#include <QDialog>

namespace Ui {
class WhitelistWindow;
}

class WhitelistWindow : public QDialog
{
    Q_OBJECT

public:
    explicit WhitelistWindow(QWidget *parent = 0);
    ~WhitelistWindow();

public slots:
    void customHide();
    void customShow();
    void validateInput();

private:
    Ui::WhitelistWindow *ui;
};

#endif // WHITELISTWINDOW_H
