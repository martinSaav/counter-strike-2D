#ifndef CREATEMATCHWINDOW_H
#define CREATEMATCHWINDOW_H

#include <QWidget>

enum closeType {
    EXITAPP = 0,
    EXITLOBBY = 1
};

namespace Ui {
class createMatchWindow;
}

class createMatchWindow : public QWidget
{
    Q_OBJECT

public:
    explicit createMatchWindow(std::list<std::string> maps, QWidget *parent = nullptr);
    ~createMatchWindow();

private slots:

    void on_createButton_clicked();

    void on_backButon_clicked();

    void on_startButton_clicked();

private:
    Ui::createMatchWindow *ui;

    void desactivarWidgets();
};

#endif // CREATEMATCHWINDOW_H
