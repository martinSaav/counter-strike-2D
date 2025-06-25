#include "estadistics.h"

#include <memory>
#include <string>

#include "ui_estadistics.h"

Estadistics::Estadistics(const std::unique_ptr<GameStateUpdate>& gameState, QWidget* parent):
        QDialog(parent), ui(new Ui::Estadistics) {
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
    int contadorCounter = 0;
    int contadorTerrorist = 0;

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
    QString qtexto = QString::fromStdString(winner);
    ui->labelWinner->setText(qtexto);

    for (const PlayerInfo& jugador: players) {

        // Me quedo con la primer letra de la skin
        char team = jugador.get_skin()[0];
        if (team == 'C') {
            configurarTabla(ui->tableTeam1, contadorCounter, jugador);
        } else {
            configurarTabla(ui->tableTeam2, contadorTerrorist, jugador);
        }
    }
}

void Estadistics::configurarTabla(QTableWidget* tabla, int& contador, const PlayerInfo& jugador) {
    tabla->setItem(contador, 0,
                   new QTableWidgetItem(QString::fromStdString(jugador.get_user_name())));
    tabla->setItem(contador, 1, new QTableWidgetItem(QString::number(jugador.get_kills())));
    tabla->setItem(contador, 2, new QTableWidgetItem(QString::number(jugador.get_deaths())));
    int diferencia = jugador.get_kills() - jugador.get_deaths();
    tabla->setItem(contador, 3, new QTableWidgetItem(QString::number(diferencia)));
    tabla->setItem(contador, 4, new QTableWidgetItem(QString::number(jugador.get_money())));
    contador++;
}

Estadistics::~Estadistics() { delete ui; }
