#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <list>
#include <string>

#include "../../common/dto/join_game_request.h"
#include "../../common/dto/join_game_response.h"

#include "creatematchwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class LobbyWindow;
}
QT_END_NAMESPACE

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(Protocol& protocolo, std::string& namePlayer, QWidget* parent = nullptr);
    ~MainWindow();

private Q_SLOTS:
    void on_crearButton_clicked();

    void on_unirseButton_clicked();

    void on_actualizarButton_clicked();

    void on_quitButton_clicked();

    void on_buscarButton_clicked();

    void on_loginButton_clicked();

private:
    Ui::LobbyWindow* ui;

    Protocol& protocolo;

    std::list<std::string> maps;

    std::string& namePlayer;
};
#endif  // LOBBYWINDOW_H
