//
// Created by matias on 03/06/25.
//

#ifndef GAME_READY_NOTIFICATION_H
#define GAME_READY_NOTIFICATION_H

// success en false avisa al jugador que pidio arrancar que la partida no se puede iniciar
// (por ejemplo, porque algun equipo no tiene jugadores).
class GameReadyNotification {
public:
    bool success = true;
};

#endif  // GAME_READY_NOTIFICATION_H
