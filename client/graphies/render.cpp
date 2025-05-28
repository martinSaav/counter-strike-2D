#include "render.h"

#include <algorithm>
#include <cmath>
#include <list>

using SDL2pp::Rect;

Render::Render(Renderer* renderer, Protocol& protocolo, std::string& namePlayer):
        sdlRenderer(renderer), texturas(), protocolo(protocolo), namePlayer(namePlayer) {
    // Cargamos las texturas
    texturas.loadTexture("fondo", "../client/data/maps/default_aztec.png", sdlRenderer);
    texturas.loadTexture("mira", "../client/data/mira.png", sdlRenderer);
    texturas.loadTexture("personaje", "../client/data/players/t1.bmp", sdlRenderer);
}

void Render::renderFrame(std::optional<GameStateUpdate> mensaje){

    std::list<PlayerInfo> jugadores = mensaje->get_players();

    Texture& fondo = texturas.getTexture("fondo");
    Texture& personaje = texturas.getTexture("personaje");
    Texture& mira = texturas.getTexture("mira");

    float zoom = 10.0f;

    int screenWidth = 1000;
    int screenHeight = 1000;

    int worldWidth = fondo.GetWidth();
    int worldHeight = fondo.GetHeight();

    int camWidth = screenWidth / zoom;
    int camHeight = screenHeight / zoom;

    // Calcula la posición del jugador
    for (auto const& jugador : jugadores) {
        if (jugador.get_user_name() == namePlayer) {
            posJugadorX = jugador.get_pos_x();
            posJugadorY = jugador.get_pos_y();
        }
    }

    SDL_Rect camera = {0, 0, camWidth, camHeight};

    camera.x = posJugadorX - camWidth / 2;
    camera.y = posJugadorY - camHeight / 2;

    // Limitar bordes
    if (camera.x < 0) camera.x = 0;
    if (camera.y < 0) camera.y = 0;
    if (camera.x > worldWidth - camWidth) camera.x = worldWidth - camWidth;
    if (camera.y > worldHeight - camHeight) camera.y = worldHeight - camHeight;

    SDL_Rect srcRect = {camera.x, camera.y, camWidth, camHeight};
    SDL_Rect destRect = {0, 0, screenWidth, screenHeight};  // dibuja el fondo en toda la ventana

    sdlRenderer->Clear();
    sdlRenderer->Copy(fondo, srcRect, destRect);

    // Angulo default
    double angleDefault = 0.0;

    // Obtengo mi angulo
    SDL_GetMouseState(&mouseX, &mouseY);
    int mouse_map_x = int(mouseX / zoom + camera.x);
    int mouse_map_y = int(mouseY / zoom + camera.y);

    myAngle = getAnglePlayer(posJugadorX, posJugadorY, mouse_map_x, mouse_map_y);

    // Dibuja jugadores con zoom
    for (auto const& jugador : jugadores) {
        angleDefault = 0.0;
        if (jugador.get_user_name() == namePlayer) angleDefault = myAngle;

        SDL_Rect srcRectPlayers = {0, 0, 32, 32};
        SDL_Rect destRectPlayers = {
            int((jugador.get_pos_x() - camera.x) * zoom),
            int((jugador.get_pos_y() - camera.y) * zoom),
            int(32 * zoom / 4),
            int(32 * zoom / 4)
        };

        sdlRenderer->Copy(personaje, srcRectPlayers, destRectPlayers, angleDefault);
    }

    // Dibuja mira sin zoom (porque está en coords de pantalla)
    sdlRenderer->Copy(mira, SDL2pp::NullOpt, Rect(mouseX, mouseY, 45, 45));

    sdlRenderer->Present();
}

double Render::getAnglePlayer(int jugadorX, int jugadorY, int mousex, int mousey) {

    // Calcular el ángulo entre el jugador y el mouse
    int dx = mousex - jugadorX;
    int dy = mousey - jugadorY;

    double angleRadians = atan2(dy, dx);  // da un ángulo en radianes entre -π y π
    double angleDegrees = angleRadians * 180 / M_PI;

    return angleDegrees + 90.0;  // Ajustar para que apunte hacia la dirección del mouse
}
