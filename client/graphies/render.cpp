#include "render.h"

#include <algorithm>
#include <cmath>
#include <list>

using SDL2pp::Rect;

Render::Render(Renderer* renderer, Protocol& protocolo, std::string& namePlayer, Configuracion& configuracion):
    sdlRenderer(renderer), protocolo(protocolo), namePlayer(namePlayer),
    hud(sdlRenderer, screenWidth, screenHeight), mapa(sdlRenderer), player(sdlRenderer),
    configuracion(configuracion)
{
}

void Render::renderFrame(std::optional<GameStateUpdate> mensaje){

    std::list<PlayerInfo> jugadores = mensaje->get_players();

    float zoom = configuracion.zoom;

    int worldWidth = mapa.getWidth();
    int worldHeight = mapa.getHeight();

    int camWidth = screenWidth / zoom;
    int camHeight = screenHeight / zoom;

    // Calcula la posición del jugador
    for (auto& jugador : jugadores){
        if (jugador.get_user_name() == namePlayer){
            myPlayer = &jugador;
        }
    }

    SDL_Rect& camera = configuracion.camera;
    camera = {0, 0, camWidth, camHeight};

    camera.x = myPlayer->get_pos_x() - camWidth / 2;
    camera.y = myPlayer->get_pos_y() - camHeight / 2;

    // Limitar bordes
    if (camera.x < 0) camera.x = 0;
    if (camera.y < 0) camera.y = 0;
    if (camera.x > worldWidth - camWidth) camera.x = worldWidth - camWidth;
    if (camera.y > worldHeight - camHeight) camera.y = worldHeight - camHeight;

    sdlRenderer->Clear();

    mapa.draw(camera, camWidth, camHeight); //Dibujo el mapa

    // Angulo default
    double anglePlayer = 0.0;

    // Obtengo mi angulo
    SDL_GetMouseState(&mouseX, &mouseY);
    int mouse_map_x = int(mouseX / zoom + camera.x);
    int mouse_map_y = int(mouseY / zoom + camera.y);

    // Dibuja jugadores con zoom
    int posPlayerX = 0;
    int posPlayerY = 0;
    int mousePlayerX = 0;
    int mousePlayerY = 0;
    for (auto const& jugador : jugadores){

        if (jugador.get_user_name() == namePlayer){
            anglePlayer = getAnglePlayer(jugador.get_pos_x(), jugador.get_pos_y(), mouse_map_x, mouse_map_y);

        } else{
            posPlayerX = jugador.get_pos_x();
            posPlayerY = jugador.get_pos_y();
            mousePlayerX = jugador.get_pos_shoot_x();
            mousePlayerY = jugador.get_pos_shoot_y();
            anglePlayer = getAnglePlayer(posPlayerX, posPlayerY, mousePlayerX, mousePlayerY);
        }
        player.drawPlayer(jugador, camera, zoom, anglePlayer);
    }

    int health = myPlayer->get_health();
    int money = myPlayer->get_money();
    int time = mensaje->get_round_time();
    int round = mensaje->get_round();
    
    SDL_Rect mouse = {mouseX, mouseY, 45, 45};
    hud.draw(mouse, health, money, time, round);

    sdlRenderer->Present();
}

double Render::getAnglePlayer(int jugadorX, int jugadorY, int mousex, int mousey){

    // Calcular el ángulo entre el jugador y el mouse
    int dx = mousex - jugadorX;
    int dy = mousey - jugadorY;

    double angleRadians = atan2(dy, dx);  // da un ángulo en radianes entre -π y π
    double angleDegrees = angleRadians * 180 / M_PI;

    return angleDegrees + 90.0;  // Ajustar para que apunte hacia la dirección del mouse
}
