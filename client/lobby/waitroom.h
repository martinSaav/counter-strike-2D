#ifndef WAITROOM_H
#define WAITROOM_H

#include <QCloseEvent>
#include <QWidget>
#include <atomic>
#include <string>
#include <thread>

#include "common/dto/select_skin_request.h"
#include "common/dto/simple_messages.h"
#include "common/protocol.h"

#include "selectskin.h"

enum closeType { EXITAPP = 0, EXITLOBBY = 1 };
enum userType { CREATOR = 0, JOINED = 1 };

namespace Ui {
class waitRoom;
}

class waitRoom: public QWidget {
    Q_OBJECT

public:
    explicit waitRoom(Protocol& protocol, userType& user, QWidget* parent = nullptr);
    ~waitRoom();

private slots:
    void on_terroristButton_clicked();

    void on_counterTerroristButton_clicked();

    void on_startButton_clicked();

signals:
    void gameReady();

    void connectionLost();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    void createSkinSelectionWindow(team& teamPlayer);

    void ask_game_started();

    void leaveMatch();

    void showConnectionLost(const QString& detail);

    std::thread socketThread;
    // true mientras socketThread esta bloqueado esperando el aviso de que arranco la partida
    std::atomic<bool> waitingGameStart{false};
    // true si la ventana se cierra porque arranca la partida: no hay que irse de ella
    bool matchStarting = false;
    // true cuando ya cerramos la conexion: socketThread no tiene que avisar un error ni hay que
    // volver a cerrarla en el destructor
    std::atomic<bool> connectionClosed{false};

    Ui::waitRoom* ui;
    Protocol& protocol;
    userType user;
    std::string selectedSkin;
};
#endif  // WAITROOM_H
