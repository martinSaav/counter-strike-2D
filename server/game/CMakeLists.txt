add_library(game
        chunk.cpp
        collision_detector.cpp
        map.cpp
        player.cpp
        position.cpp
        structure.cpp
        game_manager.cpp
)

target_include_directories(game
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(gun)
