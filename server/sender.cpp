#include "sender.h"

#include "common/dto/game_state_update.h"


void Sender::run() {
    while (should_keep_running()) {
        try {
            MatchStatusDTO status = sender_queue.pop();
            const auto player = status.players[0];
            protocol.send_message(GameStateUpdate(player.position_x, player.position_y));
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
    sender_queue.close();
}
