#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <string>

namespace Ui {
class loginwindow;
}

class loginwindow: public QDialog {
    Q_OBJECT

public:
    loginwindow(std::string& namePlayer, QWidget* parent = nullptr);
    ~loginwindow() override;

protected:
    void closeEvent(QCloseEvent* event) override;

private Q_SLOTS:
    void on_loginButton_clicked();

Q_SIGNALS:
    void ventanaCerrada();

private:
    Ui::loginwindow* ui;
    std::string& namePlayer;
};
#endif  // LOGINWINDOW_H
