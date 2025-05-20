#include "creatematchwindow.h"
#include "ui_creatematchwindow.h"

createMatchWindow::createMatchWindow(std::list<std::string> maps, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::createMatchWindow)
{
    ui->setupUi(this);
    ui->startButton->hide();

    //Recibimos por protocolo la lista de mapas
    for (const auto& map : maps) {
        ui->listMaps->addItem(QString::fromStdString(map));
    }
}

createMatchWindow::~createMatchWindow()
{
    delete ui;
}

void createMatchWindow::on_createButton_clicked()
{
    QString nameMatch = ui->inputNameMatch->toPlainText();
    QListWidgetItem *item = ui->listMaps->currentItem();

    if (item && nameMatch != "") {
        QString map = item->text();

        //Desactivamos widgets
        this->desactivarWidgets();

        // Solo habilitar el botón de "descongelar"
        ui->startButton->show();

        std::string texto = "Esperando a comienzo de partida";
        QString qtexto = QString::fromStdString(texto);

        // Mostrar el texto en el QLabel
        ui->waitLabel->setText(qtexto);
    }
}

void createMatchWindow::on_backButon_clicked()
{
    // Volvemos a mostrar el lobby
    this->parentWidget()->show();

    this->close();
}

void createMatchWindow::on_startButton_clicked()
{
    this->close();
    QApplication::exit(EXITLOBBY);
}

void createMatchWindow::desactivarWidgets()
{
    ui->inputNameMatch->setEnabled(false);
    ui->listMaps->setEnabled(false);
    ui->backButon->setEnabled(false);
    ui->createButton->setEnabled(false);
}
