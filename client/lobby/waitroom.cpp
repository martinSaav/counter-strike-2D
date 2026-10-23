#include "waitroom.h"

#include <memory>

#include "ui_waitroom.h"

waitRoom::waitRoom(Protocol& protocol, userType& user, QWidget* parent):
        QWidget(parent), ui(new Ui::waitRoom), protocol(protocol), user(user) {

    ui->setupUi(this);
    this->parentWidget()->hide();

    if (user == JOINED) {
        ui->startButton->setEnabled(false);

        connect(this, &waitRoom::gameReady, this, [this]() {
            this->close();
            QApplication::exit(EXITLOBBY);
        });
        socketThread = std::thread(&waitRoom::ask_game_started, this);
    }
}

waitRoom::~waitRoom() {
    if (socketThread.joinable()) {
        socketThread.join();
    }
    delete ui;
}

void waitRoom::on_terroristButton_clicked() {
    team teamPlayer = TERRORIST;
    this->createSkinSelectionWindow(teamPlayer);
}

void waitRoom::on_counterTerroristButton_clicked() {
    team teamPlayer = COUNTERTERRORIST;
    this->createSkinSelectionWindow(teamPlayer);
}

void waitRoom::createSkinSelectionWindow(team& teamPlayer) {

    selectSkin* windowSkin = new selectSkin(teamPlayer, selectedSkin, this);

    connect(windowSkin, &selectSkin::windowClosed, this, [this]() {
        if (this->selectedSkin == "") {  // caso no coloco un nombre
            return;
        }

        // Mandamos al server la skin del jugador
        SelectSkinRequest requestSkin(this->selectedSkin);
        protocol.send_message(requestSkin);

        std::string imageLocation = ":/images/images/skins/skin" + selectedSkin + ".png";
        QString location = QString::fromStdString(imageLocation);
        QPixmap image(location);
        char team = selectedSkin[0];

        if (team == 'C') {
            ui->labelC->setPixmap(image);
            ui->labelC->setScaledContents(true);
        } else {
            ui->labelT->setPixmap(image);
            ui->labelT->setScaledContents(true);
        }
    });
    windowSkin->exec();
}

void waitRoom::on_startButton_clicked() {

    // Empezamos la partida
    GameReadyRequest gameReady;
    protocol.send_message(gameReady);

    const std::unique_ptr<Message> responseGameReady = protocol.recv_message();
    // const auto game = dynamic_cast<GameReadyResponse*>(responseGameReady.get());
    // bool gameStart = game->get_success();

    this->close();
    QApplication::exit(EXITLOBBY);
}

void waitRoom::ask_game_started() {
    const std::unique_ptr<Message> responseGameReady = protocol.recv_message();
    const auto game = dynamic_cast<GameReadyResponse*>(responseGameReady.get());
    bool gameStart = game->get_success();

    if (gameStart) {
        emit gameReady();  // <-- comunicar al hilo principal
    }
}
