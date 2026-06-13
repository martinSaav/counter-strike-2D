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
    statistics.cpp
    statistics.ui
    resources.qrc
)

# Estos archivos van al ejecutable principal
target_sources(taller_client PRIVATE ${LOBBY_SOURCES})

# Incluir los headers de esta carpeta y de graphics
target_include_directories(taller_client PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../graphics
)

# Linkear Qt y la lib graphics
target_link_libraries(taller_client PRIVATE
    graphics
    Qt6::Widgets
)
