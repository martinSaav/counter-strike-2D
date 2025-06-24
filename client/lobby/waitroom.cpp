#include "waitroom.h"
#include "ui_waitroom.h"

waitRoom::waitRoom(Protocol& protocolo, tipoUsuario& usuario, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::waitRoom)
    , protocolo(protocolo)
    , usuario(usuario){

    ui->setupUi(this);
    this->parentWidget()->hide();
    ui->aleatorioButton->hide();

    if (usuario == UNIDO){
        ui->empezarButton->setEnabled(false);

        connect(this, &waitRoom::gameReady, this, [this]() {
            this->close();
            QApplication::exit(EXITLOBBY);
        });
        socketThread = std::thread(&waitRoom::ask_game_started, this);
    }
}

waitRoom::~waitRoom(){
    if (socketThread.joinable()) {
        socketThread.join();
    }
    delete ui;
}

void waitRoom::on_terroristaButton_clicked(){
    team teamPlayer = TERRORISTA;
    this->crearVentanaSeleccionSkin(teamPlayer);
}

void waitRoom::on_contraterroristaButton_clicked(){
    team teamPlayer = CONTRATERRORISTA;
    this->crearVentanaSeleccionSkin(teamPlayer);
}

void waitRoom::crearVentanaSeleccionSkin(team& teamPlayer){

    selectSkin* windowSkin = new selectSkin(teamPlayer, skinSeleccionada, this);

    connect(windowSkin, &selectSkin::ventanaCerrada, this, [this]() {
        if (this->skinSeleccionada == ""){  // caso no coloco un nombre
            return;
        }
        
        // Mandamos al server la skin del jugador
        SelectSkinRequest requestSkin(this->skinSeleccionada);
        protocolo.send_message(requestSkin);

    });
    windowSkin->exec();
}

void waitRoom::on_aleatorioButton_clicked(){

}

void waitRoom::on_empezarButton_clicked(){

    // Empezamos la partida
    GameReadyRequest gameReady;
    protocolo.send_message(gameReady);

    const std::unique_ptr<Message> responseGameReady = protocolo.recv_message();
    const auto game = dynamic_cast<GameReadyResponse*>(responseGameReady.get());
    bool gameStart = game->get_success();

    this->close();
    QApplication::exit(EXITLOBBY);
}

void waitRoom::ask_game_started(){
    const std::unique_ptr<Message> responseGameReady = protocolo.recv_message();
    const auto game = dynamic_cast<GameReadyResponse*>(responseGameReady.get());
    bool gameStart = game->get_success();

    if (gameStart) {
       emit gameReady();  // <-- comunicar al hilo principal
    }
}