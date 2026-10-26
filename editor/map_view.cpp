#include "map_view.h"

#include <QBitmap>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <utility>
#include <vector>

namespace {

const QColor BOMB_COLOR(220, 40, 40, 90);
const QColor TERRORIST_COLOR(230, 150, 30, 80);
const QColor COUNTER_TERRORIST_COLOR(40, 110, 230, 80);
const QColor SELECTION_COLOR(255, 255, 255);

QRectF toScene(const MapRect& rect) {
    return QRectF(rect.x * MapView::TILE_PX, rect.y * MapView::TILE_PX,
                  rect.width * MapView::TILE_PX, rect.height * MapView::TILE_PX);
}

QRectF toScene(const MapPoint& point, int size) {
    return toScene(MapRect{point.x, point.y, size, size});
}

MapRect normalized(const QPoint& a, const QPoint& b) {
    const int x = std::min(a.x(), b.x());
    const int y = std::min(a.y(), b.y());
    return MapRect{x, y, std::abs(a.x() - b.x()) + 1, std::abs(a.y() - b.y()) + 1};
}

void drawLabel(QPainter* painter, const QRectF& rect, const QString& text, const QColor& color) {
    painter->setPen(color);
    QFont font = painter->font();
    font.setBold(true);
    font.setPixelSize(std::max(10, static_cast<int>(std::min(rect.height() / 3, 28.0))));
    painter->setFont(font);
    painter->drawText(rect, Qt::AlignCenter, text);
}

}  // namespace

MapView::MapView(MapDocument& document, const std::string& assetsPath, QWidget* parent):
        QGraphicsView(parent), document(document), assetsPath(assetsPath) {
    // Todo se dibuja en drawBackground/drawForeground a partir del documento; la escena solo
    // define el tamano del mapa para que QGraphicsView maneje el scroll y el zoom.
    setScene(new QGraphicsScene(this));
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setMouseTracking(true);
    setDragMode(QGraphicsView::NoDrag);
    setFocusPolicy(Qt::StrongFocus);
    setBackgroundBrush(QColor(30, 30, 30));
    rebuild();
}

void MapView::setTool(EditorTool newTool) {
    tool = newTool;
    if (tool != EditorTool::Select) {
        selected.reset();
    }
    viewport()->update();
}

void MapView::rebuild() {
    loadTextures();
    scene()->setSceneRect(0, 0, document.width * TILE_PX, document.height * TILE_PX);
    if (selected && !rectOf(*selected)) {
        selected.reset();
    }
    viewport()->update();
}

void MapView::loadTextures() {
    // Mismas texturas que usa el juego para cada terreno
    const QString mapsDir = QString::fromStdString(assetsPath + "/maps/" + document.theme);
    // Las texturas son mosaicos de tiles de 32 px que ocupan 4x4 tiles del mapa, igual que en el
    // juego: con TILE_PX = 8 se usan sin escalar
    floorTexture = QPixmap(mapsDir + "/floor.png");
    wallTexture = QPixmap(mapsDir + "/wall.png");
    const std::map<std::string, std::string> files = {
            {"ak-47", "ak47_d.bmp"}, {"m3", "m3_d.bmp"}, {"awp", "awp_d.bmp"}};
    weaponPixmaps.clear();
    for (const auto& [weapon, file]: files) {
        QPixmap pixmap(QString::fromStdString(assetsPath + "/weapons/" + file));
        if (!pixmap.isNull()) {
            // En el juego el negro es transparente
            pixmap.setMask(pixmap.createMaskFromColor(Qt::black));
            weaponPixmaps[weapon] = pixmap;
        }
    }
}

QPoint MapView::tileAt(const QPoint& viewPos) const {
    const QPointF scenePos = mapToScene(viewPos);
    return QPoint(static_cast<int>(std::floor(scenePos.x() / TILE_PX)),
                  static_cast<int>(std::floor(scenePos.y() / TILE_PX)));
}

