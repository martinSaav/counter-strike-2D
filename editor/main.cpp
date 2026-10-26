#include <QApplication>
#include <cstdlib>
#include <filesystem>
#include <string>

#include "editor_window.h"

namespace {

// Las texturas son las del juego: misma busqueda que el cliente (CS2D_ASSETS_DIR o
// ../client/data al correr desde build/), mas la carpeta donde las deja el instalador.
std::string find_assets_path() {
    if (const char* env = std::getenv("CS2D_ASSETS_DIR")) {
        return env;
    }
    for (const std::string candidate: {"../client/data", "client/data", "/var/cs2d"}) {
        if (std::filesystem::exists(candidate + "/maps")) {
            return candidate;
        }
    }
    return "../client/data";
}

}  // namespace

// Uso: taller_editor [mapa.yaml]
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    EditorWindow window(find_assets_path());
    if (argc > 1) {
        window.openFile(QString::fromLocal8Bit(argv[1]));
    }
    window.show();
    return app.exec();
}
