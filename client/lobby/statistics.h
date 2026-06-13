#ifndef STATISTICS_H
#define STATISTICS_H

#include <QDialog>
#include <QTableWidget>
#include <list>
#include <memory>

#include "../../common/dto/game_state_update.h"

namespace Ui {
class Statistics;
}

class Statistics: public QDialog {
    Q_OBJECT

public:
    explicit Statistics(const std::unique_ptr<GameStateUpdate>& gameState,
                         QWidget* parent = nullptr);
    ~Statistics();

private:
    Ui::Statistics* ui;

    void configureTable(QTableWidget* table, int& counter, const PlayerInfo& player);
};

#endif  // STATISTICS_H
