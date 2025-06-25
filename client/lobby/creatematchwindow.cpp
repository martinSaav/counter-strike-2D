#include "creatematchwindow.h"

#include <memory>

#include "common/dto/create_game_request.h"
#include "common/dto/create_game_response.h"

#include "ui_creatematchwindow.h"

createMatchWindow::createMatchWindow(Protocol& protocolo, const std::list<std::string>& maps,
                                     QWidget* parent):
        QDialog(parent), ui(new Ui::createMatchWindow), protocolo(protocolo) {
    this->setAttribute(Qt::WA_DeleteOnClose);  // Para eliminar el widget al cerrarlo

    ui->setupUi(this);

    // Recibimos por protocolo la lista de mapas
    for (const auto& map: maps) {
        ui->listMaps->addItem(QString::fromStdString(map));
    }
}

createMatchWindow::~createMatchWindow() { delete ui; }

void createMatchWindow::on_createButton_clicked() {
    // Obtenemos el nombre de la partida
    QString nameMatch = ui->inputNameMatch->toPlainText();
    QListWidgetItem* item = ui->listMaps->currentItem();

    if (item && nameMatch != "") {
        // Obtenemos el mapa seleccionado
        QString map = item->text();

        CreateGameRequest requestGameCreate(nameMatch.toStdString(), map.toStdString());
        protocolo.send_message(requestGameCreate);

        const std::unique_ptr<Message> responseGameCreate = protocolo.recv_message();

        const auto game = dynamic_cast<CreateGameResponse*>(responseGameCreate.get());

        bool gameCreated = game->get_success();

        if (gameCreated) {
            accept();  // Cerramos la ventana
            // Creamos la ventana de espera
            tipoUsuario usuario = CREADOR;
            waitRoom* windoWait = new waitRoom(protocolo, usuario, this->parentWidget());
            windoWait->setWindowFlags(Qt::Window);  // esto la hace ventana real
            windoWait->show();
        }
    }
}

void createMatchWindow::on_backButon_clicked() {
    // Volvemos a mostrar el lobby
    this->parentWidget()->show();
    this->close();
}
