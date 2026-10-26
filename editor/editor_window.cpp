#include "editor_window.h"

#include <QActionGroup>
#include <QCloseEvent>
#include <QComboBox>
#include <QDockWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMenuBar>
#include <QMessageBox>
#include <QSpinBox>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

namespace {

// Nombres para mostrar de los terrenos y las armas (en el YAML van los identificadores)
const std::vector<std::pair<std::string, QString>> THEMES = {
        {"desert", "Desierto"}, {"aztec_village", "Pueblo azteca"}, {"training", "Entrenamiento"}};
const std::vector<std::pair<std::string, QString>> WEAPONS = {
        {"ak-47", "AK-47"}, {"m3", "M3"}, {"awp", "AWP"}};

}  // namespace

EditorWindow::EditorWindow(const std::string& assetsPath, QWidget* parent): QMainWindow(parent) {
    view = new MapView(document, assetsPath, this);
    setCentralWidget(view);
    buildMenus();
    buildToolbar();
    buildSidePanel();

    cursorLabel = new QLabel(this);
    statusBar()->addPermanentWidget(cursorLabel);
    connect(view, &MapView::cursorMoved, this,
            [this](int x, int y) { cursorLabel->setText(QString("Tile: %1, %2").arg(x).arg(y)); });
    connect(view, &MapView::documentChanged, this, &EditorWindow::onDocumentChanged);

    resize(1280, 800);
    loadPropertiesFromDocument();
    refreshValidation();
    updateTitle();
}

void EditorWindow::buildMenus() {
    QMenu* fileMenu = menuBar()->addMenu("&Archivo");
    fileMenu->addAction("&Nuevo", QKeySequence::New, this, &EditorWindow::newMap);
    fileMenu->addAction("&Abrir...", QKeySequence::Open, this, &EditorWindow::openMap);
    fileMenu->addAction("&Guardar", QKeySequence::Save, this, &EditorWindow::saveMap);
    fileMenu->addAction("Guardar &como...", QKeySequence::SaveAs, this, &EditorWindow::saveMapAs);
    fileMenu->addSeparator();
    fileMenu->addAction("&Salir", QKeySequence::Quit, this, &QWidget::close);

    QMenu* helpMenu = menuBar()->addMenu("A&yuda");
    helpMenu->addAction("Cómo usar el editor", this, [this]() {
        QMessageBox::information(
                this, "Cómo usar el editor",
                "<b>Herramientas</b> (barra superior o teclas 1 a 9):<br>"
                "• <b>Seleccionar</b>: arrastrá cualquier elemento para moverlo; arrastrá el "
                "cuadradito blanco de la esquina para cambiar su tamaño. Supr lo borra.<br>"
                "• <b>Estructura, Zona de bomba, Inicio TT, Inicio CT</b>: arrastrá para dibujar "
                "el rectángulo.<br>"
                "• <b>Spawn TT, Spawn CT, Arma</b>: hacé click donde va (el arma se elige en el "
                "panel derecho).<br>"
                "• <b>Borrar</b>: click sobre el elemento.<br><br>"
                "La rueda del mouse mueve el mapa y Ctrl + rueda hace zoom.<br>"
                "Para guardar, el mapa necesita las zonas de inicio de los dos equipos (con "
                "spawns) y al menos una zona de bomba: la lista de la derecha muestra lo que "
                "falta.<br><br>"
                "Para jugarlo, copiá el archivo a la carpeta <i>maps</i> del servidor y agregalo "
                "a la lista <i>maps</i> de server_config.yaml.");
    });
}

