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
        player_skin.h
        time_information.h
        game_clock.cpp
        game_clock.h
        time_information.h
        bomb_site.h
        weapon_info.h
)

target_include_directories(game
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/common
)


add_subdirectory(gun)
