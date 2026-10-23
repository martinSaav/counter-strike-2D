#ifndef SELECTSKIN_H
#define SELECTSKIN_H

#include <QDialog>
#include <string>

enum team {
    TERRORIST = 0,
    COUNTERTERRORIST = 1,
};

namespace Ui {
class selectSkin;
}

class selectSkin: public QDialog {
    Q_OBJECT

public:
    selectSkin(team& teamPlayer, std::string& selectedSkin, QWidget* parent = nullptr);
    ~selectSkin();
    // cppcheck-suppress unknownMacro
private slots:
    void on_button1_clicked();

    void on_button2_clicked();

    void on_button3_clicked();

    void on_button4_clicked();

protected:
    void closeEvent(QCloseEvent* event) override;

signals:
    void windowClosed();

private:
    Ui::selectSkin* ui;
    std::string& selectedSkin;
    team& teamPlayer;
};
#endif  // SELECTSKIN_H
