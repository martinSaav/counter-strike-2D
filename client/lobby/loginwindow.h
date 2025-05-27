#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>

namespace Ui {
class loginwindow;
}

class loginwindow : public QDialog
{
    Q_OBJECT

public:
    loginwindow(std::string& namePlayer, QWidget *parent = nullptr);
    ~loginwindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_loginButton_clicked();

signals:
    void ventanaCerrada();

private:
    Ui::loginwindow *ui;
    std::string& namePlayer;
};

#endif // LOGINWINDOW_H
