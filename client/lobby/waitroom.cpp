#include "waitroom.h"

#include <QMessageBox>
#include <exception>
#include <iostream>
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
        waitingGameStart = true;
        socketThread = std::thread(&waitRoom::ask_game_started, this);
    }
}

waitRoom::~waitRoom() {
    if (socketThread.joinable()) {
        if (waitingGameStart) {
            // Se cerro la ventana antes de que arranque la partida: el hilo sigue bloqueado en
            // recv y el join no volveria nunca. Cerrar la conexion lo destraba y le avisa al
            // servidor que nos fuimos. (Si la partida ya arranco, el hilo ya termino.)
            protocol.kill();
        }
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
    const auto answer = QMessageBox::question(
            this, "Arrancar partida",
            "¿Arrancar la partida ahora? Una vez iniciada no se puede unir nadie más.");
    if (answer != QMessageBox::Yes) {
        return;
    }

    // Empezamos la partida
    std::unique_ptr<Message> responseGameReady;
    try {
        GameReadyRequest gameReady;
        protocol.send_message(gameReady);
        responseGameReady = protocol.recv_message();
    } catch (const std::exception& e) {
        // Sin esto la excepcion atravesaba el loop de eventos de Qt y cerraba el cliente
        QMessageBox::critical(this, "Sin conexión",
                              QString("Se perdió la conexión con el servidor: %1").arg(e.what()));
        QApplication::exit(EXITAPP);
        return;
    }
    const auto game = dynamic_cast<GameReadyResponse*>(responseGameReady.get());
    if (!game || !game->get_success()) {
        QMessageBox::warning(this, "Faltan jugadores",
                             "Necesitás al menos un jugador en cada equipo para arrancar. "
                             "Esperá a que se una alguien más.");
        return;
    }

    this->close();
    QApplication::exit(EXITLOBBY);
}

void waitRoom::ask_game_started() {
    // Una excepcion que se escapa de un std::thread llama a std::terminate: si el servidor corta
    // la conexion (o la cerramos nosotros al salir) simplemente no hay partida.
    try {
        const std::unique_ptr<Message> responseGameReady = protocol.recv_message();
        const auto game = dynamic_cast<GameReadyResponse*>(responseGameReady.get());
        if (game && game->get_success()) {
            waitingGameStart = false;
            emit gameReady();  // <-- comunicar al hilo principal
            return;
        }
    } catch (const std::exception& e) {
        std::cerr << "Connection to server lost while waiting: " << e.what() << "\n";
    }
    waitingGameStart = false;
}