void EditorWindow::buildToolbar() {
    QToolBar* toolbar = addToolBar("Herramientas");
    toolbar->setToolButtonStyle(Qt::ToolButtonTextOnly);
    auto* group = new QActionGroup(this);
    const std::vector<std::pair<QString, EditorTool>> tools = {
            {"Seleccionar", EditorTool::Select},
            {"Estructura", EditorTool::Structure},
            {"Zona de bomba", EditorTool::BombSite},
            {"Inicio TT", EditorTool::TerroristZone},
            {"Inicio CT", EditorTool::CounterTerroristZone},
            {"Spawn TT", EditorTool::TerroristSpawn},
            {"Spawn CT", EditorTool::CounterTerroristSpawn},
            {"Arma", EditorTool::Weapon},
            {"Borrar", EditorTool::Erase}};
    int shortcut = 1;
    for (const auto& [label, tool]: tools) {
        QAction* action = toolbar->addAction(label);
        action->setCheckable(true);
        action->setShortcut(QKeySequence(QString::number(shortcut++)));
        action->setToolTip(QString("%1 (%2)").arg(label, action->shortcut().toString()));
        group->addAction(action);
        connect(action, &QAction::triggered, this, [this, tool]() { view->setTool(tool); });
        if (tool == EditorTool::Select) {
            action->setChecked(true);
        }
    }
}

void EditorWindow::buildSidePanel() {
    auto* panel = new QWidget(this);
    auto* layout = new QVBoxLayout(panel);
    auto* form = new QFormLayout();

    nameEdit = new QLineEdit(panel);
    themeCombo = new QComboBox(panel);
    for (const auto& [id, label]: THEMES) {
        themeCombo->addItem(label, QString::fromStdString(id));
    }
    widthSpin = new QSpinBox(panel);
    heightSpin = new QSpinBox(panel);
    for (QSpinBox* spin: {widthSpin, heightSpin}) {
        spin->setRange(MapDocument::MIN_MAP_SIZE, 1000);
        spin->setSuffix(" tiles");
    }
    weaponCombo = new QComboBox(panel);
    for (const auto& [id, label]: WEAPONS) {
        weaponCombo->addItem(label, QString::fromStdString(id));
    }

    form->addRow("Nombre", nameEdit);
    form->addRow("Terreno", themeCombo);
    form->addRow("Ancho", widthSpin);
    form->addRow("Alto", heightSpin);
    form->addRow("Arma a colocar", weaponCombo);
    layout->addLayout(form);

    layout->addWidget(new QLabel("<b>Validación</b>", panel));
    validationList = new QListWidget(panel);
    validationList->setWordWrap(true);
    layout->addWidget(validationList, 1);

    auto* dock = new QDockWidget("Mapa", this);
    dock->setWidget(panel);
    dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    dock->setMinimumWidth(300);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    connect(nameEdit, &QLineEdit::textEdited, this, [this](const QString& text) {
        document.name = text.toStdString();
        onDocumentChanged();
    });
    connect(themeCombo, &QComboBox::currentIndexChanged, this, [this](int index) {
        const std::string theme = themeCombo->itemData(index).toString().toStdString();
        if (theme != document.theme) {
            document.theme = theme;
            view->rebuild();
            onDocumentChanged();
        }
    });
    auto onSizeChanged = [this]() {
        if (widthSpin->value() != document.width || heightSpin->value() != document.height) {
            document.width = widthSpin->value();
            document.height = heightSpin->value();
            view->rebuild();
            onDocumentChanged();
        }
    };
    connect(widthSpin, &QSpinBox::valueChanged, this, onSizeChanged);
    connect(heightSpin, &QSpinBox::valueChanged, this, onSizeChanged);
    connect(weaponCombo, &QComboBox::currentIndexChanged, this, [this](int index) {
        view->setWeapon(weaponCombo->itemData(index).toString().toStdString());
    });
}

void EditorWindow::loadPropertiesFromDocument() {
    // Se bloquean las señales para que cargar los valores no cuente como una edicion
    for (QWidget* widget: std::vector<QWidget*>{nameEdit, themeCombo, widthSpin, heightSpin}) {
        widget->blockSignals(true);
    }
    nameEdit->setText(QString::fromStdString(document.name));
    themeCombo->setCurrentIndex(themeCombo->findData(QString::fromStdString(document.theme)));
    widthSpin->setValue(document.width);
    heightSpin->setValue(document.height);
    for (QWidget* widget: std::vector<QWidget*>{nameEdit, themeCombo, widthSpin, heightSpin}) {
        widget->blockSignals(false);
    }
}

void EditorWindow::onDocumentChanged() {
    modified = true;
    refreshValidation();
    updateTitle();
}

