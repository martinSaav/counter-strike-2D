#include "creatematchwindow.h"

#include <memory>

#include "common/dto/create_game_request.h"
#include "common/dto/simple_messages.h"

#include "ui_creatematchwindow.h"

createMatchWindow::createMatchWindow(Protocol& protocol, const std::list<std::string>& maps,
                                     QWidget* parent):
        QDialog(parent), ui(new Ui::createMatchWindow), protocol(protocol) {
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
        protocol.send_message(requestGameCreate);

        const std::unique_ptr<Message> responseGameCreate = protocol.recv_message();

        const auto game = dynamic_cast<CreateGameResponse*>(responseGameCreate.get());

        bool gameCreated = game->get_success();

        if (gameCreated) {
            accept();  // Cerramos la ventana
            // Creamos la ventana de espera
            userType user = CREATOR;
            waitRoom* windowWait = new waitRoom(protocol, user, this->parentWidget());
            windowWait->setWindowFlags(Qt::Window);  // esto la hace ventana real
            windowWait->show();
        }
    }
}

void createMatchWindow::on_backButon_clicked() {
    // Volvemos a mostrar el lobby
    this->parentWidget()->show();
    this->close();
}
