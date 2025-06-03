#ifndef SELECTSKIN_H
#define SELECTSKIN_H

#include <QDialog>

enum team {
    TERRORISTA = 0,
    CONTRATERRORISTA = 1,
};

namespace Ui {
class selectSkin;
}

class selectSkin : public QDialog {
    Q_OBJECT

public:
    explicit selectSkin(team teamPlayer, std::sring& skinSeleccionada, QWidget *parent = nullptr);
    ~selectSkin();

private slots:
    void on_button1_clicked();

    void on_button2_clicked();

    void on_button3_clicked();

    void on_button4_clicked();

private:
    Ui::selectSkin *ui;
};
#endif // SELECTSKIN_H
