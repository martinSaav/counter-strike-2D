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
            matchStarting = true;
            this->close();
            QApplication::exit(EXITLOBBY);
        });
        // socketThread no puede tocar la interfaz: avisa con la senal y el dialogo se muestra
        // en el hilo de Qt
        connect(this, &waitRoom::connectionLost, this, [this]() { showConnectionLost(QString()); });
        waitingGameStart = true;
        socketThread = std::thread(&waitRoom::ask_game_started, this);
    }
}

waitRoom::~waitRoom() {
    if (socketThread.joinable()) {
        if (waitingGameStart && !connectionClosed) {
            // El hilo sigue bloqueado en recv y el join no volveria nunca: cerrar la conexion lo
            // destraba. (Si la partida ya arranco, el hilo ya termino.)
            connectionClosed = true;
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
        showConnectionLost(QString::fromUtf8(e.what()));
        return;
    }
    const auto game = dynamic_cast<GameReadyResponse*>(responseGameReady.get());
    if (!game || !game->get_success()) {
        QMessageBox::warning(this, "Faltan jugadores",
                             "Necesitás al menos un jugador en cada equipo para arrancar. "
                             "Esperá a que se una alguien más.");
        return;
    }

    matchStarting = true;
    this->close();
    QApplication::exit(EXITLOBBY);
}

void waitRoom::closeEvent(QCloseEvent* event) {
    if (!matchStarting) {
        // Cerrar la sala antes de que arranque la partida es irse de ella. El servidor no deja
        // volver al lobby con la misma conexion, asi que el cliente termina. Antes la ventana se
        // ocultaba y el proceso seguia vivo, sin ventanas y anotado en la partida.
        leaveMatch();
        QApplication::exit(EXITAPP);
    }
    QWidget::closeEvent(event);
}

void waitRoom::leaveMatch() {
    if (connectionClosed.exchange(true)) {
        return;
    }
    try {
        protocol.send_message(DisconnectRequest());
    } catch (const std::exception&) {}  // si ya no hay conexion no hay a quien avisarle
    protocol.kill();
}

void waitRoom::showConnectionLost(const QString& detail) {
    QString text = "Se perdió la conexión con el servidor.";
    if (!detail.isEmpty()) {
        text += " (" + detail + ")";
    }
    QMessageBox::critical(this, "Sin conexión", text + "\nEl cliente se va a cerrar.");
    matchStarting = true;  // la conexion ya no existe: cerrar la ventana no tiene que avisar nada
    QApplication::exit(EXITAPP);
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
        // Si la cerramos nosotros (el jugador se fue de la sala) no es un error
        if (!connectionClosed) {
            std::cerr << "Connection to server lost while waiting: " << e.what() << "\n";
            waitingGameStart = false;
            emit connectionLost();
            return;
        }
    }
    waitingGameStart = false;
}
