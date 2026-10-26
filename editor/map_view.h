#ifndef MAP_VIEW_H
#define MAP_VIEW_H

#include <QGraphicsView>
#include <QPixmap>
#include <map>
#include <optional>
#include <string>

#include "map_document.h"

// Herramientas del editor. Las de zonas/estructuras se usan arrastrando para dibujar el
// rectangulo; las de spawns y armas con un click.
enum class EditorTool {
    Select,
    Structure,
    BombSite,
    TerroristZone,
    CounterTerroristZone,
    TerroristSpawn,
    CounterTerroristSpawn,
    Weapon,
    Erase
};

// Referencia a un elemento del mapa (que tipo y en que posicion de su lista esta).
struct ElementRef {
    enum class Kind {
        Structure,
        BombSite,
        TerroristZone,
        CounterTerroristZone,
        TerroristSpawn,
        CounterTerroristSpawn,
        Weapon
    };
    Kind kind;
    int index = 0;
    bool operator==(const ElementRef& other) const = default;
};

// Vista del mapa con scroll y zoom. Dibuja el MapDocument y lo modifica segun la herramienta:
// point and click para colocar, drag and drop para mover y para redimensionar desde la esquina.
class MapView: public QGraphicsView {
    Q_OBJECT

public:
    static constexpr int TILE_PX = 8;  // pixeles por tile con zoom 1

    MapView(MapDocument& document, const std::string& assetsPath, QWidget* parent = nullptr);

    void setTool(EditorTool tool);
    void setWeapon(const std::string& weapon) { currentWeapon = weapon; }
    // Vuelve a dibujar todo (al abrir un mapa o cambiar tamano o terreno)
    void rebuild();
    void deleteSelection();

signals:
    void documentChanged();
    void cursorMoved(int tileX, int tileY);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void drawBackground(QPainter* painter, const QRectF& rect) override;
    void drawForeground(QPainter* painter, const QRectF& rect) override;

private:
    enum class DragMode { None, Move, Resize, Draw };

    MapDocument& document;
    std::string assetsPath;
    EditorTool tool = EditorTool::Select;
    std::string currentWeapon = "ak-47";
    std::optional<ElementRef> selected;

    DragMode dragMode = DragMode::None;
    QPoint dragStartTile;
    QPoint dragLastTile;
    std::optional<QRect> drawingRect;

    QPixmap floorTexture;
    QPixmap wallTexture;
    std::map<std::string, QPixmap> weaponPixmaps;

    void loadTextures();
    [[nodiscard]] QPoint tileAt(const QPoint& viewPos) const;
    [[nodiscard]] std::optional<ElementRef> elementAt(const QPoint& tile) const;
    [[nodiscard]] std::optional<MapRect> rectOf(const ElementRef& ref) const;
    MapRect* mutableRectOf(const ElementRef& ref);
    MapPoint* mutablePointOf(const ElementRef& ref);
    [[nodiscard]] bool onResizeHandle(const ElementRef& ref, const QPoint& tile) const;
    void moveSelected(const QPoint& delta);
    void resizeSelected(const QPoint& tile);
    void finishDrawing();
    void placeAt(const QPoint& tile);
    void erase(const ElementRef& ref);
    void changed();
};

#endif  // MAP_VIEW_H
