#ifndef CREATEMATCHWINDOW_H
#define CREATEMATCHWINDOW_H

#include <QDialog>
#include <list>
#include <string>

#include "common/protocol.h"
#include "waitroom.h"

namespace Ui {
class createMatchWindow;
}

class createMatchWindow: public QDialog {
    Q_OBJECT

public:
    createMatchWindow(Protocol& protocolo, const std::list<std::string>& maps,
                      QWidget* parent = nullptr);
    ~createMatchWindow();

private Q_SLOTS:

    void on_createButton_clicked();

    void on_backButon_clicked();

private:
    Ui::createMatchWindow* ui;

    Protocol& protocolo;
};
#endif  // CREATEMATCHWINDOW_H
