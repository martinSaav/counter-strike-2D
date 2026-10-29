#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <list>
#include <string>

#include "../../common/dto/join_game_request.h"
#include "../../common/dto/simple_messages.h"

#include "creatematchwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class LobbyWindow;
}
QT_END_NAMESPACE

class MainWindow: public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(Protocol& protocol, std::string& namePlayer, QWidget* parent = nullptr);
    ~MainWindow();

private Q_SLOTS:
    void on_createButton_clicked();

    void on_joinButton_clicked();

    void on_refreshButton_clicked();

    void on_quitButton_clicked();

    void on_searchButton_clicked();

    void on_loginButton_clicked();

private:
    Ui::LobbyWindow* ui;

    Protocol& protocol;

    std::list<std::string> maps;

    std::string& namePlayer;
};
#endif  // LOBBYWINDOW_H
