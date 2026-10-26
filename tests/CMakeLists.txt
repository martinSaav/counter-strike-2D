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

# Tests del modelo del editor de niveles (lectura, escritura y validacion de mapas)
if(TARGET editor_model)
    target_sources(taller_tests PRIVATE test_map_document.cpp)
    target_link_libraries(taller_tests editor_model)
    target_compile_definitions(taller_tests PRIVATE TALLER_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
    # Con el servidor compilado se prueba tambien que lea un mapa guardado por el editor
    if(TARGET game)
        target_sources(taller_tests
            PRIVATE
                ${PROJECT_SOURCE_DIR}/server/configuration_loader.cpp
                ${PROJECT_SOURCE_DIR}/server/weapon_translator.cpp
            )
        target_link_libraries(taller_tests yaml-cpp)
        target_compile_definitions(taller_tests PRIVATE TALLER_TEST_SERVER_LOADER)
    endif()
endif()
