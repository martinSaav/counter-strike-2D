#ifndef ESTADISTICS_H
#define ESTADISTICS_H

#include <QDialog>
#include <QTableWidget>
#include <list>
#include <memory>

#include "../../common/dto/game_state_update.h"

namespace Ui {
class Estadistics;
}

class Estadistics: public QDialog {
    Q_OBJECT

public:
    explicit Estadistics(const std::unique_ptr<GameStateUpdate>& gameState,
                         QWidget* parent = nullptr);
    ~Estadistics();

private:
    Ui::Estadistics* ui;

    void configurarTabla(QTableWidget* tabla, int& contador, const PlayerInfo& jugador);
};

#endif  // ESTADISTICS_H
