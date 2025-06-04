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
    selectSkin(team& teamPlayer, std::string& skinSeleccionada, QWidget *parent = nullptr);
    ~selectSkin();

private slots:
    void on_button1_clicked();

    void on_button2_clicked();

    void on_button3_clicked();

    void on_button4_clicked();

protected:
    void closeEvent(QCloseEvent* event) override;

signals:
    void ventanaCerrada();

private:
    Ui::selectSkin *ui;
    std::string& skinSeleccionada;
    team& teamPlayer;
};
#endif // SELECTSKIN_H
