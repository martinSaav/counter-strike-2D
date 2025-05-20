#define INPUTHANDLER_H
#ifdef INPUTHANDLER_H
#include <string>
#include <queue>
#include <optional>
#include <mutex>

class InputHandler {
    private:
    bool quit = false;
    std::queue<std::string> mensajes;
    std::mutex mtx;

    public:
    InputHandler();
    ~InputHandler();  

    void processEvents();

    std::optional<std::string> getMensaje();

    bool exitGame();
};
#endif
