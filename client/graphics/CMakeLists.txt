# Definimos los archivos fuente directamente
add_library(graphics STATIC
    chatClient.cpp
    render.cpp
    inputHandler.cpp
    inputServerHandler.cpp
    textureStore.cpp
    configuration.h
    musicManager.cpp
    components/hudSprite.cpp
    components/mapSprite.cpp
    components/playerSprite.cpp
)

# Hacemos visible el directorio actual para los includes
target_include_directories(graphics
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${SDL2_SOURCE_DIR}/include
        ${libSDL2pp_SOURCE_DIR}/include
        ${SDL2_image_SOURCE_DIR}/include
)

find_package(SDL2 REQUIRED)

# Add SDL2 include directories
target_include_directories(graphics PUBLIC ${SDL2_INCLUDE_DIRS})

# Link SDL2 library
target_link_libraries(graphics PUBLIC ${SDL2_LIBRARIES} SDL2::SDL2 SDL2pp::SDL2pp SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer)
