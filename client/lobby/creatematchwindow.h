#ifndef CREATEMATCHWINDOW_H
#define CREATEMATCHWINDOW_H

#include <QDialog>
#include "common/protocol.h"

enum closeType {
    EXITAPP = 0,
    EXITLOBBY = 1
};

namespace Ui {
class createMatchWindow;
}

class createMatchWindow : public QDialog
{
    Q_OBJECT

public:
    createMatchWindow(Protocol& protocolo, std::list<std::string> maps, QWidget *parent = nullptr);
    ~createMatchWindow();

private slots:

    void on_createButton_clicked();

    void on_backButon_clicked();

    void on_startButton_clicked();

private:
    Ui::createMatchWindow *ui;

    Protocol& protocolo;

    void desactivarWidgets();
};

#endif // CREATEMATCHWINDOW_H
