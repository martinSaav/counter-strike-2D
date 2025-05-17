#include "sender.h"

void Sender::notify_error_to_client_handler() {
    client_handler.receive_notification_about_sender_error();
    try {
        sender_queue.close();
    } catch (const ClosedQueue&) {}
}


void Sender::run() {
    while (should_keep_running()) {
        try {
            MatchStatusDTO status = sender_queue.pop();
            protocol.send(status);
        } catch (const ClosedQueue&) {
            break;
        } catch (const std::exception& e) {
            std::cerr << "Unexpected exception: " << e.what() << std::endl;
            notify_error_to_client_handler();

        } catch (...) {
            std::cerr << "Unexpected exception: <unknown>\n";
            notify_error_to_client_handler();
        }
    }
}

void Sender::stop() {
    Thread::stop();
    sender_queue.close();
}
