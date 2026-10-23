#include "selectskin.h"

#include "ui_selectskin.h"

selectSkin::selectSkin(team& teamPlayer, std::string& selectedSkin, QWidget* parent):
        QDialog(parent),
        ui(new Ui::selectSkin),
        selectedSkin(selectedSkin),
        teamPlayer(teamPlayer) {

    ui->setupUi(this);

    switch (teamPlayer) {
        case TERRORIST:
            // Desactivamos las imagenes de contraterrorista
            ui->labelC1->setEnabled(false);
            ui->labelC2->setEnabled(false);
            ui->labelC3->setEnabled(false);
            ui->labelC4->setEnabled(false);
            break;

        case COUNTERTERRORIST:
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

    if (teamPlayer == TERRORIST) {
        selectedSkin = "T1";
    } else {
        selectedSkin = "C1";
    }
    this->close();
}

void selectSkin::on_button2_clicked() {

    if (teamPlayer == TERRORIST) {
        selectedSkin = "T2";
    } else {
        selectedSkin = "C2";
    }
    this->close();
}

void selectSkin::on_button3_clicked() {

    if (teamPlayer == TERRORIST) {
        selectedSkin = "T3";
    } else {
        selectedSkin = "C3";
    }
    this->close();
}

void selectSkin::on_button4_clicked() {

    if (teamPlayer == TERRORIST) {
        selectedSkin = "T4";
    } else {
        selectedSkin = "C4";
    }
    this->close();
}

void selectSkin::closeEvent(QCloseEvent* event) {
    emit windowClosed();
    QDialog::closeEvent(event);
}
