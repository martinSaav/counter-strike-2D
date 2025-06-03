target_sources(taller_server
    PRIVATE
        acceptor.cpp
        client_handler.cpp
        lobby.cpp
        match.cpp
        player_command.cpp
        sender.cpp
        main.cpp
    PUBLIC
        client_handler.h
        game_identification.h
        lobby.h
        match.h
        match_dto.h
        match_status_dto.h
        player_credentials.h
        player_dto.h
        player_command.h
        sender.h
        acceptor.h
        game_ready_notification.h
)

add_subdirectory(game)

target_link_libraries(taller_server game)

target_include_directories(taller_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})