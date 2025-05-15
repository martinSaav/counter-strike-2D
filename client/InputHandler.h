#define INPUTHANDLER_H
#ifdef INPUTHANDLER_H
#include <string>
#include <queue>
#include <optional>

class InputHandler {
    private:
    bool quit = false;
    std::queue<std::string> mensajes;

    public:
    InputHandler();
    ~InputHandler();  

    void processEvents();

    std::optional<std::string> getMensaje() ;
};
#endif
