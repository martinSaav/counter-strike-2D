#include "render.h"
#include "../../common/dto/game_state_update.h"
#include <cmath>

using namespace SDL2pp;

Render::Render(Renderer* renderer, Protocol& protocolo):
    sdlRenderer(renderer),
    texturas(),
    protocolo(protocolo)
{
    // Cargamos las texturas
    texturas.loadTexture("fondo", "../client/data/mapa.png", sdlRenderer);
    texturas.loadTexture("mira", "../client/data/mira.png", sdlRenderer);
    texturas.loadTexture("sprites", "../client/data/jugador.png", sdlRenderer);
}
//SetColorKey(true, 0))
void Render::renderFrame(int posX, int posY){

    Texture& fondo = texturas.getTexture("fondo");
    Texture& sprites = texturas.getTexture("sprites");
    Texture& mira = texturas.getTexture("mira");

    const int jugador_ancho = 25;
    const int jugador_alto = 25;

    // Timing: calculate difference between this and previous frame
    // in milliseconds
    unsigned int frame_ticks = SDL_GetTicks();
    unsigned int frame_delta = frame_ticks - prev_ticks;
    prev_ticks = frame_ticks;

    // Ajustar tamaño de cámara con zoom
    float zoom = 4.0f;
    int cam_w = sdlRenderer->GetOutputWidth() / zoom;
    int cam_h = sdlRenderer->GetOutputHeight() / zoom;

    // Calcular la posición de la cámara centrada en el jugador
    int mapa_x = posX - cam_w / 2;
    int mapa_y = posY - cam_h / 2;

    // Limitar para que no se salga del fondo
    mapa_x = std::max(0, std::min(mapa_x, fondo.GetWidth() - cam_w));
    mapa_y = std::max(0, std::min(mapa_y, fondo.GetHeight() - cam_h));

    sdlRenderer->Clear();
    
    int screen_w = sdlRenderer->GetOutputWidth();
    int screen_h = sdlRenderer->GetOutputHeight();

    // --- DIBUJAR fondo ---
    sdlRenderer->Copy(
        fondo,
        Rect(mapa_x, mapa_y, cam_w, cam_h),   // qué parte del fondo recortar
        Rect(0, 0, screen_w, screen_h));      // se escala para que llene la pantalla

    // --- DIBUJAR jugador centrado ---
    int player_screen_x = (posX - mapa_x) * zoom;
    int player_screen_y = (posY - mapa_y) * zoom;

    SDL_GetMouseState(&mouseX, &mouseY);

    // Convertir coordenadas de mouse (pantalla) a coordenadas de mapa
    int mouse_map_x = mouseX / zoom + mapa_x;
    int mouse_map_y = mouseY / zoom + mapa_y;

    angle = getAnglePlayer(posX, posY, mouse_map_x, mouse_map_y);
    
    sdlRenderer->Copy(
    sprites,
    Rect(0, 0, 79, 79),  // sprite en el atlas
    Rect(player_screen_x, player_screen_y, 25 * zoom, 25 * zoom),  // en pantalla
    angle);

    sdlRenderer->Copy(
        mira,
        SDL2pp::NullOpt, // recorte del fondo
        Rect(mouseX, mouseY, 45, 45)            // dibujado a pantalla
    );

    // Show rendered frame
    sdlRenderer->Present();
}

double Render::getAnglePlayer(int jugadorX, int jugadorY, int mousex, int mousey){ 
    // Calcular el ángulo entre el jugador y el mouse
    int dx = mousex - jugadorX;
    int dy = mousey - jugadorY;

    double angleRadians = atan2(dy, dx); // da un ángulo en radianes entre -π y π
    double angleDegrees = angleRadians * 180 / M_PI;

    return angleDegrees + 90.0; // Ajustar para que apunte hacia la dirección del mouse
}
