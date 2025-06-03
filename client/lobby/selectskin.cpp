#include "selectskin.h"
#include "ui_selectskin.h"

selectSkin::selectSkin(team teamPlayer, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::selectSkin){
        
    ui->setupUi(this);

    switch (teamPlayer)
    {
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

selectSkin::~selectSkin(){
    delete ui;
}

void selectSkin::on_button1_clicked(){
    this->close();
}

void selectSkin::on_button2_clicked(){
    this->close();
}

void selectSkin::on_button3_clicked(){
    this->close();
}

void selectSkin::on_button4_clicked(){
    this->close();
}
