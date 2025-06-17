# Archivos fuente
set(LOBBY_SOURCES
    main.cpp
    lobbywindow.cpp
    lobbywindow.ui
    creatematchwindow.cpp
    creatematchwindow.ui
    loginwindow.cpp
    loginwindow.ui
    waitroom.cpp
    waitroom.ui
    selectskin.cpp
    selectskin.ui
    estadistics.cpp
    estadistics.ui
    recursos.qrc
)

# Estos archivos van al ejecutable principal
target_sources(taller_client PRIVATE ${LOBBY_SOURCES})

# Incluir los headers de esta carpeta y de graphies
target_include_directories(taller_client PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../graphies
)

# Linkear Qt y la lib graphies
target_link_libraries(taller_client PRIVATE
    graphies
    Qt6::Widgets
)
