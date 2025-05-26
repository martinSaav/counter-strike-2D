#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <utility>
#include <vector>

#include "../player_dto.h"

#include "position.h"
#define player_hitbox_height 5
#define player_hitbox_width 5

class Player {
    const std::string username;
    int position_x;
    int position_y;
    std::vector<std::pair<int, int>>
            chunks_idxs;  // indices de los chunks en los que se encuentra el jugador

public:
    Player(const std::string& username, const int position_x, const int position_y):
            username(username), position_x(position_x), position_y(position_y) {
        chunks_idxs.emplace_back(0, 0);
    }
    std::pair<int, int> get_location();
    void set_location(Position position, std::vector<std::pair<int, int>>&& chunks_idxs);
    [[nodiscard]] PlayerDTO get_player_info() const;

    std::vector<std::pair<int, int>>& get_chunk_idxs() { return chunks_idxs; }

    bool operator==(const Player& player) const = default;
};

#endif  // PLAYER_H