std::optional<MapRect> MapView::rectOf(const ElementRef& ref) const {
    auto pointRect = [](const std::vector<MapPoint>& points, int index, int size) {
        return (index >= 0 && index < static_cast<int>(points.size())) ?
                       std::optional(MapRect{points[index].x, points[index].y, size, size}) :
                       std::nullopt;
    };
    switch (ref.kind) {
        case ElementRef::Kind::Structure:
            if (ref.index < static_cast<int>(document.structures.size())) {
                return document.structures[ref.index];
            }
            return std::nullopt;
        case ElementRef::Kind::BombSite:
            if (ref.index < static_cast<int>(document.bomb_sites.size())) {
                return document.bomb_sites[ref.index];
            }
            return std::nullopt;
        case ElementRef::Kind::TerroristZone:
            return document.tt_zone.defined ? std::optional(document.tt_zone.area) : std::nullopt;
        case ElementRef::Kind::CounterTerroristZone:
            return document.ct_zone.defined ? std::optional(document.ct_zone.area) : std::nullopt;
        case ElementRef::Kind::TerroristSpawn:
            return pointRect(document.tt_zone.spawns, ref.index, MapDocument::PLAYER_SIZE);
        case ElementRef::Kind::CounterTerroristSpawn:
            return pointRect(document.ct_zone.spawns, ref.index, MapDocument::PLAYER_SIZE);
        case ElementRef::Kind::Weapon:
            if (ref.index < static_cast<int>(document.weapon_drops.size())) {
                const MapPoint& p = document.weapon_drops[ref.index].position;
                return MapRect{p.x, p.y, MapDocument::WEAPON_SIZE, MapDocument::WEAPON_SIZE};
            }
            return std::nullopt;
    }
    return std::nullopt;
}

MapRect* MapView::mutableRectOf(const ElementRef& ref) {
    switch (ref.kind) {
        case ElementRef::Kind::Structure:
            return &document.structures[ref.index];
        case ElementRef::Kind::BombSite:
            return &document.bomb_sites[ref.index];
        case ElementRef::Kind::TerroristZone:
            return &document.tt_zone.area;
        case ElementRef::Kind::CounterTerroristZone:
            return &document.ct_zone.area;
        default:
            return nullptr;
    }
}

MapPoint* MapView::mutablePointOf(const ElementRef& ref) {
    switch (ref.kind) {
        case ElementRef::Kind::TerroristSpawn:
            return &document.tt_zone.spawns[ref.index];
        case ElementRef::Kind::CounterTerroristSpawn:
            return &document.ct_zone.spawns[ref.index];
        case ElementRef::Kind::Weapon:
            return &document.weapon_drops[ref.index].position;
        default:
            return nullptr;
    }
}

std::optional<ElementRef> MapView::elementAt(const QPoint& tile) const {
    // Primero lo mas chico (armas y spawns) para poder agarrarlo aunque este sobre una zona
    auto hit = [&](const MapRect& r) { return r.contains(tile.x(), tile.y(), 1); };
    using Kind = ElementRef::Kind;
    for (int i = static_cast<int>(document.weapon_drops.size()) - 1; i >= 0; --i) {
        if (hit(*rectOf({Kind::Weapon, i}))) {
            return ElementRef{Kind::Weapon, i};
        }
    }
    for (int i = static_cast<int>(document.tt_zone.spawns.size()) - 1; i >= 0; --i) {
        if (hit(*rectOf({Kind::TerroristSpawn, i}))) {
            return ElementRef{Kind::TerroristSpawn, i};
        }
    }
    for (int i = static_cast<int>(document.ct_zone.spawns.size()) - 1; i >= 0; --i) {
        if (hit(*rectOf({Kind::CounterTerroristSpawn, i}))) {
            return ElementRef{Kind::CounterTerroristSpawn, i};
        }
    }
    for (int i = static_cast<int>(document.structures.size()) - 1; i >= 0; --i) {
        if (hit(document.structures[i])) {
            return ElementRef{Kind::Structure, i};
        }
    }
    for (int i = static_cast<int>(document.bomb_sites.size()) - 1; i >= 0; --i) {
        if (hit(document.bomb_sites[i])) {
            return ElementRef{Kind::BombSite, i};
        }
    }
    if (document.tt_zone.defined && hit(document.tt_zone.area)) {
        return ElementRef{Kind::TerroristZone, 0};
    }
    if (document.ct_zone.defined && hit(document.ct_zone.area)) {
        return ElementRef{Kind::CounterTerroristZone, 0};
    }
    return std::nullopt;
}