void EditorWindow::refreshValidation() {
    const MapValidation result = document.validate();
    validationList->clear();
    for (const auto& error: result.errors) {
        auto* item = new QListWidgetItem("✗ " + QString::fromStdString(error), validationList);
        item->setForeground(QColor(200, 40, 40));
    }
    for (const auto& warning: result.warnings) {
        auto* item = new QListWidgetItem("⚠ " + QString::fromStdString(warning), validationList);
        item->setForeground(QColor(180, 120, 0));
    }
    if (result.errors.empty() && result.warnings.empty()) {
        auto* item = new QListWidgetItem("✓ El mapa está listo para jugar", validationList);
        item->setForeground(QColor(30, 140, 30));
    }
}

void EditorWindow::updateTitle() {
    const QString file = currentPath.isEmpty() ? "sin guardar" : QFileInfo(currentPath).fileName();
    setWindowTitle(QString("Editor de niveles - %1%2").arg(file, modified ? " *" : ""));
}

bool EditorWindow::confirmDiscard() {
    if (!modified) {
        return true;
    }
    const auto answer = QMessageBox::question(
            this, "Cambios sin guardar", "El mapa tiene cambios sin guardar. ¿Guardarlos?",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (answer == QMessageBox::Save) {
        return saveMap();
    }
    return answer == QMessageBox::Discard;
}

void EditorWindow::newMap() {
    if (!confirmDiscard()) {
        return;
    }
    document = MapDocument();
    currentPath.clear();
    modified = false;
    loadPropertiesFromDocument();
    view->rebuild();
    refreshValidation();
    updateTitle();
}

void EditorWindow::openMap() {
    if (!confirmDiscard()) {
        return;
    }
    const QString path =
            QFileDialog::getOpenFileName(this, "Abrir mapa", currentPath, "Mapas (*.yaml *.yml)");
    if (!path.isEmpty()) {
        openFile(path);
    }
}

void EditorWindow::openFile(const QString& path) {
    try {
        document = MapDocument::load(path.toStdString());
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "No se pudo abrir", e.what());
        return;
    }
    currentPath = path;
    modified = false;
    loadPropertiesFromDocument();
    view->rebuild();
    refreshValidation();
    updateTitle();
}

bool EditorWindow::saveMap() {
    if (currentPath.isEmpty()) {
        return saveMapAs();
    }
    return writeTo(currentPath);
}

bool EditorWindow::saveMapAs() {
    if (!checkCanSave()) {
        return false;
    }
    const QString suggested =
            currentPath.isEmpty() ? QString::fromStdString(document.name) + ".yaml" : currentPath;
    QString path =
            QFileDialog::getSaveFileName(this, "Guardar mapa", suggested, "Mapas (*.yaml *.yml)");
    if (path.isEmpty()) {
        return false;
    }
    if (!path.endsWith(".yaml") && !path.endsWith(".yml")) {
        path += ".yaml";
    }
    return writeTo(path);
}

bool EditorWindow::checkCanSave() {
    // El enunciado pide validar antes de guardar: zonas de inicio de los dos equipos y al menos
    // una zona de bomba (entre otras reglas, ver MapDocument::validate)
    const MapValidation result = document.validate();
    if (result.ok()) {
        return true;
    }
    const QString message =
            std::accumulate(result.errors.begin(), result.errors.end(),
                            QString("No se puede guardar el mapa todavía:\n"),
                            [](const QString& text, const std::string& error) {
                                return text + "\n• " + QString::fromStdString(error);
                            });
    QMessageBox::warning(this, "Mapa incompleto", message);
    return false;
}

bool EditorWindow::writeTo(const QString& path) {
    if (!checkCanSave()) {
        return false;
    }
    try {
        document.save(path.toStdString());
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "No se pudo guardar", e.what());
        return false;
    }
    currentPath = path;
    modified = false;
    updateTitle();
    statusBar()->showMessage("Mapa guardado en " + path, 5000);
    return true;
}

void EditorWindow::closeEvent(QCloseEvent* event) {
    if (confirmDiscard()) {
        event->accept();
    } else {
        event->ignore();
    }
}
