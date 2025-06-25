#include "selectskin.h"

#include "ui_selectskin.h"

selectSkin::selectSkin(team& teamPlayer, std::string& skinSeleccionada, QWidget* parent):
        QDialog(parent),
        ui(new Ui::selectSkin),
        skinSeleccionada(skinSeleccionada),
        teamPlayer(teamPlayer) {

    ui->setupUi(this);

    switch (teamPlayer) {
        case TERRORISTA:
            // Desactivamos las imagenes de contraterrorista
            ui->labelC1->setEnabled(false);
            ui->labelC2->setEnabled(false);
            ui->labelC3->setEnabled(false);
            ui->labelC4->setEnabled(false);
            break;

        case CONTRATERRORISTA:
            // Desactivamos las imagenes de terorrista
            ui->labelT1->setEnabled(false);
            ui->labelT2->setEnabled(false);
            ui->labelT3->setEnabled(false);
            ui->labelT4->setEnabled(false);
            break;
    }
}

selectSkin::~selectSkin() { delete ui; }

void selectSkin::on_button1_clicked() {

    if (teamPlayer == TERRORISTA) {
        skinSeleccionada = "T1";
    } else {
        skinSeleccionada = "C1";
    }
    this->close();
}

void selectSkin::on_button2_clicked() {

    if (teamPlayer == TERRORISTA) {
        skinSeleccionada = "T2";
    } else {
        skinSeleccionada = "C2";
    }
    this->close();
}

void selectSkin::on_button3_clicked() {

    if (teamPlayer == TERRORISTA) {
        skinSeleccionada = "T3";
    } else {
        skinSeleccionada = "C3";
    }
    this->close();
}

void selectSkin::on_button4_clicked() {

    if (teamPlayer == TERRORISTA) {
        skinSeleccionada = "T4";
    } else {
        skinSeleccionada = "C4";
    }
    this->close();
}

void selectSkin::closeEvent(QCloseEvent* event) {
    emit ventanaCerrada();
    QDialog::closeEvent(event);
}
