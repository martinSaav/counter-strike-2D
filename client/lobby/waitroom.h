#ifndef WAITROOM_H
#define WAITROOM_H

#include <QWidget>
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

private:
    void createSkinSelectionWindow(team& teamPlayer);

    void ask_game_started();

    std::thread socketThread;

    Ui::waitRoom* ui;
    Protocol& protocol;
    userType user;
    std::string selectedSkin;
};
#endif  // WAITROOM_H
