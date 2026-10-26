# Modelo del mapa (sin Qt): lo usan el editor y los tests
add_library(editor_model STATIC
    map_document.cpp
    map_document.h
)
target_include_directories(editor_model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(editor_model PUBLIC yaml-cpp)

target_sources(taller_editor
    PRIVATE
        main.cpp
        editor_window.cpp
        map_view.cpp
    PUBLIC
        editor_window.h
        map_view.h
)
