#include "lobbywindow.h"

#include <QMessageBox>
#include <memory>

#include "./ui_lobbywindow.h"
#include "common/dto/game_list_response.h"
#include "common/dto/login_request.h"
#include "common/dto/map_names_response.h"
#include "common/dto/simple_messages.h"

#include "loginwindow.h"

MainWindow::MainWindow(Protocol& protocol, std::string& namePlayer, QWidget* parent):
        QMainWindow(parent), ui(new Ui::LobbyWindow), protocol(protocol), namePlayer(namePlayer) {
    // this->setAttribute(Qt::WA_DeleteOnClose); // Para eliminar el widget al cerrarlo

    // Instanciamos los widgets
    ui->setupUi(this);

    // Ocultamos widgets para buscar partidas
    ui->matchList->hide();
    ui->refreshButton->hide();
    ui->joinButton->hide();
    ui->userImage->hide();
    ui->createButton->hide();
    ui->searchButton->hide();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_createButton_clicked() {
    createMatchWindow* windowCreate = new createMatchWindow(this->protocol, this->maps, this);

    windowCreate->exec();
}

void MainWindow::on_joinButton_clicked() {
    // Obtener la fila seleccionada
    int selectedRow = ui->matchList->currentRow();

    // Obtener el ítem de la primera columna (columna 0) de la fila seleccionada
    QTableWidgetItem* item = ui->matchList->item(selectedRow, 0);

    if (item) {

        QString map = item->text();
        JoinGameRequest requestJoinGame(map.toStdString());
        protocol.send_message(requestJoinGame);

        const std::unique_ptr<Message> responseJoinGame = protocol.recv_message();
        const auto* game = dynamic_cast<JoinGameResponse*>(responseJoinGame.get());

        bool gameJoined = game->get_success();

        if (gameJoined) {
            // Creamos la ventana de espera
            userType user = JOINED;
            waitRoom* windowWait = new waitRoom(protocol, user, this);
            windowWait->setWindowFlags(Qt::Window);  // esto la hace ventana real
            windowWait->show();
        } else {
            QMessageBox::warning(this, "No se pudo entrar",
                                 "La partida ya empezó o está llena. Actualizá la lista y "
                                 "elegí otra.");
            on_refreshButton_clicked();
        }
    }
}

void MainWindow::on_refreshButton_clicked() {
    // recibimos del protocolo
    GameListRequest requestGameList;
    protocol.send_message(requestGameList);

    const std::unique_ptr<Message> responseGameList = protocol.recv_message();

    const auto gameList = dynamic_cast<GameListResponse*>(responseGameList.get());

    std::list<GameInfo> games = gameList->get_games();

    int matchCount = games.size();
    // Agregar filas a la tabla
    ui->matchList->clear();
    ui->matchList->setRowCount(matchCount);

    int i = 0;
    for (const auto& game: games) {
        ui->matchList->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(game.name)));
        ui->matchList->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(game.map_name)));
        std::string playerCount =
                std::to_string(game.current_players) + "/" + std::to_string(game.max_players);
        ui->matchList->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(playerCount)));
        ++i;
    }

    // Actualizamos nombres columnas
    QStringList headers;
    headers << "Match name"
            << "Map"
            << "Players";
    ui->matchList->setHorizontalHeaderLabels(headers);
}

void MainWindow::on_quitButton_clicked() { this->close(); }

void MainWindow::on_searchButton_clicked() {
    // Habilitamos los widgets para buscar partida
    ui->searchButton->setEnabled(false);
    ui->matchList->show();
    ui->refreshButton->show();
    ui->joinButton->show();

    this->on_refreshButton_clicked();
}

void MainWindow::on_loginButton_clicked() {
    loginwindow* windowLogin = new loginwindow(namePlayer, this);

    connect(windowLogin, &loginwindow::windowClosed, this, [this]() {
        if (this->namePlayer == "") {  // caso no coloco un nombre
            return;
        }
        QString qtext = QString::fromStdString(this->namePlayer);

        // Mostrar el texto en el QLabel
        ui->nameLabel->setText(qtext);
        ui->userImage->show();
        ui->createButton->show();
        ui->searchButton->show();
        ui->loginButton->hide();

        // Mandamos al server el nombre del login
        LoginRequest requestLogin(this->namePlayer);
        protocol.send_message(requestLogin);

        // Pedimos los mapas al servidor
        MapNamesRequest requestNamesMaps;
        protocol.send_message(requestNamesMaps);

        const std::unique_ptr<Message> responseNameMaps = protocol.recv_message();
        const auto nameMaps = dynamic_cast<MapNamesResponse*>(responseNameMaps.get());

        maps = nameMaps->get_mapGames();
    });

    windowLogin->exec();  // Aquí se bloquea y se ejecuta la ventana
}
