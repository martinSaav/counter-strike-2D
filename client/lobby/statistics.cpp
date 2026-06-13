#include "statistics.h"

#include <memory>
#include <string>

#include "ui_statistics.h"

Statistics::Statistics(const std::unique_ptr<GameStateUpdate>& gameState, QWidget* parent):
        QDialog(parent), ui(new Ui::Statistics) {
    ui->setupUi(this);

    // Seteo el ancho de las columnas
    ui->tableTeam1->setColumnWidth(0, 250);  // ancho de la columna 0 a 200 píxeles
    for (int i = 1; i <= 4; i++) {
        ui->tableTeam1->setColumnWidth(i, 60);
    }

    // Seteo el ancho de las columnas
    ui->tableTeam2->setColumnWidth(0, 250);  // ancho de la columna 0 a 200 píxeles
    for (int i = 1; i <= 4; i++) {
        ui->tableTeam2->setColumnWidth(i, 60);
    }

    std::list<PlayerInfo> players = gameState->get_players();
    int counterCount = 0;
    int terroristCount = 0;

    // Terrorists = 0x01,
    // CounterTerrorists = 0x02,
    ui->labelWinner->setStyleSheet("color: white;");
    Team teamWinner = gameState->get_game_winner();
    std::string winner;
    if (teamWinner == Team::Terrorists) {
        winner = "Orange team wins";
    } else {
        winner = "Blue team wins";
    }
    QString qtext = QString::fromStdString(winner);
    ui->labelWinner->setText(qtext);

    for (const PlayerInfo& player: players) {

        // Me quedo con la primer letra de la skin
        char team = player.get_skin()[0];
        if (team == 'C') {
            configureTable(ui->tableTeam1, counterCount, player);
        } else {
            configureTable(ui->tableTeam2, terroristCount, player);
        }
    }
}

void Statistics::configureTable(QTableWidget* table, int& counter, const PlayerInfo& player) {
    table->setItem(counter, 0,
                   new QTableWidgetItem(QString::fromStdString(player.get_user_name())));
    table->setItem(counter, 1, new QTableWidgetItem(QString::number(player.get_kills())));
    table->setItem(counter, 2, new QTableWidgetItem(QString::number(player.get_deaths())));
    int difference = player.get_kills() - player.get_deaths();
    table->setItem(counter, 3, new QTableWidgetItem(QString::number(difference)));
    table->setItem(counter, 4, new QTableWidgetItem(QString::number(player.get_money())));
    counter++;
}

Statistics::~Statistics() { delete ui; }
