#ifndef ESTADISTICS_H
#define ESTADISTICS_H

#include <QDialog>
#include <list>
#include <QTableWidget>
#include "../../common/dto/game_state_update.h"

namespace Ui {
class Estadistics;
}

class Estadistics : public QDialog
{
    Q_OBJECT

public:
    Estadistics(std::unique_ptr<GameStateUpdate>& gameState, QWidget *parent = nullptr);
    ~Estadistics();

private:
    Ui::Estadistics *ui;

    void configurarTabla(QTableWidget* tabla, int& contador, PlayerInfo& jugador);
};

#endif // ESTADISTICS_H
