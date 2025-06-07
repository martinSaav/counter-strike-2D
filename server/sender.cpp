#include "sender.h"

#include <list>

#include "common/dto/game_ready_response.h"
#include "common/dto/game_state_update.h"

#include "skin_translator.h"


void Sender::send_status(const MatchStatusDTO& status) const {
    const auto player_dtos = status.players;
    std::list<PlayerInfo> players;
    for (const auto& player_dto: player_dtos) {
        PlayerInfo player(player_dto.username, player_dto.position_x, player_dto.position_y, 100,
                          Status::Alive, 500, 0, 0, Action::MoveUp, 0, 0,
                          SkinTranslator::code_to_string(player_dto.skin));
        players.push_back(player);
    }
    const GameStateUpdate state(true, false, 1, 0, true, false, false, 0, 0, 0, Team::Terrorists,
                                Team::Terrorists, std::move(players));
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
