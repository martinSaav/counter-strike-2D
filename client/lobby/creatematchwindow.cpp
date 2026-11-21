#include "creatematchwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <exception>
#include <memory>
#include <string>

#include "common/dto/create_game_request.h"
#include "common/dto/simple_messages.h"
#include "common/name_limits.h"

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
    // Antes, si faltaba algo o el servidor rechazaba el nombre, la ventana no hacia nada y no se
    // entendia por que. Lo que se puede validar aca se avisa sin preguntarle al servidor.
    const std::string nameMatch = ui->inputNameMatch->toPlainText().trimmed().toStdString();
    QListWidgetItem* item = ui->listMaps->currentItem();
    if (nameMatch.empty()) {
        QMessageBox::warning(this, "Falta el nombre", "Escribí un nombre para la partida.");
        return;
    }
    if (nameMatch.size() > MAX_NAME_LENGTH) {
        QMessageBox::warning(
                this, "Nombre muy largo",
                QString("El nombre puede tener hasta %1 caracteres.").arg(MAX_NAME_LENGTH));
        return;
    }
    if (!item) {
        QMessageBox::warning(this, "Falta el mapa", "Elegí un mapa de la lista.");
        return;
    }

    std::unique_ptr<Message> responseGameCreate;
    try {
        CreateGameRequest requestGameCreate(nameMatch, item->text().toStdString());
        protocol.send_message(requestGameCreate);
        responseGameCreate = protocol.recv_message();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Sin conexión",
                              QString("Se perdió la conexión con el servidor: %1").arg(e.what()));
        QApplication::exit(EXITAPP);
        return;
    }

    const auto game = dynamic_cast<CreateGameResponse*>(responseGameCreate.get());
    if (!game || !game->get_success()) {
        // El largo ya se valido: si el servidor la rechaza es porque el nombre esta en uso
        QMessageBox::warning(this, "No se pudo crear la partida",
                             "Ya existe una partida con ese nombre. Elegí otro.");
        return;
    }

    accept();  // Cerramos la ventana
    // Creamos la ventana de espera
    userType user = CREATOR;
    waitRoom* windowWait = new waitRoom(protocol, user, this->parentWidget());
    windowWait->setWindowFlags(Qt::Window);  // esto la hace ventana real
    windowWait->show();
}

void createMatchWindow::on_backButon_clicked() {
    // Volvemos a mostrar el lobby
    this->parentWidget()->show();
    this->close();
}
