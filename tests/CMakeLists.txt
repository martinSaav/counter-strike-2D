target_sources(taller_tests
    PRIVATE
    # .cpp files
        test_protocol.cpp
    PUBLIC
    # .h files
        mock_socket.h
    )

# Tests de las reglas del juego: necesitan la logica del servidor (libreria game)
if(TARGET game)
    target_sources(taller_tests
        PRIVATE
            test_game_rules.cpp
            ${PROJECT_SOURCE_DIR}/server/game_config.cpp
            ${PROJECT_SOURCE_DIR}/server/gun_shop.cpp
            ${PROJECT_SOURCE_DIR}/server/skin_translator.cpp
        )
    target_link_libraries(taller_tests game)
endif()