bool MapView::onResizeHandle(const ElementRef& ref, const QPoint& tile) const {
    // El tirador es el tile de la esquina inferior derecha de los rectangulos
    if (ref.kind != ElementRef::Kind::Structure && ref.kind != ElementRef::Kind::BombSite &&
        ref.kind != ElementRef::Kind::TerroristZone &&
        ref.kind != ElementRef::Kind::CounterTerroristZone) {
        return false;
    }
    const auto rect = rectOf(ref);
    return rect && tile.x() >= rect->x + rect->width - 2 && tile.x() <= rect->x + rect->width &&
           tile.y() >= rect->y + rect->height - 2 && tile.y() <= rect->y + rect->height;
}

void MapView::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton) {
        QGraphicsView::mousePressEvent(event);
        return;
    }
    const QPoint tile = tileAt(event->pos());
    dragStartTile = tile;
    dragLastTile = tile;

    switch (tool) {
        case EditorTool::Select: {
            selected = elementAt(tile);
            if (selected) {
                dragMode = onResizeHandle(*selected, tile) ? DragMode::Resize : DragMode::Move;
            }
            break;
        }
        case EditorTool::Structure:
        case EditorTool::BombSite:
        case EditorTool::TerroristZone:
        case EditorTool::CounterTerroristZone:
            dragMode = DragMode::Draw;
            drawingRect = QRect(tile, tile);
            break;
        case EditorTool::Erase:
            if (const auto ref = elementAt(tile)) {
                erase(*ref);
            }
            break;
        default:
            placeAt(tile);
            break;
    }
    viewport()->update();
}

void MapView::mouseMoveEvent(QMouseEvent* event) {
    const QPoint tile = tileAt(event->pos());
    emit this->cursorMoved(tile.x(), tile.y());
    if (dragMode == DragMode::Move && selected && tile != dragLastTile) {
        moveSelected(tile - dragLastTile);
        dragLastTile = tile;
    } else if (dragMode == DragMode::Resize && selected) {
        resizeSelected(tile);
    } else if (dragMode == DragMode::Draw) {
        drawingRect = QRect(dragStartTile, tile);
        viewport()->update();
    }
    // Cursor que avisa si se puede redimensionar
    if (tool == EditorTool::Select && selected && onResizeHandle(*selected, tile)) {
        viewport()->setCursor(Qt::SizeFDiagCursor);
    } else {
        viewport()->setCursor(tool == EditorTool::Select ? Qt::ArrowCursor : Qt::CrossCursor);
    }
}

void MapView::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if (dragMode == DragMode::Draw) {
            finishDrawing();
        } else if ((dragMode == DragMode::Move || dragMode == DragMode::Resize) &&
                   dragLastTile != dragStartTile) {
            changed();
        }
        dragMode = DragMode::None;
        drawingRect.reset();
        viewport()->update();
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void MapView::moveSelected(const QPoint& delta) {
    if (MapRect* rect = mutableRectOf(*selected)) {
        // Mover una zona de inicio arrastra tambien sus spawns
        if (selected->kind == ElementRef::Kind::TerroristZone ||
            selected->kind == ElementRef::Kind::CounterTerroristZone) {
            TeamZone& zone = selected->kind == ElementRef::Kind::TerroristZone ? document.tt_zone :
                                                                                 document.ct_zone;
            for (auto& spawn: zone.spawns) {
                spawn.x += delta.x();
                spawn.y += delta.y();
            }
        }
        rect->x += delta.x();
        rect->y += delta.y();
    } else if (MapPoint* point = mutablePointOf(*selected)) {
        point->x += delta.x();
        point->y += delta.y();
    }
    viewport()->update();
}

void MapView::resizeSelected(const QPoint& tile) {
    MapRect* rect = mutableRectOf(*selected);
    if (!rect) {
        return;
    }
    rect->width = std::max(1, tile.x() - rect->x + 1);
    rect->height = std::max(1, tile.y() - rect->y + 1);
    dragLastTile = tile;
    viewport()->update();
}

void MapView::finishDrawing() {
    if (!drawingRect) {
        return;
    }
    const MapRect rect = normalized(drawingRect->topLeft(), drawingRect->bottomRight());
    switch (tool) {
        case EditorTool::Structure:
            document.structures.push_back(rect);
            selected = ElementRef{ElementRef::Kind::Structure,
                                  static_cast<int>(document.structures.size()) - 1};
            break;
        case EditorTool::BombSite:
            document.bomb_sites.push_back(rect);
            selected = ElementRef{ElementRef::Kind::BombSite,
                                  static_cast<int>(document.bomb_sites.size()) - 1};
            break;
        case EditorTool::TerroristZone:
            // Hay una sola zona por equipo: dibujar otra la reemplaza (los spawns se conservan)
            document.tt_zone.defined = true;
            document.tt_zone.area = rect;
            break;
        case EditorTool::CounterTerroristZone:
            document.ct_zone.defined = true;
            document.ct_zone.area = rect;
            break;
        default:
            return;
    }
    changed();
}

