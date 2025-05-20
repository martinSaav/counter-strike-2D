#include "Render.h"

using namespace SDL2pp;

Render::Render(Renderer* renderer):
    sdlRenderer(renderer),
    fondo(*sdlRenderer, Surface(IMG_Load("../client/data/mapa.png"))),
    mira(*sdlRenderer, Surface(IMG_Load("../client/data/mira.png"))),
    sprites(*sdlRenderer, Surface(IMG_Load("../client/data/jugador.png")).SetColorKey(true, 0))
{
}

void Render::renderFrame(std::string mensaje){

    const int jugador_ancho = 25;
    const int jugador_alto = 25;

    // Timing: calculate difference between this and previous frame
    // in milliseconds
    unsigned int frame_ticks = SDL_GetTicks();
    unsigned int frame_delta = frame_ticks - prev_ticks;
    prev_ticks = frame_ticks;

    // Update game state for this frame:
    // if character is runnung, move it to the right
    if (mensaje == "d" && posJugadorX < fondo.GetWidth() - jugador_ancho){
        posJugadorX += frame_delta * 0.2;
        angle = 90.0;
    } 

    if (mensaje == "a" && posJugadorX > 0){
        posJugadorX -= frame_delta * 0.2;
        angle = 270.0;
    } 

    if (mensaje == "w" && posJugadorY > 0){
        posJugadorY -= frame_delta * 0.2;
        angle = 0.0;
    } 

    if (mensaje == "s" && posJugadorY < fondo.GetHeight() - jugador_alto){
        posJugadorY += frame_delta * 0.2;
        angle = 180.0;
    }

    if (mensaje == "wd" && posJugadorX < fondo.GetWidth() - jugador_ancho && posJugadorY > 0){
        posJugadorX += frame_delta * 0.2;
        posJugadorY -= frame_delta * 0.2;
        angle = 45.0;
    }

    if (mensaje == "wa" && posJugadorX > 0 && posJugadorY > 0){
        posJugadorX -= frame_delta * 0.2;
        posJugadorY -= frame_delta * 0.2;
        angle = 315.0;
    }

    if (mensaje == "sd" && posJugadorX < fondo.GetWidth() - jugador_ancho && posJugadorY < fondo.GetHeight() - jugador_alto){
        posJugadorX += frame_delta * 0.2;
        posJugadorY += frame_delta * 0.2;
        angle = 135.0;
    }

    if (mensaje == "sa" && posJugadorX > 0 && posJugadorY < fondo.GetHeight() - jugador_alto){
        posJugadorX -= frame_delta * 0.2;
        posJugadorY += frame_delta * 0.2;
        angle = 225.0;
    }

    // Ajustar tamaño de cámara con zoom
    float zoom = 4.0f;
    int cam_w = sdlRenderer->GetOutputWidth() / zoom;
    int cam_h = sdlRenderer->GetOutputHeight() / zoom;

    // Calcular la posición de la cámara centrada en el jugador
    int mapa_x = posJugadorX - cam_w / 2;
    int mapa_y = posJugadorY - cam_h / 2;

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
    int player_screen_x = (posJugadorX - mapa_x) * zoom;
    int player_screen_y = (posJugadorY - mapa_y) * zoom;

    sdlRenderer->Copy(
    sprites,
    Rect(0, 0, 79, 79),  // sprite en el atlas
    Rect(player_screen_x, player_screen_y, 25 * zoom, 25 * zoom),  // en pantalla
    angle);

    SDL_GetMouseState(&mouseX, &mouseY);

    sdlRenderer->Copy(
        mira,
        SDL2pp::NullOpt, // recorte del fondo
        Rect(mouseX, mouseY, 45, 45)            // dibujado a pantalla
    );

    // Show rendered frame
    sdlRenderer->Present();
}
