#include "lobbywindow.h"
#include "./ui_lobbywindow.h"
#include "common/dto/game_list_request.h"
#include "common/dto/game_list_response.h"

MainWindow::MainWindow(Protocol& protocolo, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LobbyWindow)
    , protocolo(protocolo)
{
    //Instanciamos los widgets
    ui->setupUi(this);

    //Colocamos fondo de pantalla
    //this->setStyleSheet("background-image: url(:/imagenes/fondoLobby.jpg);"
    //                "background-repeat: no-repeat;"
    //                "background-position: center;");

    //Ocultamos widgets para buscar partidas
    ui->listaPartidas->hide();
    ui->actualizarButton->hide();
    ui->unirseButton->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_crearButton_clicked()
{
    createMatchWindow *windowCreate = new createMatchWindow(this);

    windowCreate->setWindowFlags(Qt::Window);  // esto la hace ventana real
    windowCreate->show();

    // Ocultamos el lobby
    this->hide();
}

void MainWindow::on_unirseButton_clicked()
{
    // Obtener la fila seleccionada
    int filaSeleccionada = ui->listaPartidas->currentRow();

    // Obtener el ítem de la primera columna (columna 0) de la fila seleccionada
    QTableWidgetItem *item = ui->listaPartidas->item(filaSeleccionada, 0);

    if (item) {

        std::string texto = "Esperando a comienzo de partida";
        QString qtexto = QString::fromStdString(texto);

        // Mostrar el texto en el QLabel
        ui->waitText->setText(qtexto);

        // Desactiva todos los widgets
        this->setEnabled(false);

        ui->listaPartidas->clear();

        //QApplication::exit(EXITLOBBY);
    }
}

void MainWindow::on_actualizarButton_clicked()
{
    //recibimos del protocolo
    GameListRequest requestGameList;
    protocolo.send_message(requestGameList);

    const auto responseGameList = protocolo.recv_message();

    const auto gameList = dynamic_cast<GameListResponse*>(responseGameList.get());
    
    std::list<Game> games = gameList->get_games();
    
    int cantPartidas = games.size();
    std::string cantJugadores;
    // Agregar filas a la tabla
    ui->listaPartidas->clear();
    ui->listaPartidas->setRowCount(cantPartidas);

    int i = 0;
    for (const auto& game : games) {
        ui->listaPartidas->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(game.name)));
        ui->listaPartidas->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(game.map_name)));
        cantJugadores = std::to_string(game.current_players) + "/" + std::to_string(game.max_players); 
        ui->listaPartidas->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(cantJugadores)));
        ++i;
    }

    //Actualizamos nombres columnas
    QStringList headers;
    headers << "Nombre partida" << "Mapa" << "Cantidad jugadores";
    ui->listaPartidas->setHorizontalHeaderLabels(headers);
}

void MainWindow::on_quitButton_clicked()
{
    this->close();
}

void MainWindow::on_buscarButton_clicked()
{
    //Habilitamos los widgets para buscar partida
    ui->buscarButton->setEnabled(false);
    ui->listaPartidas->show();
    ui->actualizarButton->show();
    ui->unirseButton->show();

    this->on_actualizarButton_clicked();
}
