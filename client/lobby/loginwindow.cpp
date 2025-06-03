#include "loginwindow.h"

#include "ui_loginwindow.h"

loginwindow::loginwindow(std::string& namePlayer, QWidget* parent):
        QDialog(parent), ui(new Ui::loginwindow), namePlayer(namePlayer){
    ui->setupUi(this);
}

loginwindow::~loginwindow() { delete ui; }

void loginwindow::on_loginButton_clicked(){
    QString nameInput = ui->usernameInput->toPlainText();
    if (nameInput != "") {

        namePlayer = nameInput.toStdString();
        // Volvemos a mostrar el lobby
        this->parentWidget()->show();
        this->close();
    }
}

void loginwindow::closeEvent(QCloseEvent* event){
    emit ventanaCerrada();
    QWidget::closeEvent(event);
}
