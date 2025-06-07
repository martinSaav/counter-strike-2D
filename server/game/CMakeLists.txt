add_library(game
        chunk.cpp
        collision_detector.cpp
        map.cpp
        player.cpp
        position.cpp
        structure.cpp
        game_manager.cpp
        player.h
        chunk.h
        collision_detector.h
        map.h
        position.h
        structure.h
        game_manager.h
        team.h
        status.h
        player_skin.h
)


target_include_directories(game
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(gun)
