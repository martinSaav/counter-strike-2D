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
// SetColorKey(true, 0))
void Render::renderFrame(std::optional<GameStateUpdate> mensaje) {

    std::list<PlayerInfo> jugadores = mensaje->get_players();

    Texture& fondo = texturas.getTexture("fondo");
    Texture& personaje = texturas.getTexture("personaje");
    Texture& mira = texturas.getTexture("mira");

    // const int jugador_ancho = 25;
    // const int jugador_alto = 25;

    // Timing: calculate difference between this and previous frame
    // in milliseconds
    unsigned int frame_ticks = SDL_GetTicks();
    // unsigned int frame_delta = frame_ticks - prev_ticks;
    prev_ticks = frame_ticks;

    // Ajustar tamaño de cámara con zoom
    float zoom = 10.0f;

    // Calculo la posicion de la camara en mi jugador
    for (auto const& jugador: jugadores) {
        if (jugador.get_user_name() == namePlayer) {
            posJugadorX = jugador.get_pos_x();
            posJugadorY = jugador.get_pos_y();
        }
    }
    int cam_w = sdlRenderer->GetOutputWidth() / zoom;
    int cam_h = sdlRenderer->GetOutputHeight() / zoom;

    // Calcular la posición de la cámara centrada en el jugador
    int mapa_jug_x = posJugadorX - cam_w / 2;
    int mapa_jug_y = posJugadorY - cam_h / 2;

    // Limitar para que no se salga del fondo
    mapa_jug_x = std::max(0, std::min(mapa_jug_x, fondo.GetWidth() - cam_w));
    mapa_jug_y = std::max(0, std::min(mapa_jug_y, fondo.GetHeight() - cam_h));

    sdlRenderer->Clear();

    int screen_w = sdlRenderer->GetOutputWidth();
    int screen_h = sdlRenderer->GetOutputHeight();

    // --- DIBUJAR fondo ---
    sdlRenderer->Copy(fondo,
                      Rect(mapa_jug_x, mapa_jug_y, cam_w, cam_h),  // qué parte del fondo recortar
                      Rect(0, 0, screen_w, screen_h));  // se escala para que llene la pantalla

    for (auto const& jugador: jugadores) {

        // Mi personaje lo salteo
        if (jugador.get_user_name() == namePlayer) {
            continue;
        }

        sdlRenderer->Copy(personaje, Rect(0, 0, 32, 32),  // sprite en el atlas
                          Rect(jugador.get_pos_x(), jugador.get_pos_y(), 32 * 2, 32 * 2));
    }
    // --- DIBUJAR jugador centrado ---
    // int player_screen_x = (posMyUserX - mapa_x) * zoom;
    // int player_screen_y = (posMyUserY - mapa_y) * zoom;

    SDL_GetMouseState(&mouseX, &mouseY);

    // Convertir coordenadas de mouse (pantalla) a coordenadas de mapa
    // int mouse_map_x = mouseX / zoom + mapa_x;
    // int mouse_map_y = mouseY / zoom + mapa_y;

    angle = getAnglePlayer(posJugadorX, posJugadorY, mouseX, mouseY);

    sdlRenderer->Copy(personaje, Rect(0, 0, 32, 32),                   // sprite en el atlas
                      Rect(posJugadorX, posJugadorY, 32 * 2, 32 * 2),  // en pantalla
                      angle);

    sdlRenderer->Copy(mira,
                      SDL2pp::NullOpt,                // recorte del fondo
                      Rect(mouseX, mouseY, 45, 45));  // dibujado a pantalla


    // Show rendered frame
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
