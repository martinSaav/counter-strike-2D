# Buscar Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Agregar subdirectorios
add_subdirectory(lobby)
add_subdirectory(graphies)