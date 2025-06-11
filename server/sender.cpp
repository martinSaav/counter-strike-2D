#include "sender.h"

#include <list>

#include "catedra/liberror.h"
#include "common/dto/game_ready_response.h"
#include "common/dto/game_state_update.h"

#include "skin_translator.h"


void Sender::send_status(const MatchStatusDTO& status) const {
    const auto player_dtos = status.players;
    std::list<PlayerInfo> players;
    for (const auto& player_dto: player_dtos) {
        PlayerInfo player(player_dto.username, player_dto.position_x, player_dto.position_y,
                          player_dto.health, player_dto.status, player_dto.money, player_dto.kills,
                          player_dto.deaths, player_dto.last_action, player_dto.aim_x,
                          player_dto.aim_y, SkinTranslator::code_to_string(player_dto.skin),
                          player_dto.primary_weapon, player_dto.primary_weapon_ammo,
                          player_dto.secondary_weapon, player_dto.secondary_weapon_ammo,
                          player_dto.knife, player_dto.bomb, player_dto.current_weapon,
                          player_dto.current_weapon_ammo);
        players.push_back(player);
    }
    const GameStateUpdate state(status.game_started, status.game_ended, status.round,
                                status.round_time, status.round_started, status.round_ended,
                                status.bomb_planted, status.bomb_x, status.bomb_y,
                                status.bomb_timer, status.round_winner, status.game_winner,
                                std::move(players), status.dropped_weapons);
    protocol.send_message(state);
}


void Sender::run() {
    while (should_keep_running()) {
        try {
            if (auto status_v = sender_queue->pop();
                std::holds_alternative<MatchStatusDTO>(status_v)) {
                send_status(std::get<MatchStatusDTO>(status_v));
            } else {
                protocol.send_message(GameReadyResponse(true));
            }
        } catch (const ClosedQueue&) {
            running = false;
            return;
        } catch (const LibError&) {
            break;
        } catch (const std::exception& e) {
            std::cerr << "Unexpected exception: " << e.what() << std::endl;
            break;
        } catch (...) {
            std::cerr << "Unexpected exception: <unknown>\n";
            break;
        }
    }
    running = false;
}


bool Sender::is_running() { return running; }


void Sender::stop() {
    Thread::stop();
    try {
        sender_queue->close();
    } catch (const ClosedQueue&) {}
    running = false;
}
