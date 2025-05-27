#include "creatematchwindow.h"
#include "ui_creatematchwindow.h"
#include "common/dto/create_game_request.h"
#include "common/dto/create_game_response.h"

createMatchWindow::createMatchWindow(Protocol& protocolo, std::list<std::string> maps, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::createMatchWindow)
    , protocolo(protocolo)
{
    this->setAttribute(Qt::WA_DeleteOnClose); // Para eliminar el widget al cerrarlo

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
    // Obtenemos el nombre de la partida
    QString nameMatch = ui->inputNameMatch->toPlainText();
    QListWidgetItem *item = ui->listMaps->currentItem();

    if (item && nameMatch != "") {
        // Obtenemos el mapa seleccionado
        QString map = item->text();

        CreateGameRequest requestGameCreate(nameMatch.toStdString());
        protocolo.send_message(requestGameCreate);

        const std::unique_ptr<Message> responseGameCreate = protocolo.recv_message();

        const auto game = dynamic_cast<CreateGameResponse*>(responseGameCreate.get());
    
        bool gameCreated = game->get_success();

        if (gameCreated){
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
}

void createMatchWindow::on_backButon_clicked()
{
    // Volvemos a mostrar el lobby
    this->parentWidget()->show();

    this->close();
}

void createMatchWindow::on_startButton_clicked()
{
    this->parentWidget()->hide();
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
