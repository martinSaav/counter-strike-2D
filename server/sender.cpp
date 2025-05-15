//
// Created by matias on 15/05/25.
//

#include "sender.h"

void Sender::run() {
    while (should_keep_running()) {
        try {
            MatchStatusDTO status = sender_queue.pop();
            protocol.send(status);
        } catch (const ClosedQueue&) {
            break;
        } catch (const std::exception& e) {
            std::cerr << "Unexpected exception: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unexpected exception: <unknown>\n";
        }
    }
}
