#ifndef WHITELISTEDIPSWINDOW_H
#define WHITELISTEDIPSWINDOW_H

#include <QWidget>

namespace Ui {
class WhitelistedIpsWindow;
}

class WhitelistedIpsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit WhitelistedIpsWindow(QWidget *parent = 0);
    ~WhitelistedIpsWindow();

private:
    Ui::WhitelistedIpsWindow *ui;
};

#endif // WHITELISTEDIPSWINDOW_H
