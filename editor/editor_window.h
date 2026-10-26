#ifndef EDITOR_WINDOW_H
#define EDITOR_WINDOW_H

#include <QMainWindow>
#include <string>

#include "map_document.h"
#include "map_view.h"

class QComboBox;
class QLabel;
class QLineEdit;
class QListWidget;
class QSpinBox;

// Ventana principal del editor de niveles: menu de archivo, herramientas, propiedades del mapa
// y la lista de validacion que se actualiza con cada cambio.
class EditorWindow: public QMainWindow {
    Q_OBJECT

public:
    explicit EditorWindow(const std::string& assetsPath, QWidget* parent = nullptr);

    void openFile(const QString& path);

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    MapDocument document;
    QString currentPath;
    bool modified = false;

    MapView* view;
    QLineEdit* nameEdit;
    QComboBox* themeCombo;
    QSpinBox* widthSpin;
    QSpinBox* heightSpin;
    QComboBox* weaponCombo;
    QListWidget* validationList;
    QLabel* cursorLabel;

    void buildMenus();
    void buildToolbar();
    void buildSidePanel();
    void loadPropertiesFromDocument();
    void onDocumentChanged();
    void refreshValidation();
    void updateTitle();
    bool confirmDiscard();

    void newMap();
    void openMap();
    bool saveMap();
    bool saveMapAs();
    bool checkCanSave();
    bool writeTo(const QString& path);
};

#endif  // EDITOR_WINDOW_H
