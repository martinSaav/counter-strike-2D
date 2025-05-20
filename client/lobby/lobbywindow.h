#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include "creatematchwindow.h"
#include "common/protocol.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class LobbyWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Protocol& protocolo, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_crearButton_clicked();

    void on_unirseButton_clicked();

    void on_actualizarButton_clicked();

    void on_quitButton_clicked();

    void on_buscarButton_clicked();

private:
    Ui::LobbyWindow *ui;

    Protocol& protocolo;
};
#endif // LOBBYWINDOW_H