void MapView::placeAt(const QPoint& tile) {
    switch (tool) {
        case EditorTool::TerroristSpawn:
            document.tt_zone.spawns.push_back(MapPoint{tile.x(), tile.y()});
            break;
        case EditorTool::CounterTerroristSpawn:
            document.ct_zone.spawns.push_back(MapPoint{tile.x(), tile.y()});
            break;
        case EditorTool::Weapon:
            document.weapon_drops.push_back(
                    WeaponDrop{currentWeapon, MapPoint{tile.x(), tile.y()}});
            break;
        default:
            return;
    }
    changed();
}

void MapView::erase(const ElementRef& ref) {
    switch (ref.kind) {
        case ElementRef::Kind::Structure:
            document.structures.erase(document.structures.begin() + ref.index);
            break;
        case ElementRef::Kind::BombSite:
            document.bomb_sites.erase(document.bomb_sites.begin() + ref.index);
            break;
        case ElementRef::Kind::TerroristZone:
            document.tt_zone.defined = false;
            break;
        case ElementRef::Kind::CounterTerroristZone:
            document.ct_zone.defined = false;
            break;
        case ElementRef::Kind::TerroristSpawn:
            document.tt_zone.spawns.erase(document.tt_zone.spawns.begin() + ref.index);
            break;
        case ElementRef::Kind::CounterTerroristSpawn:
            document.ct_zone.spawns.erase(document.ct_zone.spawns.begin() + ref.index);
            break;
        case ElementRef::Kind::Weapon:
            document.weapon_drops.erase(document.weapon_drops.begin() + ref.index);
            break;
    }
    selected.reset();
    changed();
}

void MapView::deleteSelection() {
    if (selected) {
        erase(*selected);
    }
}

void MapView::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
        deleteSelection();
        return;
    }
    QGraphicsView::keyPressEvent(event);
}

void MapView::wheelEvent(QWheelEvent* event) {
    // Ctrl + rueda hace zoom; la rueda sola scrollea
    if (event->modifiers() & Qt::ControlModifier) {
        const double factor = event->angleDelta().y() > 0 ? 1.25 : 0.8;
        const double current = transform().m11();
        if ((factor > 1 && current < 8) || (factor < 1 && current > 0.25)) {
            setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
            scale(factor, factor);
        }
        return;
    }
    QGraphicsView::wheelEvent(event);
}

void MapView::changed() {
    viewport()->update();
    emit documentChanged();
}

