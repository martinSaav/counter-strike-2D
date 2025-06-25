#ifndef WAITROOM_H
#define WAITROOM_H

#include <QWidget>
#include <thread>

#include "common/dto/game_ready_request.h"
#include "common/dto/game_ready_response.h"
#include "common/dto/select_skin_request.h"
#include "common/protocol.h"

#include "selectskin.h"

enum closeType { EXITAPP = 0, EXITLOBBY = 1 };
enum tipoUsuario { CREADOR = 0, UNIDO = 1 };

namespace Ui {
class waitRoom;
}

class waitRoom: public QWidget {
    Q_OBJECT

public:
    explicit waitRoom(Protocol& protocolo, tipoUsuario& usuario, QWidget* parent = nullptr);
    ~waitRoom();

private slots:
    void on_terroristaButton_clicked();

    void on_contraterroristaButton_clicked();

    void on_empezarButton_clicked();

signals:
    void gameReady();

private:
    void crearVentanaSeleccionSkin(team& teamPlayer);

    void ask_game_started();

    std::thread socketThread;

    Ui::waitRoom* ui;
    Protocol& protocolo;
    tipoUsuario usuario;
    std::string skinSeleccionada;
};
#endif  // WAITROOM_H
