#include "waitroom.h"
#include "ui_waitroom.h"

waitRoom::waitRoom(Protocol& protocolo, tipoUsuario& usuario, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::waitRoom)
    , protocolo(protocolo)
    , usuario(usuario){

    ui->setupUi(this);
    this->parentWidget()->hide();
}

waitRoom::~waitRoom(){
    delete ui;
}

void waitRoom::on_terroristaButton_clicked(){
    team teamPlayer = TERRORISTA;
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

void waitRoom::on_contraterroristaButton_clicked(){
    team teamPlayer = CONTRATERRORISTA;
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

    if (this->skinSeleccionada == ""){  // caso no coloco un nombre
        skinSeleccionada = "C1";
    }

    if (usuario == CREADOR){
        GameReadyRequest gameReady;
        protocolo.send_message(gameReady);
    }
    const std::unique_ptr<Message> responseGameReady = protocolo.recv_message();
    const auto game = dynamic_cast<GameReadyResponse*>(responseGameReady.get());
    bool gameReady = game->get_success();

    this->close();
    QApplication::exit(EXITLOBBY);
}