void MapView::drawBackground(QPainter* painter, const QRectF& rect) {
    QGraphicsView::drawBackground(painter, rect);
    const QRectF mapRect(0, 0, document.width * TILE_PX, document.height * TILE_PX);

    // Terreno
    if (!floorTexture.isNull()) {
        painter->fillRect(mapRect, QBrush(floorTexture));
    } else {
        painter->fillRect(mapRect, QColor(110, 110, 100));
    }

    // Zonas de inicio y de bomba (semitransparentes, debajo de lo demas)
    auto drawZone = [&](const MapRect& area, const QColor& color, const QString& label) {
        const QRectF r = toScene(area);
        painter->fillRect(r, color);
        painter->setPen(QPen(color.darker(150), 2));
        painter->drawRect(r);
        drawLabel(painter, r, label, color.darker(200));
    };
    if (document.tt_zone.defined) {
        drawZone(document.tt_zone.area, TERRORIST_COLOR, "Inicio TT");
    }
    if (document.ct_zone.defined) {
        drawZone(document.ct_zone.area, COUNTER_TERRORIST_COLOR, "Inicio CT");
    }
    for (size_t i = 0; i < document.bomb_sites.size(); ++i) {
        drawZone(document.bomb_sites[i], BOMB_COLOR,
                 QString("Bomba %1").arg(QChar('A' + static_cast<int>(i % 26))));
    }

    // Estructuras solidas
    for (const auto& structure: document.structures) {
        const QRectF r = toScene(structure);
        if (!wallTexture.isNull()) {
            // El mosaico arranca en la esquina de cada estructura, como en el juego
            painter->setBrushOrigin(r.topLeft());
            painter->fillRect(r, QBrush(wallTexture));
            painter->setBrushOrigin(0, 0);
        } else {
            painter->fillRect(r, QColor(120, 90, 60));
        }
        painter->setPen(QPen(QColor(40, 30, 20), 1));
        painter->drawRect(r);
    }

    // Spawns y armas
    auto drawSpawn = [&](const MapPoint& spawn, const QColor& color, const QString& label) {
        const QRectF r = toScene(spawn, MapDocument::PLAYER_SIZE);
        painter->setBrush(color);
        painter->setPen(QPen(Qt::black, 1));
        painter->drawEllipse(r);
        painter->setBrush(Qt::NoBrush);
        drawLabel(painter, r, label, Qt::white);
    };
    for (const auto& spawn: document.tt_zone.spawns) {
        drawSpawn(spawn, QColor(230, 150, 30), "T");
    }
    for (const auto& spawn: document.ct_zone.spawns) {
        drawSpawn(spawn, QColor(40, 110, 230), "CT");
    }
    for (const auto& drop: document.weapon_drops) {
        // El sprite del juego es chico y oscuro: va sobre una placa clara, algo mas grande que
        // el lugar que ocupa el arma, con el nombre abajo
        const QRectF r = toScene(drop.position, MapDocument::WEAPON_SIZE);
        const QRectF badge = r.adjusted(-TILE_PX, -TILE_PX / 2.0, TILE_PX, TILE_PX * 1.5);
        painter->setPen(QPen(QColor(90, 70, 0), 1));
        painter->setBrush(QColor(255, 240, 170, 220));
        painter->drawRoundedRect(badge, 3, 3);
        const auto pixmap = weaponPixmaps.find(drop.weapon);
        if (pixmap != weaponPixmaps.end()) {
            painter->setRenderHint(QPainter::SmoothPixmapTransform);
            painter->drawPixmap(badge.adjusted(2, 1, -2, -badge.height() / 2.5).toRect(),
                                pixmap->second);
        }
        QFont font = painter->font();
        font.setBold(true);
        font.setPixelSize(std::max(8, static_cast<int>(badge.height() / 3)));
        painter->setFont(font);
        painter->setPen(QColor(60, 40, 0));
        painter->drawText(badge.adjusted(0, badge.height() / 2, 0, 0), Qt::AlignCenter,
                          QString::fromStdString(drop.weapon).toUpper());
    }

    // Borde del mapa
    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(mapRect);
}

void MapView::drawForeground(QPainter* painter, const QRectF& rect) {
    // Grilla cada 5 tiles, solo si el zoom deja verla
    if (transform().m11() >= 0.75) {
        painter->setPen(QPen(QColor(0, 0, 0, 40), 0));
        const int step = 5 * TILE_PX;
        const int left = std::max(0, static_cast<int>(rect.left()) / step * step);
        const int top = std::max(0, static_cast<int>(rect.top()) / step * step);
        const int right = std::min(static_cast<int>(rect.right()), document.width * TILE_PX);
        const int bottom = std::min(static_cast<int>(rect.bottom()), document.height * TILE_PX);
        for (int x = left; x <= right; x += step) {
            painter->drawLine(x, top, x, bottom);
        }
        for (int y = top; y <= bottom; y += step) {
            painter->drawLine(left, y, right, y);
        }
    }

    // Rectangulo que se esta dibujando
    if (drawingRect) {
        const QRectF r = toScene(normalized(drawingRect->topLeft(), drawingRect->bottomRight()));
        painter->setPen(QPen(Qt::white, 0, Qt::DashLine));
        painter->setBrush(QColor(255, 255, 255, 40));
        painter->drawRect(r);
    }

    // Seleccion con su tirador para redimensionar
    if (selected) {
        if (const auto selectedRect = rectOf(*selected)) {
            const QRectF r = toScene(*selectedRect);
            painter->setPen(QPen(SELECTION_COLOR, 0, Qt::DashLine));
            painter->setBrush(Qt::NoBrush);
            painter->drawRect(r);
            if (mutableRectOf(*selected)) {
                painter->fillRect(QRectF(r.right() - 2 * TILE_PX, r.bottom() - 2 * TILE_PX,
                                         2 * TILE_PX, 2 * TILE_PX),
                                  SELECTION_COLOR);
            }
        }
    }
}
