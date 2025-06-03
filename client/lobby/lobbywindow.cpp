#include "lobbywindow.h"

#include <memory>

#include "./ui_lobbywindow.h"
#include "common/dto/game_list_request.h"
#include "common/dto/game_list_response.h"
#include "common/dto/login_request.h"
#include "common/dto/map_names_request.h"
#include "common/dto/map_names_response.h"

#include "loginwindow.h"

MainWindow::MainWindow(Protocol& protocolo, std::string& namePlayer, QWidget* parent):
        QMainWindow(parent), ui(new Ui::LobbyWindow), protocolo(protocolo), namePlayer(namePlayer){
    // this->setAttribute(Qt::WA_DeleteOnClose); // Para eliminar el widget al cerrarlo

    // Instanciamos los widgets
    ui->setupUi(this);

    // Ocultamos widgets para buscar partidas
    ui->listaPartidas->hide();
    ui->actualizarButton->hide();
    ui->unirseButton->hide();
    ui->userImage->hide();
    ui->crearButton->hide();
    ui->buscarButton->hide();
}

MainWindow::~MainWindow(){ delete ui; }

void MainWindow::on_crearButton_clicked(){
    createMatchWindow* windowCreate = new createMatchWindow(this->protocolo, this->maps, this);

    windowCreate->exec();
}

void MainWindow::on_unirseButton_clicked(){
    // Obtener la fila seleccionada
    int filaSeleccionada = ui->listaPartidas->currentRow();

    // Obtener el ítem de la primera columna (columna 0) de la fila seleccionada
    QTableWidgetItem* item = ui->listaPartidas->item(filaSeleccionada, 0);

    if (item){

        QString map = item->text();
        JoinGameRequest requestJoinGame(map.toStdString());
        protocolo.send_message(requestJoinGame);

        const std::unique_ptr<Message> responseJoinGame = protocolo.recv_message();
        const auto* game = dynamic_cast<JoinGameResponse*>(responseJoinGame.get());

        bool gameJoined = game->get_success();

        if (gameJoined){
            // Creamos la ventana de espera
            tipoUsuario usuario = UNIDO;
            waitRoom* windoWait = new waitRoom(protocolo, usuario, this);
            windoWait->setWindowFlags(Qt::Window);  // esto la hace ventana real
            windoWait->show();
        }
    }
}

void MainWindow::on_actualizarButton_clicked(){
    // recibimos del protocolo
    GameListRequest requestGameList;
    protocolo.send_message(requestGameList);

    const std::unique_ptr<Message> responseGameList = protocolo.recv_message();

    const auto gameList = dynamic_cast<GameListResponse*>(responseGameList.get());

    std::list<GameInfo> games = gameList->get_games();

    int cantPartidas = games.size();
    // Agregar filas a la tabla
    ui->listaPartidas->clear();
    ui->listaPartidas->setRowCount(cantPartidas);

    int i = 0;
    for (const auto& game: games) {
        ui->listaPartidas->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(game.name)));
        ui->listaPartidas->setItem(i, 1,
                                   new QTableWidgetItem(QString::fromStdString(game.map_name)));
        std::string cantJugadores =
                std::to_string(game.current_players) + "/" + std::to_string(game.max_players);
        ui->listaPartidas->setItem(i, 2,
                                   new QTableWidgetItem(QString::fromStdString(cantJugadores)));
        ++i;
    }

    // Actualizamos nombres columnas
    QStringList headers;
    headers << "Nombre partida"
            << "Mapa"
            << "Cantidad jugadores";
    ui->listaPartidas->setHorizontalHeaderLabels(headers);
}

void MainWindow::on_quitButton_clicked(){ this->close(); }

void MainWindow::on_buscarButton_clicked(){
    // Habilitamos los widgets para buscar partida
    ui->buscarButton->setEnabled(false);
    ui->listaPartidas->show();
    ui->actualizarButton->show();
    ui->unirseButton->show();

    this->on_actualizarButton_clicked();
}

void MainWindow::on_loginButton_clicked(){
    loginwindow* windowLogin = new loginwindow(namePlayer, this);

    connect(windowLogin, &loginwindow::ventanaCerrada, this, [this]() {
        if (this->namePlayer == ""){  // caso no coloco un nombre
            return;
        }
        QString qtexto = QString::fromStdString(this->namePlayer);

        // Mostrar el texto en el QLabel
        ui->nameLabel->setText(qtexto);
        ui->userImage->show();
        ui->crearButton->show();
        ui->buscarButton->show();
        ui->loginButton->hide();

        // Mandamos al server el nombre del login
        LoginRequest requestLogin(this->namePlayer);
        protocolo.send_message(requestLogin);

        // Pedimos los mapas al servidor
        MapNamesRequest requestNamesMaps;
        protocolo.send_message(requestNamesMaps);

        const std::unique_ptr<Message> responseNameMaps = protocolo.recv_message();
        const auto nameMaps = dynamic_cast<MapNamesResponse*>(responseNameMaps.get());

        maps = nameMaps->get_mapGames();
    });

    windowLogin->exec();  // Aquí se bloquea y se ejecuta la ventana
}
