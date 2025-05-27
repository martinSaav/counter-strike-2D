#include "sender.h"

#include <list>

#include "common/dto/game_state_update.h"


void Sender::run() {
    while (should_keep_running()) {
        try {
            MatchStatusDTO status = sender_queue->pop();
            auto player_dtos = status.players;
            std::list<Player> players;
            for (const auto& player_dto: player_dtos) {
                Player player(player_dto.username, player_dto.position_x, player_dto.position_y,
                              100, Status::Alive, 500, 0, 0, Action::MoveUp);
                players.push_back(player);
            }
            const auto player = status.players[0];
            GameStateUpdate state(true, false, 1, 0, true, false, false, 0, 0, 0, Team::Terrorists,
                                  Team::Terrorists, players);
            protocol.send_message(state);
        } catch (const ClosedQueue&) {
            break;
        } catch (const std::exception& e) {
            std::cerr << "Unexpected exception: " << e.what() << std::endl;

        } catch (...) {
            std::cerr << "Unexpected exception: <unknown>\n";
        }
    }
}

void Sender::stop() {
    Thread::stop();
    sender_queue->close();
}
