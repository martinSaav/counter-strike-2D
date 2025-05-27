#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include "creatematchwindow.h"
#include "loginwindow.h"
#include "../../common/dto/join_game_response.h"
#include "../../common/dto/join_game_request.h"
#include "../../common/dto/login_request.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class LobbyWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Protocol& protocolo, std::string& namePlayer, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_crearButton_clicked();

    void on_unirseButton_clicked();

    void on_actualizarButton_clicked();

    void on_quitButton_clicked();

    void on_buscarButton_clicked();

    void on_loginButton_clicked();

private:
    Ui::LobbyWindow *ui;

    Protocol& protocolo;

    std::list<std::string> maps;

    std::string& namePlayer;
};
#endif // LOBBYWINDOW_H
