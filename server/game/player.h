#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <utility>
#include <vector>

#include "../player_dto.h"

#include "player_skin.h"
#include "position.h"
#define player_hitbox_height 32
#define player_hitbox_width 32

class Player {
    const std::string username;
    PlayerSkin skin;
    int current_angle;
    int position_x;
    int position_y;
    std::vector<std::pair<int, int>>
            chunks_idxs;  // indices de los chunks en los que se encuentra el jugador

public:
    Player(std::string username, const int position_x, const int position_y):
            username(std::move(username)),
            current_angle(0),
            position_x(position_x),
            position_y(position_y) {
        skin = PlayerSkin::C1;
        chunks_idxs.emplace_back(0, 0);
    }
    std::pair<int, int> get_location();
    void set_location(Position position, std::vector<std::pair<int, int>>&& chunks_idxs);
    void set_angle(const int angle) { current_angle = angle; }
    void set_skin(const PlayerSkin skin) { this->skin = skin; }
    [[nodiscard]] PlayerDTO get_player_info() const;

    std::vector<std::pair<int, int>>& get_chunk_idxs() { return chunks_idxs; }

    bool operator==(const Player& player) const = default;
};

#endif  // PLAYER_H
